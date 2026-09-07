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

    for (int i = 0; i <= 6; ++i) {
        QString checkTools = dir.filePath("tools/yt-dlp.exe");
        if (QFileInfo::exists(checkTools)) return QFileInfo(checkTools).absoluteFilePath();

        QString checkLocal = dir.filePath("yt-dlp.exe");
        if (QFileInfo::exists(checkLocal)) return QFileInfo(checkLocal).absoluteFilePath();

        if (!dir.cdUp()) break;
    }

    static const QStringList fallbackPaths = {
        "E:/AiToy/video/tools/yt-dlp.exe",
        "E:/AiToy/video/Release_Package/StarryAudioExtractor/tools/yt-dlp.exe",
        "E:/AiToy/video/build_release/tools/yt-dlp.exe",
        "E:/AiToy/video/yt-dlp.exe"
    };
    for (const QString &p : fallbackPaths) {
        if (QFileInfo::exists(p)) return p;
    }

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

    if (options.extractTarget == UrlExtractTarget::Audio) {
        // 提取纯音频模式
        args << "-x";
        args << "--audio-format" << options.targetFormat.toLower();
        args << "--audio-quality" << options.audioQuality;
        emit logMessage(QString("🌐 开始解析网络视频并提取音频: %1 (目标格式: %2)").arg(options.url, options.targetFormat), "INFO");
    } else {
        // 下载完整高清视频模式
        QString formatRule = "bestvideo+bestaudio/best";
        if (options.videoResolution == "1080") {
            formatRule = "bestvideo[height<=1080]+bestaudio/best[height<=1080]/best";
        } else if (options.videoResolution == "720") {
            formatRule = "bestvideo[height<=720]+bestaudio/best[height<=720]/best";
        } else if (options.videoResolution == "480") {
            formatRule = "bestvideo[height<=480]+bestaudio/best[height<=480]/best";
        }
        args << "-f" << formatRule;
        args << "--merge-output-format" << options.videoContainer.toLower(); // 默认 mp4
        args << "--remux-video" << options.videoContainer.toLower();
        emit logMessage(QString("🎬 开始解析并下载网络高清视频: %1 (封装格式: %2, 目标清晰度: %3)")
                            .arg(options.url, options.videoContainer.toUpper(), options.videoResolution), "INFO");
    }

    args << "-o" << outTemplate;
    args << options.url.trimmed();

    emit logMessage(QString("💾 目标输出目录 (默认桌面): %1").arg(options.outputDirectory), "INFO");
    emit started();
    emit progressUpdated(0, "0 KB/s", "--:--");

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
        emit logMessage("⏹️ 用户终止了网络下载/提取任务，正在自动关闭引擎...", "WARNING");
        m_process->kill();
        m_process->waitForFinished(1000);
        emit logMessage("⏹️ 网络提取引擎已安全退出关闭。", "INFO");
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

    // 1. 捕获下载进度与速度: [download]  45.2% of ~  12.34MiB at    3.50MiB/s ETA 00:05
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

    // 2. 捕获纯音频抽取产物路径: [ExtractAudio] Destination: ...
    if (trimmed.contains("[ExtractAudio] Destination:")) {
        QString dest = trimmed.section("[ExtractAudio] Destination:", 1).trimmed();
        if (!dest.isEmpty()) {
            m_lastOutputFile = dest;
            emit logMessage(QString("🎛️ 音频流抽取并转码完成: %1").arg(QFileInfo(dest).fileName()), "SUCCESS");
            return;
        }
    }

    // 3. 捕获高清视频音画合流产物路径: [Merger] Merging formats into "..."
    if (trimmed.contains("[Merger] Merging formats into")) {
        QString dest = trimmed.section("[Merger] Merging formats into", 1).trimmed();
        dest = dest.remove('"').trimmed();
        if (!dest.isEmpty()) {
            m_lastOutputFile = dest;
            emit logMessage(QString("🎬 音视频高精合流完成: %1").arg(QFileInfo(dest).fileName()), "SUCCESS");
            return;
        }
    }

    // 4. 捕获视频重封装路径: [VideoRemuxer] Remuxing video from ... to "..."
    if (trimmed.contains("[VideoRemuxer] Remuxing video from")) {
        QString dest = trimmed.section(" to ", 1).trimmed();
        dest = dest.remove('"').trimmed();
        if (!dest.isEmpty()) {
            m_lastOutputFile = dest;
            emit logMessage(QString("🎬 视频封装格式转换完成: %1").arg(QFileInfo(dest).fileName()), "SUCCESS");
            return;
        }
    }

    // 5. 捕获常规单文件下载完成目标
    if (trimmed.startsWith("[download] Destination:")) {
        QString dest = trimmed.section("[download] Destination:", 1).trimmed();
        m_lastOutputFile = dest;
        emit logMessage(trimmed, "INFO");
        return;
    }

    // 6. 状态与错误捕获
    if (trimmed.startsWith("[info]")) {
        emit logMessage(trimmed, "INFO");
    } else if (trimmed.startsWith("ERROR:")) {
        emit logMessage(trimmed, "ERROR");
    }
}

void UrlExtractorHelper::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    bool success = (exitStatus == QProcess::NormalExit && exitCode == 0);

    if (success) {
        // 如果最后输出文件未被直接行捕获，从目标目录查找最新生成的文件
        if (m_lastOutputFile.isEmpty() || !QFileInfo::exists(m_lastOutputFile)) {
            QDir dir(m_currentOptions.outputDirectory);
            QStringList filters;
            if (m_currentOptions.extractTarget == UrlExtractTarget::Video) {
                filters << "*.mp4" << "*.mkv" << "*.webm" << "*.flv";
            } else {
                filters << "*.mp3" << "*.m4a" << "*.wav" << "*.flac" << "*.aac" << "*.ogg";
            }
            QFileInfoList files = dir.entryInfoList(filters, QDir::Files, QDir::Time);
            if (!files.isEmpty()) {
                m_lastOutputFile = files.first().absoluteFilePath();
            }
        }

        emit progressUpdated(100, "已完成", "00:00");
        if (m_currentOptions.extractTarget == UrlExtractTarget::Video) {
            emit logMessage("🎉 网络高清视频下载完成！已直接保存至桌面", "SUCCESS");
        } else {
            emit logMessage("🎉 网络音频提取完成！已直接保存至桌面", "SUCCESS");
        }
        emit logMessage("⏹️ 网络提取引擎任务完成，已自动退出关闭。", "INFO");
        emit extractionFinished(true, m_currentOptions.outputDirectory, m_lastOutputFile, "");
    } else {
        QString errMsg = QString("网络处理失败 (错误代码: %1)").arg(exitCode);
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
