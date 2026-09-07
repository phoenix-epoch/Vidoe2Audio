#include "ffmpeghelper.h"
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QRegularExpression>
#include <QMap>
#include <cmath>

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
    return QString("常见音视频文件 (%1);;所有文件 (*.*)").arg(exts.join(" "));
}

ExtensionDiagnosis FFmpegHelper::diagnoseExtension(const QString &rawInput) {
    QString ext = rawInput.trimmed();
    if (ext.startsWith(".")) {
        ext = ext.mid(1);
    }
    ext = ext.toLower();

    static const QMap<QString, ExtensionDiagnosis> knowledge = {
        {"mp4",  {true, true, "MP4 (MPEG-4 Part 14)", "主流通用容器", "极速无损 / MP3 / AAC", "通用性最强，音视频兼容性极佳"}},
        {"mkv",  {true, true, "MKV (Matroska)", "开源高清复合容器", "极速无损 / FLAC / MP3", "支持多音轨/内嵌字幕，无损抽取秒出"}},
        {"mov",  {true, true, "MOV (QuickTime)", "苹果高清容器", "极速无损 (M4A) / MP3", "高质量原生音频流，推荐直接抽取"}},
        {"avi",  {true, false, "AVI (Audio Video Interleaved)", "经典音视频交错", "转码为 MP3 / WAV", "老旧容器建议转码为标准 MP3 以防兼容问题"}},
        {"flv",  {true, true, "FLV (Flash Video)", "流媒体视频格式", "转码为 MP3 / AAC", "常见直播与流媒体录制，可完美提取音频"}},
        {"wmv",  {true, false, "WMV (Windows Media Video)", "微软流媒体格式", "转码为 MP3 / WMA", "内置 WMA 音频流，推荐转码为通用 MP3"}},
        {"webm", {true, true, "WebM", "Google 开源网页流容器", "极速无损 (Opus/M4A) / MP3", "HTML5 网页视频主流格式，音质纯净"}},
        {"ts",   {true, true, "TS (Transport Stream)", "广播传输流 / HLS", "转码为 MP3 / AAC", "网络分段视频流录像，完美支持音频聚合抽取"}},
        {"m2ts", {true, true, "M2TS (BDAV)", "蓝光高清音视频流", "无损 FLAC / WAV", "常见于蓝光盘与高清摄像机，原生无损音质"}},
        {"mts",  {true, true, "MTS (AVCHD)", "索尼/松下高清摄像", "转码为 MP3 / AAC", "摄像机原生素材，支持高清音轨无缝提取"}},
        {"m4v",  {true, true, "M4V", "苹果 iTunes 格式", "极速无损 (M4A) / MP3", "类似 MP4，内嵌 AAC 高品质音频"}},
        {"3gp",  {true, false, "3GP", "第三代移动端格式", "转码为 MP3", "老式手机录音视频，建议转码提升兼容性"}},
        {"rmvb", {true, false, "RMVB (RealMedia)", "经典动态压缩流", "转码为 MP3 / AAC", "早年经典影视资源，全兼容转码为 MP3"}},
        {"rm",   {true, false, "RM (RealMedia)", "RealNetworks 格式", "转码为 MP3", "经典音频编码，建议统一转码为 MP3"}},
        {"vob",  {true, false, "VOB (DVD Video Object)", "DVD 原盘媒体流", "转码为 MP3 / AC3", "DVD 光盘内嵌音频，支持直接提取"}},
        {"mpg",  {true, false, "MPEG-1/MPEG-2", "经典运动图像标准", "转码为 MP3", "老式 VCD/SVCD 音频抽取完全支持"}},
        {"mpeg", {true, false, "MPEG-1/MPEG-2", "经典运动图像标准", "转码为 MP3", "老式影视音视频流支持"}},
        {"ogv",  {true, true, "OGV (Ogg Video)", "Xiph 开源多媒体", "极速无损 (OGG) / MP3", "内嵌 Vorbis 音频，推荐直接抽取或转码"}},
        {"f4v",  {true, true, "F4V (Flash MP4)", "高清 Flash 视频", "极速无损 / MP3", "主流网络流录像格式支持"}},
        {"asf",  {true, false, "ASF (Advanced Systems)", "高级流媒体格式", "转码为 MP3 / WMA", "微软流媒体封装格式支持"}},
        {"divx", {true, false, "DivX", "高压缩视频流", "转码为 MP3", "支持该容器内音频提取"}}
    };

    if (knowledge.contains(ext)) {
        return knowledge[ext];
    }

    static const QStringList videoList = {
        "dav", "mxf", "bik", "nut", "amv", "roq", "drc", "gifv", "y4m"
    };

    if (videoList.contains(ext)) {
        return ExtensionDiagnosis{
            true, false, QString("%1 格式").arg(ext.toUpper()),
            "冷门/专用流媒体封装",
            "建议重编码为 MP3 / WAV",
            "FFmpeg 底层解复用器原生支持此封装，可直接抽取音频！"
        };
    }

    return ExtensionDiagnosis{
        false, false, "未知或非视频格式", "未知", "暂不支持",
        "未识别为此类视频后缀，如确为多媒体文件，可直接载入让 FFmpeg 尝试探测"
    };
}

