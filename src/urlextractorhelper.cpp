#include "urlextractorhelper.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QRegularExpression>

UrlExtractorHelper::UrlExtractorHelper(QObject *parent)
    : QObject(parent),
      m_process(nullptr)
{
}

UrlExtractorHelper::~UrlExtractorHelper() {
    cancel();
}

QString UrlExtractorHelper::findYtDlpBinary() {
    QString appDir = QCoreApplication::applicationDirPath();
    QDir dir(appDir);

    // 1. 沿运行目录向上逐级追溯查找 tools/yt-dlp.exe 或 yt-dlp.exe
    for (int i = 0; i <= 6; ++i) {
        QString checkTools = dir.filePath("tools/yt-dlp.exe");
        if (QFileInfo::exists(checkTools)) return QFileInfo(checkTools).absoluteFilePath();

        QString checkLocal = dir.filePath("yt-dlp.exe");
        if (QFileInfo::exists(checkLocal)) return QFileInfo(checkLocal).absoluteFilePath();

        if (!dir.cdUp()) break;
    }

    // 2. 检查固定工程目录
    static const QStringList fallbackPaths = {
        "E:/AiToy/video/tools/yt-dlp.exe",
        "E:/AiToy/video/Release_Package/StarryAudioExtractor/tools/yt-dlp.exe",
        "E:/AiToy/video/build_release/tools/yt-dlp.exe",
        "E:/AiToy/video/yt-dlp.exe"
    };
    for (const QString &p : fallbackPaths) {
        if (QFileInfo::exists(p)) return p;
    }

    // 3. 检查环境变量 PATH
    QString sysExe = QStandardPaths::findExecutable("yt-dlp");
    if (!sysExe.isEmpty()) return sysExe;

    return "";
}

void UrlExtractorHelper::startExtraction(const QString &ffmpegPath, const UrlExtractionOptions &options) {
    if (isRunning()) {
        emit logMessage("⚠️ 网页提取任务正在执行中，请等待完成", "WARNING");
        return;
    }

    m_currentOptions = options;
    m_outputBuffer.clear();
    m_lastOutputFile.clear();

    QString ytdlpExe = findYtDlpBinary();
    if (ytdlpExe.isEmpty()) {
        emit logMessage("❌ 未找到网络提取引擎 (tools/yt-dlp.exe)！", "ERROR");
        emit extractionFinished(false, options.outputDirectory, "", "未找到 yt-dlp.exe");
        return;
    }

    QDir().mkpath(options.outputDirectory);
    QString outTemplate = QDir(options.outputDirectory).filePath("%(title)s.%(ext)s");

    QStringList args;
    args << "--newline";
    args << "--no-mtime";
    if (options.singleVideoOnly) {
        args << "--no-playlist";
    }

    if (!ffmpegPath.isEmpty()) {
        args << "--ffmpeg-location" << ffmpegPath;
    }

    args << "-x";
    args << "--audio-format" << options.targetFormat.toLower();
    args << "--audio-quality" << options.audioQuality;
    args << "-o" << outTemplate;
    args << options.url.trimmed();

    emit logMessage(QString("🌐 开始解析网络视频地址: %1").arg(options.url), "INFO");
    emit logMessage(QString("💾 目标音频输出目录: %1").arg(options.outputDirectory), "INFO");
    emit started();
    emit progressUpdated(0, "0 KB/s", "--:--");

    // 动态启动进程：提取开始时自动启动
    m_process = new QProcess(this);
    m_process->setProcessChannelMode(QProcess::MergedChannels);

    connect(m_process, &QProcess::readyRead, this, &UrlExtractorHelper::onProcessReadyRead);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &UrlExtractorHelper::onProcessFinished);
    connect(m_process, &QProcess::errorOccurred, this, &UrlExtractorHelper::onProcessError);

    m_process->start(ytdlpExe, args);
}

