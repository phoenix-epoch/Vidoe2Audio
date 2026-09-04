#include "ffmpeghelper.h"
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QRegularExpression>
#include <QDebug>

FFmpegHelper::FFmpegHelper(QObject *parent)
    : QObject(parent),
      m_process(nullptr),
      m_totalDurationSec(0.0),
      m_currentSec(0.0),
      m_durationParsed(false)
{
}

FFmpegHelper::~FFmpegHelper() {
    cancelExtraction();
}

QStringList FFmpegHelper::supportedVideoExtensions() {
    return QStringList{
        "*.mp4", "*.mkv", "*.avi", "*.mov", "*.flv", "*.wmv", "*.webm",
        "*.ts", "*.m4v", "*.3gp", "*.rmvb", "*.rm", "*.vob", "*.mpg",
        "*.mpeg", "*.ogv", "*.mts", "*.m2ts", "*.asf", "*.f4v", "*.divx"
    };
}

QStringList FFmpegHelper::supportedAudioFormats() {
    return QStringList{
        "MP3", "AAC", "WAV", "FLAC", "M4A", "OGG", "OPUS", "WMA", "AC3", "MKA", "ALAC", "AIFF"
    };
}

QString FFmpegHelper::getAudioExtensionForFormat(const QString &format) {
    QString lower = format.trimmed().toLower();
    if (lower == "alac") return "m4a";
    return lower;
}

QString FFmpegHelper::getFilterStringForVideoFiles() {
    QStringList exts = supportedVideoExtensions();
    QString filter = QString("常用音视频文件 (%1);;所有文件 (*.*)").arg(exts.join(" "));
    return filter;
}

QString FFmpegHelper::findFFmpegBinary(const QString &customPath) {
    if (!customPath.isEmpty()) {
        QFileInfo fi(customPath);
        if (fi.exists() && fi.isExecutable()) {
            return fi.absoluteFilePath();
        }
    }

    // 1. 检查应用程序所在目录
    QString appDir = QCoreApplication::applicationDirPath();
    QString localExe = QDir(appDir).filePath("ffmpeg.exe");
    if (QFileInfo::exists(localExe)) {
        return localExe;
    }

    // 2. 检查程序同级或上一级 tools 目录
    QString toolsExe = QDir(appDir).filePath("tools/ffmpeg.exe");
    if (QFileInfo::exists(toolsExe)) {
        return toolsExe;
    }

    // 3. 检查系统环境变量 PATH
    QString systemExe = QStandardPaths::findExecutable("ffmpeg");
    if (!systemExe.isEmpty()) {
        return systemExe;
    }

    return "";
}

bool FFmpegHelper::checkFFmpegExecutable(const QString &ffmpegPath, QString *versionOut) {
    if (ffmpegPath.isEmpty()) return false;

    QProcess proc;
    proc.start(ffmpegPath, QStringList() << "-version");
    if (!proc.waitForStarted(2000)) {
        return false;
    }

    if (!proc.waitForFinished(3000)) {
        proc.kill();
        return false;
    }

    if (proc.exitCode() == 0) {
        QString output = QString::fromUtf8(proc.readAllStandardOutput());
        if (output.isEmpty()) {
            output = QString::fromUtf8(proc.readAllStandardError());
        }
        if (versionOut) {
            QString firstLine = output.split('\n').value(0).trimmed();
            *versionOut = firstLine;
        }
        return true;
    }
    return false;
}

QString FFmpegHelper::selectAudioCodec(const QString &format) {
    QString fmt = format.toLower();
    if (fmt == "mp3") return "libmp3lame";
    if (fmt == "aac") return "aac";
    if (fmt == "wav") return "pcm_s16le";
    if (fmt == "flac") return "flac";
    if (fmt == "m4a") return "aac";
    if (fmt == "ogg") return "libvorbis";
    if (fmt == "opus") return "libopus";
    if (fmt == "wma") return "wmav2";
    if (fmt == "ac3") return "ac3";
    if (fmt == "alac") return "alac";
    if (fmt == "aiff") return "pcm_s16be";
    return "";
}