double FFmpegHelper::estimateAudioSizeMB(double durationSec, const QString &format, const QString &bitrate) {
    if (durationSec <= 0.0) return 0.0;

    QString fmt = format.toLower();
    if (fmt == "wav") {
        return (durationSec * 44100.0 * 2.0 * 2.0) / (1024.0 * 1024.0);
    }

    if (fmt == "flac" || fmt == "alac") {
        double wavSize = (durationSec * 44100.0 * 2.0 * 2.0) / (1024.0 * 1024.0);
        return wavSize * 0.58;
    }

    double kbps = 192.0;
    if (bitrate.contains("320k")) kbps = 320.0;
    else if (bitrate.contains("256k")) kbps = 256.0;
    else if (bitrate.contains("192k")) kbps = 192.0;
    else if (bitrate.contains("128k")) kbps = 128.0;
    else if (bitrate.contains("64k")) kbps = 64.0;

    return (durationSec * (kbps * 1000.0 / 8.0)) / (1024.0 * 1024.0);
}

QString FFmpegHelper::formatSizeString(double sizeMB) {
    if (sizeMB <= 0.0) return "载入视频后自动推算";
    if (sizeMB < 1.0) {
        return QString("约 %1 KB").arg(static_cast<int>(sizeMB * 1024.0));
    }
    return QString("约 %1 MB").arg(sizeMB, 0, 'f', 1);
}

