#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QProgressBar>
#include <QTextEdit>
#include <QLabel>
#include <QScrollArea>
#include <QElapsedTimer>
#include "starrybackground.h"
#include "ffmpeghelper.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void onBrowseInputVideo();
    void onBrowseOutputDir();
    void onBrowseFFmpeg();
    void onModeChanged();
    void onFormatChanged(int index);
    void onStartOrCancelClicked();
    void onOpenOutputFolder();
    void onPlayAudio();
    void onClearLog();

    // FFmpeg Helper 信号槽
    void onFFmpegStarted();
    void onFFmpegLog(const QString &message, const QString &type);
    void onFFmpegProgress(int percentage, double currentSec, double totalDurationSec);
    void onFFmpegFinished(bool success, const QString &outputFilePath, const QString &errorMsg);

private:
    void setupUi();
    void checkAndDetectFFmpeg();
    void updateDefaultOutputPath();
    void setExtractionUiState(bool extracting);
    void appendLog(const QString &message, const QString &type);
    QString formatSeconds(double totalSeconds);

    // 控件成员
    StarryBackground *m_starryBg;
    QScrollArea *m_scrollArea;
    QWidget *m_contentContainer;

    // 输入区
    QLineEdit *m_inputVideoEdit;
    QPushButton *m_browseInputBtn;
    QLabel *m_fileInfoLabel;

    // 提取配置区
    QRadioButton *m_radioDirectCopy;
    QRadioButton *m_radioTranscode;
    QComboBox *m_formatCombo;
    QComboBox *m_bitrateCombo;
    QComboBox *m_sampleRateCombo;
    QComboBox *m_channelCombo;
    QCheckBox *m_overwriteCheck;

    // 输出区
    QLineEdit *m_outputPathEdit;
    QPushButton *m_browseOutputBtn;

    // FFmpeg 状态区
    QLineEdit *m_ffmpegPathEdit;
    QPushButton *m_browseFFmpegBtn;
    QLabel *m_ffmpegStatusBadge;

    // 控制与进度区
    QPushButton *m_startBtn;
    QPushButton *m_openFolderBtn;
    QPushButton *m_playAudioBtn;
    QProgressBar *m_progressBar;
    QLabel *m_progressDetailLabel;

    // 日志控制台
    QTextEdit *m_consoleLog;
    QPushButton *m_clearLogBtn;

    // 状态管理
    FFmpegHelper *m_ffmpegHelper;
    QElapsedTimer m_elapsedTimer;
    QString m_lastGeneratedAudioPath;
};

#endif // MAINWINDOW_H