void UrlExtractorHelper::cancel() {
    if (m_process && m_process->state() != QProcess::NotRunning) {
        emit logMessage("⏹️ 用户终止了网络提取任务，正在自动关闭引擎...", "WARNING");
        m_process->kill();
        m_process->waitForFinished(1000);
        emit logMessage("⏹️ 网络提取引擎已关闭。", "INFO");
    }
}

bool UrlExtractorHelper::isRunning() const {
    return (m_process && m_process->state() != QProcess::NotRunning);
}

void UrlExtractorHelper::onProcessReadyRead() {
    if (!m_process) return;

    QByteArray data = m_process->readAll();
    m_outputBuffer += QString::fromUtf8(data);

    QStringList lines = m_outputBuffer.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);
    if (!m_outputBuffer.endsWith('\r') && !m_outputBuffer.endsWith('\n') && !lines.isEmpty()) {
        m_outputBuffer = lines.takeLast();
    } else {
        m_outputBuffer.clear();
    }

    for (const QString &line : lines) {
        parseYtDlpLine(line);
    }
}

void UrlExtractorHelper::parseYtDlpLine(const QString &line) {
    QString trimmed = line.trimmed();
    if (trimmed.isEmpty()) return;

    if (trimmed.startsWith("[download]")) {
        static QRegularExpression progRegex(R"(\[download\]\s+(\d+(?:\.\d+)?)%\s+of\s+[~]?\s*([\d\.]+[A-Za-z]+)\s+at\s+([\d\.]+[A-Za-z/]+)\s+ETA\s+(\d{2}:\d{2}))");
        QRegularExpressionMatch match = progRegex.match(trimmed);
        if (match.hasMatch()) {
            double percent = match.captured(1).toDouble();
            QString speed = match.captured(3);
            QString eta = match.captured(4);
            emit progressUpdated(static_cast<int>(percent), speed, eta);
            return;
        }

        if (trimmed.contains("100%")) {
            emit progressUpdated(100, "完成", "00:00");
        }
    }

    if (trimmed.contains("[ExtractAudio] Destination:")) {
        QString dest = trimmed.section("[ExtractAudio] Destination:", 1).trimmed();
        if (!dest.isEmpty()) {
            m_lastOutputFile = dest;
            emit logMessage(QString("🎛️ 音频流抽取并转码完成: %1").arg(QFileInfo(dest).fileName()), "SUCCESS");
            return;
        }
    }

    if (trimmed.startsWith("[info]") || trimmed.startsWith("[download] Destination")) {
        emit logMessage(trimmed, "INFO");
    } else if (trimmed.startsWith("ERROR:")) {
        emit logMessage(trimmed, "ERROR");
    }
}

void UrlExtractorHelper::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    bool success = (exitStatus == QProcess::NormalExit && exitCode == 0);

    if (success) {
        emit progressUpdated(100, "已完成", "00:00");
        emit logMessage(QString("🎉 网络音频提取成功！已直接保存至桌面"), "SUCCESS");
        emit logMessage("⏹️ 网络提取引擎已自动完成并退出。", "INFO");
        emit extractionFinished(true, m_currentOptions.outputDirectory, m_lastOutputFile, "");
    } else {
        QString errMsg = QString("网络提取失败 (错误代码: %1)").arg(exitCode);
        emit logMessage(QString("❌ %1").arg(errMsg), "ERROR");
        emit logMessage("⏹️ 网络提取引擎已自动关闭退出。", "INFO");
        emit extractionFinished(false, m_currentOptions.outputDirectory, "", errMsg);
    }

    if (m_process) {
        m_process->deleteLater();
        m_process = nullptr;
    }
}

void UrlExtractorHelper::onProcessError(QProcess::ProcessError error) {
    if (error == QProcess::FailedToStart) {
        emit logMessage("❌ 启动 yt-dlp 失败，请检查 tools/yt-dlp.exe 是否就绪！", "ERROR");
        emit extractionFinished(false, m_currentOptions.outputDirectory, "", "yt-dlp 启动失败");
    }
}