QString FFmpegHelper::findFFmpegBinary(const QString &customPath) {
    if (!customPath.isEmpty()) {
        QFileInfo fi(customPath);
        if (fi.exists() && fi.isExecutable()) {
            return fi.absoluteFilePath();
        }
    }

    QString appDir = QCoreApplication::applicationDirPath();
    QDir dir(appDir);

    // 1. 沿当前运行目录向上逐级追溯查找 tools/ffmpeg.exe 或 ffmpeg.exe (覆盖各种 IDE 嵌套输出目录)
    for (int i = 0; i <= 6; ++i) {
        QString checkTools = dir.filePath("tools/ffmpeg.exe");
        if (QFileInfo::exists(checkTools)) return QFileInfo(checkTools).absoluteFilePath();

        QString checkLocal = dir.filePath("ffmpeg.exe");
        if (QFileInfo::exists(checkLocal)) return QFileInfo(checkLocal).absoluteFilePath();

        if (!dir.cdUp()) break;
    }

    // 2. 检查预置工作区与发布包固定路径
    static const QStringList fallbackPaths = {
        "E:/AiToy/video/tools/ffmpeg.exe",
        "E:/AiToy/video/Release_Package/StarryAudioExtractor/tools/ffmpeg.exe",
        "E:/AiToy/video/build_release/tools/ffmpeg.exe",
        "E:/AiToy/video/ffmpeg.exe",
        "C:/Program Files/ffmpeg/bin/ffmpeg.exe",
        "C:/Program Files (x86)/ffmpeg/bin/ffmpeg.exe"
    };
    for (const QString &p : fallbackPaths) {
        if (QFileInfo::exists(p)) return p;
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
    if (!proc.waitForStarted(2000)) return false;
    if (!proc.waitForFinished(3000)) {
        proc.kill();
        return false;
    }

    if (proc.exitCode() == 0) {
        if (versionOut) {
            QString output = QString::fromUtf8(proc.readAllStandardOutput());
            if (output.isEmpty()) output = QString::fromUtf8(proc.readAllStandardError());
            *versionOut = output.split('\n').value(0).trimmed();
        }
        return true;
    }
    return false;
}

double FFmpegHelper::probeDurationSeconds(const QString &ffmpegPath, const QString &videoPath) {
    QString exec = findFFmpegBinary(ffmpegPath);
    if (exec.isEmpty() || !QFileInfo::exists(videoPath)) return 0.0;

    QProcess proc;
    proc.setProcessChannelMode(QProcess::MergedChannels);
    proc.start(exec, QStringList() << "-hide_banner" << "-i" << videoPath);
    proc.waitForFinished(2500);

    QString output = QString::fromUtf8(proc.readAll());
    static QRegularExpression durRegex(R"(Duration:\s*(\d{2}:\d{2}:\d{2}(?:\.\d+)?))");
    QRegularExpressionMatch match = durRegex.match(output);
    if (match.hasMatch()) {
        return parseTimeToSeconds(match.captured(1));
    }
    return 0.0;
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
        emit logMessage("⚠️ 任务正在运行中，请等待完成或取消当前任务", "WARNING");
        return;
    }

    m_currentOptions = options;
    m_totalDurationSec = 0.0;
    m_currentSec = 0.0;
    m_durationParsed = false;
    m_outputBuffer.clear();

    QString exec = findFFmpegBinary(ffmpegPath);
    if (exec.isEmpty()) {
        emit logMessage("❌ 未找到 FFmpeg 执行引擎 (tools/ffmpeg.exe)！", "ERROR");
        emit logMessage("💡 提示：请将 ffmpeg.exe 放置于 tools 目录下，即可开箱即用", "WARNING");
        emit extractionFinished(false, "", "未找到 FFmpeg");
        return;
    }

    QFileInfo inputFi(options.inputFilePath);
    if (!inputFi.exists()) {
        emit logMessage(QString("❌ 输入视频文件不存在: %1").arg(options.inputFilePath), "ERROR");
        emit extractionFinished(false, "", "文件不存在");
        return;
    }

    QFileInfo outFi(options.outputFilePath);
    QDir().mkpath(outFi.absolutePath());

    QStringList args;
    args << "-hide_banner";
    if (options.overwriteOutput) args << "-y";
    else args << "-n";

    args << "-i" << options.inputFilePath;
    args << "-vn";

    if (options.mode == ExtractMode::DirectCopy) {
        args << "-c:a" << "copy";
        emit logMessage("⚡ 启用极速无损流提取模式 (-c:a copy)", "INFO");
    } else {
        QString codec = selectAudioCodec(options.targetFormat);
        if (!codec.isEmpty()) args << "-c:a" << codec;

        if (options.bitrate != "Auto" && !options.bitrate.isEmpty()) {
            args << "-b:a" << options.bitrate;
        }
        if (options.sampleRate != "Auto" && !options.sampleRate.isEmpty()) {
            args << "-ar" << options.sampleRate;
        }
        if (options.channels == "2") args << "-ac" << "2";
        else if (options.channels == "1") args << "-ac" << "1";

        emit logMessage(QString("🎛️ 启用转码提取模式: 目标格式=[%1], 码率=[%2]")
                            .arg(options.targetFormat.toUpper())
                            .arg(options.bitrate), "INFO");
    }

    args << options.outputFilePath;

    emit logMessage(QString("🚀 已自动启动 FFmpeg 核心抽取引擎 (%1)").arg(QFileInfo(exec).fileName()), "INFO");
    emit started();
    emit progressUpdated(0, 0, 0);

    // 动态启动进程：开始提取时自动启动引擎
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
        emit logMessage("⏹️ 用户中断任务，正在自动关闭 FFmpeg 引擎...", "WARNING");
        m_process->kill();
        m_process->waitForFinished(1000);
        emit logMessage("⏹️ FFmpeg 引擎已安全关闭并退出。", "INFO");
    }
}