void FFmpegHelper::startExtraction(const QString &ffmpegPath, const ExtractionOptions &options) {
    if (isRunning()) {
        emit logMessage("⚠️ 任务正在运行中，请等待或先取消当前任务", "WARNING");
        return;
    }

    m_currentOptions = options;
    m_totalDurationSec = 0.0;
    m_currentSec = 0.0;
    m_durationParsed = false;
    m_outputBuffer.clear();

    QString exec = findFFmpegBinary(ffmpegPath);
    if (exec.isEmpty()) {
        emit logMessage("❌ 未找到 FFmpeg 可执行文件，请在界面设置正确的 ffmpeg.exe 路径！", "ERROR");
        emit extractionFinished(false, "", "未找到 FFmpeg 可执行文件");
        return;
    }

    QFileInfo inputFi(options.inputFilePath);
    if (!inputFi.exists()) {
        emit logMessage(QString("❌ 输入文件不存在: %1").arg(options.inputFilePath), "ERROR");
        emit extractionFinished(false, "", "输入文件不存在");
        return;
    }

    // 构建 FFmpeg 参数列表
    QStringList args;
    args << "-hide_banner";
    if (options.overwriteOutput) {
        args << "-y";
    } else {
        args << "-n";
    }

    args << "-i" << options.inputFilePath;
    args << "-vn"; // 禁用视频轨

    if (options.mode == ExtractMode::DirectCopy) {
        // 极速流拷贝模式
        args << "-c:a" << "copy";
        emit logMessage("⚡ 启用极速无损流提取模式 (-c:a copy)", "INFO");
    } else {
        // 格式转码模式
        QString codec = selectAudioCodec(options.targetFormat);
        if (!codec.isEmpty()) {
            args << "-c:a" << codec;
        }

        // 码率设置
        if (options.bitrate != "Auto" && !options.bitrate.isEmpty()) {
            args << "-b:a" << options.bitrate;
        }

        // 采样率设置
        if (options.sampleRate != "Auto" && !options.sampleRate.isEmpty()) {
            args << "-ar" << options.sampleRate;
        }

        // 声道数设置
        if (options.channels == "2") {
            args << "-ac" << "2";
        } else if (options.channels == "1") {
            args << "-ac" << "1";
        }

        emit logMessage(QString("🎛️ 启用转码提取模式: 目标格式=[%1], 编码器=[%2], 码率=[%3]")
                            .arg(options.targetFormat.toUpper())
                            .arg(codec.isEmpty() ? "默认" : codec)
                            .arg(options.bitrate), "INFO");
    }

    args << options.outputFilePath;

    emit logMessage(QString("🚀 执行指令: %1 %2").arg(exec, args.join(" ")), "INFO");
    emit started();
    emit progressUpdated(0, 0, 0);

    m_process = new QProcess(this);
    m_process->setProcessChannelMode(QProcess::MergedChannels);

    connect(m_process, &QProcess::readyRead, this, &FFmpegHelper::onProcessReadyRead);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &FFmpegHelper::onProcessFinished);
    connect(m_process, &QProcess::errorOccurred, this, &FFmpegHelper::onProcessError);

    m_process->start(exec, args);
}

void FFmpegHelper::cancelExtraction() {
    if (m_process && m_process->state() != QProcess::NotRunning) {
        emit logMessage("⏹️ 用户中断了提取任务...", "WARNING");
        m_process->kill();
        m_process->waitForFinished(1000);
    }
}

bool FFmpegHelper::isRunning() const {
    return (m_process && m_process->state() != QProcess::NotRunning);
}

