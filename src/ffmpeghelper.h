#ifndef FFMPEGHELPER_H
#define FFMPEGHELPER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>

enum class ExtractMode {
    DirectCopy,     // 极速无损流拷贝 (-c:a copy)
    Transcode       // 格式重编码定制
};

struct ExtractionOptions {
    QString inputFilePath;
    QString outputFilePath;
    ExtractMode mode;
    QString targetFormat;    // 如 "MP3", "AAC", "WAV", "FLAC" 等
    QString bitrate;         // 如 "320k", "256k", "192k", "128k", "Auto"
    QString sampleRate;      // 如 "48000", "44100", "Auto"
    QString channels;        // 如 "Auto", "2", "1"
    bool overwriteOutput;    // 是否覆盖已有文件
};

// 后缀诊断信息
struct ExtensionDiagnosis {
    bool isSupported;
    bool isDirectCopyable;
    QString formatName;
    QString containerType;
    QString recommendMode;
    QString advice;
};

class FFmpegHelper : public QObject {
    Q_OBJECT

public:
    explicit FFmpegHelper(QObject *parent = nullptr);
    ~FFmpegHelper() override;

    // 格式支持清单与诊断接口
    static QStringList supportedVideoExtensions();
    static QStringList supportedAudioFormats();
    static QString getAudioExtensionForFormat(const QString &format);
    static QString getFilterStringForVideoFiles();
    static ExtensionDiagnosis diagnoseExtension(const QString &rawInput);

    // 大小预估
    static double estimateAudioSizeMB(double durationSec, const QString &format, const QString &bitrate);
    static QString formatSizeString(double sizeMB);

    // FFmpeg 智能寻找与检测
    static QString findFFmpegBinary(const QString &customPath = "");
    static bool checkFFmpegExecutable(const QString &ffmpegPath, QString *versionOut = nullptr);
    static double probeDurationSeconds(const QString &ffmpegPath, const QString &videoPath);

    // 核心提取操作
    void startExtraction(const QString &ffmpegPath, const ExtractionOptions &options);
    void cancelExtraction();
    bool isRunning() const;

signals:
    void started();
    void logMessage(const QString &message, const QString &type); // type: "INFO", "SUCCESS", "WARNING", "ERROR", "FFMPEG"
    void progressUpdated(int percentage, double currentSec, double totalDurationSec);
    void extractionFinished(bool success, const QString &outputFilePath, const QString &errorMsg);

private slots:
    void onProcessReadyRead();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);

private:
    void parseFFmpegOutput(const QString &text);
    static double parseTimeToSeconds(const QString &timeStr);
    static QString selectAudioCodec(const QString &format);

    QProcess *m_process;
    ExtractionOptions m_currentOptions;
    double m_totalDurationSec;
    double m_currentSec;
    bool m_durationParsed;
    QString m_outputBuffer;
};

#endif // FFMPEGHELPER_H