bool FFmpegHelper::isRunning() const {
    return (m_process && m_process->state() != QProcess::NotRunning);
}

void FFmpegHelper::onProcessReadyRead() {
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
        parseFFmpegOutput(line);
    }
}

void FFmpegHelper::parseFFmpegOutput(const QString &line) {
    QString trimmed = line.trimmed();
    if (trimmed.isEmpty()) return;

    if (!m_durationParsed && trimmed.contains("Duration:")) {
        static QRegularExpression durRegex(R"(Duration:\s*(\d{2}:\d{2}:\d{2}(?:\.\d+)?))");
        QRegularExpressionMatch match = durRegex.match(trimmed);
        if (match.hasMatch()) {
            QString timeStr = match.captured(1);
            m_totalDurationSec = parseTimeToSeconds(timeStr);
            m_durationParsed = true;
            emit logMessage(QString("⏱️ 检测到视频时长: %1").arg(timeStr), "INFO");
        }
    }

    if (trimmed.contains("time=")) {
        static QRegularExpression timeRegex(R"(time=\s*(\d{2}:\d{2}:\d{2}(?:\.\d+)?))");
        QRegularExpressionMatch match = timeRegex.match(trimmed);
        if (match.hasMatch()) {
            m_currentSec = parseTimeToSeconds(match.captured(1));
            int percent = 0;
            if (m_totalDurationSec > 0.0) {
                percent = qBound(0, static_cast<int>((m_currentSec / m_totalDurationSec) * 100.0), 99);
            }
            emit progressUpdated(percent, m_currentSec, m_totalDurationSec);
        }
    }
}

double FFmpegHelper::parseTimeToSeconds(const QString &timeStr) {
    QStringList parts = timeStr.split(':');
    if (parts.size() == 3) {
        return parts[0].toDouble() * 3600.0 + parts[1].toDouble() * 60.0 + parts[2].toDouble();
    }
    return 0.0;
}

void FFmpegHelper::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    bool success = (exitStatus == QProcess::NormalExit && exitCode == 0);

    if (success) {
        emit progressUpdated(100, m_totalDurationSec, m_totalDurationSec);
        emit logMessage(QString("🎉 音频提取成功！已保存至桌面: %1").arg(m_currentOptions.outputFilePath), "SUCCESS");
        emit logMessage("⏹️ FFmpeg 提取引擎任务完成，已自动退出关闭。", "INFO");
        emit extractionFinished(true, m_currentOptions.outputFilePath, "");
    } else {
        QString errMsg = QString("提取失败 (代码: %1)").arg(exitCode);
        emit logMessage(QString("❌ %1").arg(errMsg), "ERROR");
        emit logMessage("⏹️ FFmpeg 提取引擎已自动关闭。", "INFO");
        emit extractionFinished(false, m_currentOptions.outputFilePath, errMsg);
    }

    // 自动清理关闭进程
    if (m_process) {
        m_process->deleteLater();
        m_process = nullptr;
    }
}

void FFmpegHelper::onProcessError(QProcess::ProcessError error) {
    if (error == QProcess::FailedToStart) {
        emit logMessage("❌ 启动 FFmpeg 失败，请检查 tools/ffmpeg.exe 是否存在！", "ERROR");
        emit extractionFinished(false, "", "FFmpeg 启动失败");
    }
}