void FFmpegHelper::onProcessReadyRead() {
    if (!m_process) return;

    QByteArray data = m_process->readAll();
    QString chunk = QString::fromUtf8(data);
    m_outputBuffer += chunk;

    // 按行拆分解析
    QStringList lines = m_outputBuffer.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);
    if (!m_outputBuffer.endsWith('\r') && !m_outputBuffer.endsWith('\n') && !lines.isEmpty()) {
        m_outputBuffer = lines.takeLast(); // 保留不完整的末行
    } else {
        m_outputBuffer.clear();
    }

    for (const QString &line : lines) {
        parseFFmpegOutput(line);
    }
}

void FFmpegHelper::parseFFmpegOutput(const QString &line) {
    QString trimmed = line.trimmed();
    if (trimmed.isEmpty()) return;

    // 1. 解析视频总时长: Duration: 00:01:23.45, start: ...
    if (!m_durationParsed && trimmed.contains("Duration:")) {
        static QRegularExpression durRegex(R"(Duration:\s*(\d{2}:\d{2}:\d{2}(?:\.\d+)?))");
        QRegularExpressionMatch match = durRegex.match(trimmed);
        if (match.hasMatch()) {
            QString timeStr = match.captured(1);
            m_totalDurationSec = parseTimeToSeconds(timeStr);
            m_durationParsed = true;
            emit logMessage(QString("⏱️ 检测到视频时长: %1 (%2 秒)").arg(timeStr).arg(m_totalDurationSec, 0, 'f', 1), "INFO");
        }
    }

    // 2. 解析实时提取进度: time=00:00:15.36
    if (trimmed.contains("time=")) {
        static QRegularExpression timeRegex(R"(time=\s*(\d{2}:\d{2}:\d{2}(?:\.\d+)?))");
        QRegularExpressionMatch match = timeRegex.match(trimmed);
        if (match.hasMatch()) {
            QString timeStr = match.captured(1);
            m_currentSec = parseTimeToSeconds(timeStr);

            int percent = 0;
            if (m_totalDurationSec > 0.0) {
                percent = qBound(0, static_cast<int>((m_currentSec / m_totalDurationSec) * 100.0), 99);
            }
            emit progressUpdated(percent, m_currentSec, m_totalDurationSec);
        }
    } else {
        // 输出普通的 ffmpeg 信息
        if (trimmed.startsWith("Stream #") || trimmed.startsWith("Input #") || trimmed.startsWith("Output #")) {
            emit logMessage(trimmed, "FFMPEG");
        }
    }
}

double FFmpegHelper::parseTimeToSeconds(const QString &timeStr) {
    QStringList parts = timeStr.split(':');
    if (parts.size() == 3) {
        double hours = parts[0].toDouble();
        double mins = parts[1].toDouble();
        double secs = parts[2].toDouble();
        return hours * 3600.0 + mins * 60.0 + secs;
    }
    return 0.0;
}

void FFmpegHelper::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    bool success = (exitStatus == QProcess::NormalExit && exitCode == 0);

    if (success) {
        emit progressUpdated(100, m_totalDurationSec, m_totalDurationSec);
        emit logMessage(QString("🎉 音频提取成功！文件已保存至: %1").arg(m_currentOptions.outputFilePath), "SUCCESS");
        emit extractionFinished(true, m_currentOptions.outputFilePath, "");
    } else {
        QString errMsg = QString("提取失败 (退出码: %1)").arg(exitCode);
        emit logMessage(QString("❌ %1").arg(errMsg), "ERROR");
        emit extractionFinished(false, m_currentOptions.outputFilePath, errMsg);
    }

    if (m_process) {
        m_process->deleteLater();
        m_process = nullptr;
    }
}

void FFmpegHelper::onProcessError(QProcess::ProcessError error) {
    if (error == QProcess::FailedToStart) {
        emit logMessage("❌ 启动 FFmpeg 进程失败，请检查路径或执行权限！", "ERROR");
        emit extractionFinished(false, "", "FFmpeg 启动失败");
    }
}
