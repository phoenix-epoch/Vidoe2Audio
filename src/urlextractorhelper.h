#ifndef URLEXTRACTORHELPER_H
#define URLEXTRACTORHELPER_H

#include <QObject>
#include <QString>
#include <QProcess>

struct UrlExtractionOptions {
    QString url;
    QString outputDirectory;
    QString targetFormat;    // 如 "mp3", "m4a", "wav", "flac"
    QString audioQuality;    // "0" (最佳 320k), "2" (高质量 256k), "5" (标准 192k), "8" (轻量 128k)
    bool singleVideoOnly;    // 是否强制仅单视频 (--no-playlist)
};

class UrlExtractorHelper : public QObject {
    Q_OBJECT

public:
    explicit UrlExtractorHelper(QObject *parent = nullptr);
    ~UrlExtractorHelper() override;

    static QString findYtDlpBinary();
    void startExtraction(const QString &ffmpegPath, const UrlExtractionOptions &options);
    void cancel();
    bool isRunning() const;

signals:
    void started();
    void logMessage(const QString &message, const QString &type);
    void progressUpdated(int percentage, const QString &speedStr, const QString &etaStr);
    void extractionFinished(bool success, const QString &outputFolder, const QString &lastExtractedFile, const QString &errorMsg);

private slots:
    void onProcessReadyRead();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);

private:
    void parseYtDlpLine(const QString &line);

    QProcess *m_process;
    UrlExtractionOptions m_currentOptions;
    QString m_outputBuffer;
    QString m_lastOutputFile;
};

#endif // URLEXTRACTORHELPER_H
