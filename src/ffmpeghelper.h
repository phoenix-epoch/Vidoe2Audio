#ifndef FFMPEGHELPER_H
#define FFMPEGHELPER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>

enum class ExtractMode {
    DirectCopy,     // 极速无损流拷贝 (-c:a copy)
    Transcode       // 格式转码定制
};

struct ExtractionOptions {
    QString inputFilePath;
    QString outputFilePath;
    ExtractMode mode;
    QString targetFormat;    // 如 "mp3", "aac", "wav", "flac" 等
    QString bitrate;         // 如 "320k", "256k", "192k", "128k", "Auto"
    QString sampleRate;      // 如 "48000", "44100", "Auto"
    QString channels;        // 如 "Auto", "2" (立体声), "1" (单声道)
    bool overwriteOutput;    // 是否覆盖已有文件
};

class FFmpegHelper : public QObject {
    Q_OBJECT

public:
    explicit FFmpegHelper(QObject *parent = nullptr);
    ~FFmpegHelper() override;

    // 格式支持清单
    static QStringList supportedVideoExtensions();
    static QStringList supportedAudioFormats();
    static QString getAudioExtensionForFormat(const QString &format);
    static QString getFilterStringForVideoFiles();

    // FFmpeg 探测与校验
    static QString findFFmpegBinary(const QString &customPath = "");
    static bool checkFFmpegExecutable(const QString &ffmpegPath, QString *versionOut = nullptr);

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
    double parseTimeToSeconds(const QString &timeStr);
    QString selectAudioCodec(const QString &format);

    QProcess *m_process;
    ExtractionOptions m_currentOptions;
    double m_totalDurationSec;
    double m_currentSec;
    bool m_durationParsed;
    QString m_outputBuffer;
};

#endif // FFMPEGHELPER_H
