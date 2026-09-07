#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include "ffmpeghelper.h"
#include "urlextractorhelper.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class StarryTitleBar;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void changeEvent(QEvent *event) override;
#if defined(Q_OS_WIN)
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
#endif

private slots:
    // 导航槽函数
    void onEnterLocalMode();
    void onEnterUrlMode();
    void onBackToPortal();

    // 本地工作台槽函数
    void onBrowseInputVideo();
    void onBrowseOutputDir();
    void onShowFormatsDialog();
    void onInputTextChanged(const QString &text);
    void onLocalModeToggled();
    void onLocalFormatOrBitrateChanged();
    void onStartOrCancelLocalClicked();
    void onOpenLocalOutputFolder();
    void onPlayLocalAudio();
    void onClearLocalLog();

    // 本地 FFmpeg 信号
    void onLocalFFmpegStarted();
    void onLocalFFmpegLog(const QString &message, const QString &type);
    void onLocalFFmpegProgress(int percentage, double currentSec, double totalDurationSec);
    void onLocalFFmpegFinished(bool success, const QString &outputFilePath, const QString &errorMsg);

    // 网络工作台槽函数
    void onUrlTargetModeChanged();
    void onPasteUrl();
    void onBrowseUrlOutputDir();
    void onStartOrCancelUrlClicked();
    void onOpenUrlOutputFolder();
    void onPlayUrlAudio();
    void onClearUrlLog();

    // 网络 yt-dlp 信号
    void onUrlYtDlpStarted();
    void onUrlYtDlpLog(const QString &message, const QString &type);
    void onUrlYtDlpProgress(int percentage, const QString &speedStr, const QString &etaStr);
    void onUrlYtDlpFinished(bool success, const QString &outputFolder, const QString &lastExtractedFile, const QString &errorMsg);

private:
    void setupUiCustom();
    void updateDefaultLocalOutputPath();
    void updateLocalEstimatedSize();
    void setLocalExtractionUiState(bool extracting);
    void setUrlExtractionUiState(bool extracting);
    void appendLocalLog(const QString &message, const QString &type);
    void appendUrlLog(const QString &message, const QString &type);
    QString formatSeconds(double totalSeconds);
    QString getDesktopPath() const;

    Ui::MainWindow *ui;
    StarryTitleBar *m_titleBar;

    // 本地引擎
    FFmpegHelper *m_ffmpegHelper;
    QElapsedTimer m_localTimer;
    QString m_lastLocalAudioPath;
    double m_currentVideoDurationSec;

    // 网络引擎
    UrlExtractorHelper *m_urlHelper;
    QElapsedTimer m_urlTimer;
    QString m_lastUrlAudioPath;
};

#endif // MAINWINDOW_H
