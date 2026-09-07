#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDateTime>
#include <QScrollBar>
#include <QStandardPaths>
#include <QGuiApplication>
#include <QClipboard>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_ffmpegHelper(new FFmpegHelper(this)),
      m_currentVideoDurationSec(0.0),
      m_urlHelper(new UrlExtractorHelper(this))
{
    ui->setupUi(this);
    setAcceptDrops(true);

    setupUiCustom();

    // 默认展示首页导航大厅
    ui->stackedWidget->setCurrentIndex(0);

    appendLocalLog("🌟 本地音视频分离引擎就绪 (默认输出至桌面)", "INFO");
    appendUrlLog("🌟 网络音视频提取引擎就绪 (基于 yt-dlp + FFmpeg)", "INFO");
}

MainWindow::~MainWindow() {
    delete ui;
}

QString MainWindow::getDesktopPath() const {
    QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    if (desktop.isEmpty()) {
        desktop = QDir::homePath();
    }
    return QDir::toNativeSeparators(desktop);
}

void MainWindow::setupUiCustom() {
    // 1. 导航按钮绑定
    connect(ui->modeCardLocal, &QPushButton::clicked, this, &MainWindow::onEnterLocalMode);
    connect(ui->modeCardUrl, &QPushButton::clicked, this, &MainWindow::onEnterUrlMode);
    connect(ui->backFromLocalBtn, &QPushButton::clicked, this, &MainWindow::onBackToPortal);
    connect(ui->backFromUrlBtn, &QPushButton::clicked, this, &MainWindow::onBackToPortal);

    // 2. 本地工作台组件初始化
    ui->formatCombo->addItems(FFmpegHelper::supportedAudioFormats());
    ui->formatCombo->setCurrentText("MP3");

    ui->bitrateCombo->addItems({"Auto (自适应)", "320k (超清音质)", "256k (高品质)", "192k (标准品质)", "128k (流畅)", "64k (低码率)"});
    ui->bitrateCombo->setCurrentIndex(1); // 默认 320k

    ui->sampleRateCombo->addItems({"Auto (源采样率)", "48000 Hz", "44100 Hz", "32000 Hz", "24000 Hz", "16000 Hz"});
    ui->sampleRateCombo->setCurrentIndex(0);

    ui->channelCombo->addItems({"Auto (保持源声道)", "立体声 (2 Channels)", "单声道 (Mono)"});
    ui->channelCombo->setCurrentIndex(0);

    // 默认本地输出路径指向桌面
    ui->outputPathEdit->setText(getDesktopPath());

    connect(ui->browseInputBtn, &QPushButton::clicked, this, &MainWindow::onBrowseInputVideo);
    connect(ui->browseOutputBtn, &QPushButton::clicked, this, &MainWindow::onBrowseOutputDir);
    connect(ui->showFormatsBtn, &QPushButton::clicked, this, &MainWindow::onShowFormatsDialog);
    connect(ui->startBtn, &QPushButton::clicked, this, &MainWindow::onStartOrCancelLocalClicked);
    connect(ui->openFolderBtn, &QPushButton::clicked, this, &MainWindow::onOpenLocalOutputFolder);
    connect(ui->playAudioBtn, &QPushButton::clicked, this, &MainWindow::onPlayLocalAudio);
    connect(ui->clearLogBtn, &QPushButton::clicked, this, &MainWindow::onClearLocalLog);

    connect(ui->radioDirectCopy, &QRadioButton::toggled, this, &MainWindow::onLocalModeToggled);
    connect(ui->radioTranscode, &QRadioButton::toggled, this, &MainWindow::onLocalModeToggled);
    connect(ui->formatCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onLocalFormatOrBitrateChanged);
    connect(ui->bitrateCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onLocalFormatOrBitrateChanged);
    connect(ui->inputVideoEdit, &QLineEdit::textChanged, this, &MainWindow::onInputTextChanged);

    // 本地 FFmpeg 信号连接
    connect(m_ffmpegHelper, &FFmpegHelper::started, this, &MainWindow::onLocalFFmpegStarted);
    connect(m_ffmpegHelper, &FFmpegHelper::logMessage, this, &MainWindow::onLocalFFmpegLog);
    connect(m_ffmpegHelper, &FFmpegHelper::progressUpdated, this, &MainWindow::onLocalFFmpegProgress);
    connect(m_ffmpegHelper, &FFmpegHelper::extractionFinished, this, &MainWindow::onLocalFFmpegFinished);

    // 3. 网络工作台组件初始化
    ui->urlFormatCombo->addItems({"MP3", "M4A", "WAV", "FLAC"});
    ui->urlFormatCombo->setCurrentText("MP3");

    ui->urlQualityCombo->addItems({"最佳音质 (320k 推荐)", "高质量 (256k)", "标准音质 (192k)", "轻量极速 (128k)"});
    ui->urlQualityCombo->setCurrentIndex(0);

    ui->urlOutputPathEdit->setText(getDesktopPath());

    connect(ui->pasteUrlBtn, &QPushButton::clicked, this, &MainWindow::onPasteUrl);
    connect(ui->browseUrlOutputBtn, &QPushButton::clicked, this, &MainWindow::onBrowseUrlOutputDir);
    connect(ui->startUrlBtn, &QPushButton::clicked, this, &MainWindow::onStartOrCancelUrlClicked);
    connect(ui->openUrlFolderBtn, &QPushButton::clicked, this, &MainWindow::onOpenUrlOutputFolder);
    connect(ui->playUrlAudioBtn, &QPushButton::clicked, this, &MainWindow::onPlayUrlAudio);
    connect(ui->clearUrlLogBtn, &QPushButton::clicked, this, &MainWindow::onClearUrlLog);

    // 网络 yt-dlp 信号连接
    connect(m_urlHelper, &UrlExtractorHelper::started, this, &MainWindow::onUrlYtDlpStarted);
    connect(m_urlHelper, &UrlExtractorHelper::logMessage, this, &MainWindow::onUrlYtDlpLog);
    connect(m_urlHelper, &UrlExtractorHelper::progressUpdated, this, &MainWindow::onUrlYtDlpProgress);
    connect(m_urlHelper, &UrlExtractorHelper::extractionFinished, this, &MainWindow::onUrlYtDlpFinished);
}

// ==========================================
// 页面导航控制
// ==========================================
void MainWindow::onEnterLocalMode() {
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::onEnterUrlMode() {
    ui->stackedWidget->setCurrentIndex(2);

    // 自动探测剪贴板是否已有网址
    QString clipText = QGuiApplication::clipboard()->text().trimmed();
    if (clipText.startsWith("http://") || clipText.startsWith("https://")) {
        if (ui->urlEdit->text().isEmpty()) {
            ui->urlEdit->setText(clipText);
            appendUrlLog(QString("📋 已自动从剪贴板识别到视频网址: %1").arg(clipText), "INFO");
        }
    }
}

void MainWindow::onBackToPortal() {
    ui->stackedWidget->setCurrentIndex(0);
}

// ==========================================
// 本地提取模式交互
// ==========================================
void MainWindow::onInputTextChanged(const QString &text) {
    QString raw = text.trimmed();
    if (raw.isEmpty()) {
        ui->fileInfoLabel->setText("文件状态: 未载入视频 (可直接输入后缀名如 mkv 快速诊断)");
        ui->fileInfoLabel->setStyleSheet("color: #64748b; font-size: 11px;");
        m_currentVideoDurationSec = 0.0;
        updateLocalEstimatedSize();
        return;
    }

    // 1. 如果是物理存在的文件
    if (QFileInfo::exists(raw) && QFileInfo(raw).isFile()) {
        QFileInfo fi(raw);
        double sizeMB = fi.size() / (1024.0 * 1024.0);

        // 探测时长
        m_currentVideoDurationSec = FFmpegHelper::probeDurationSeconds("", raw);
        QString durStr = (m_currentVideoDurationSec > 0.0)
                             ? QString(" | 时长: %1").arg(formatSeconds(m_currentVideoDurationSec))
                             : "";

        ui->fileInfoLabel->setText(QString("✅ 已加载文件: %1 (大小: %2 MB | 格式: %3%4)")
                                       .arg(fi.fileName())
                                       .arg(sizeMB, 0, 'f', 2)
                                       .arg(fi.suffix().toUpper())
                                       .arg(durStr));
        ui->fileInfoLabel->setStyleSheet("color: #00ff88; font-size: 11px;");

        updateDefaultLocalOutputPath();
        updateLocalEstimatedSize();
        return;
    }

    // 2. 如果用户输入的是纯后缀名 (例如 "mkv", ".rmvb", "mp4")
    QString cleanExt = raw;
    if (cleanExt.startsWith(".")) cleanExt = cleanExt.mid(1);

    if (!cleanExt.contains("/") && !cleanExt.contains("\\") && cleanExt.length() <= 8) {
        ExtensionDiagnosis diag = FFmpegHelper::diagnoseExtension(cleanExt);
        if (diag.isSupported) {
            ui->fileInfoLabel->setText(QString("✅ [格式诊断: .%1] %2 | %3 (推荐模式: %4)")
                                           .arg(cleanExt.toUpper(), diag.formatName, diag.containerType, diag.recommendMode));
            ui->fileInfoLabel->setStyleSheet("color: #00f0ff; font-weight: bold; font-size: 11px;");
        } else {
            ui->fileInfoLabel->setText(QString("⚠️ [格式诊断: .%1] %2 | %3")
                                           .arg(cleanExt.toUpper(), diag.formatName, diag.advice));
            ui->fileInfoLabel->setStyleSheet("color: #ffaa00; font-size: 11px;");
        }
    } else {
        ui->fileInfoLabel->setText("❌ 文件路径不存在，请检查或通过右侧按钮浏览选择");
        ui->fileInfoLabel->setStyleSheet("color: #ff3366; font-size: 11px;");
    }

    m_currentVideoDurationSec = 0.0;
    updateLocalEstimatedSize();
}

void MainWindow::updateDefaultLocalOutputPath() {
    QString inputPath = ui->inputVideoEdit->text().trimmed();
    if (inputPath.isEmpty() || !QFileInfo::exists(inputPath)) return;

    QFileInfo fi(inputPath);
    QString baseName = fi.completeBaseName();

    QString ext;
    if (ui->radioDirectCopy->isChecked()) {
        QString vExt = fi.suffix().toLower();
        if (vExt == "mp4" || vExt == "m4v" || vExt == "mov") ext = "m4a";
        else if (vExt == "webm" || vExt == "mkv") ext = "mka";
        else ext = "aac";
    } else {
        ext = FFmpegHelper::getAudioExtensionForFormat(ui->formatCombo->currentText());
    }

    // 默认一律保存到【电脑桌面】
    QString desktop = getDesktopPath();
    QString defaultOut = QDir(desktop).filePath(QString("%1_extracted.%2").arg(baseName, ext));
    ui->outputPathEdit->setText(defaultOut);
}

void MainWindow::updateLocalEstimatedSize() {
    if (m_currentVideoDurationSec <= 0.0) {
        ui->estimatedSizeLabel->setText("💾 预估音频大小: 载入视频后自动推算");
        return;
    }

    double sizeMB = 0.0;
    if (ui->radioDirectCopy->isChecked()) {
        // 无损流拷贝预估 (一般 128~192k 算)
        sizeMB = FFmpegHelper::estimateAudioSizeMB(m_currentVideoDurationSec, "AAC", "192k");
        ui->estimatedSizeLabel->setText(QString("💾 预估音频大小: %1 (无损原声拷贝)").arg(FFmpegHelper::formatSizeString(sizeMB)));
    } else {
        QString fmt = ui->formatCombo->currentText();
        QString br = ui->bitrateCombo->currentText();
        sizeMB = FFmpegHelper::estimateAudioSizeMB(m_currentVideoDurationSec, fmt, br);
        ui->estimatedSizeLabel->setText(QString("💾 预估音频大小: %1 (%2 / %3)")
                                            .arg(FFmpegHelper::formatSizeString(sizeMB), fmt, br));
    }
}

void MainWindow::onLocalModeToggled() {
    bool transcode = ui->radioTranscode->isChecked();
    ui->formatCombo->setEnabled(transcode);
    ui->bitrateCombo->setEnabled(transcode);
    ui->sampleRateCombo->setEnabled(transcode);
    ui->channelCombo->setEnabled(transcode);

    updateDefaultLocalOutputPath();
    updateLocalEstimatedSize();
}

void MainWindow::onLocalFormatOrBitrateChanged() {
    updateDefaultLocalOutputPath();
    updateLocalEstimatedSize();
}

void MainWindow::onBrowseInputVideo() {
    QString filter = FFmpegHelper::getFilterStringForVideoFiles();
    QString path = QFileDialog::getOpenFileName(this, "选择需要提取音频的源音视频文件", "", filter);
    if (!path.isEmpty()) {
        ui->inputVideoEdit->setText(path);
        appendLocalLog(QString("📂 已载入本地视频: %1").arg(path), "INFO");
    }
}

void MainWindow::onBrowseOutputDir() {
    QString currentOut = ui->outputPathEdit->text().trimmed();
    QString selected = QFileDialog::getSaveFileName(this, "指定提取后的音频保存文件", currentOut, "音频文件 (*.*)");
    if (!selected.isEmpty()) {
        ui->outputPathEdit->setText(selected);
        appendLocalLog(QString("💾 已更新保存路径: %1").arg(selected), "INFO");
    }
}

void MainWindow::onShowFormatsDialog() {
    QString msg =
        "<h3>🌌 支持提取的音视频格式清单 (FFmpeg 驱动)</h3>"
        "<p>本软件支持市面上 <b>99%</b> 的多媒体格式：</p>"
        "<ul>"
        "<li><b>主流通用格式：</b>MP4, MKV, MOV (苹果), AVI, WebM</li>"
        "<li><b>流媒体与网络录制：</b>FLV, TS, M4V, F4V, OGV</li>"
        "<li><b>高清与摄像机原盘：</b>MTS, M2TS (蓝光), VOB (DVD), WMV</li>"
        "<li><b>经典与老旧格式：</b>RMVB, RM, 3GP, MPG, MPEG, ASF, DIVX</li>"
        "<li><b>冷门专用封装：</b>DAV, MXF, BIK, NUT 等</li>"
        "</ul>"
        "<p>💡 <i>提示：您也可以直接在主界面输入框内输入后缀（如 <code>rmvb</code>、<code>mkv</code>）回车，系统将即刻提供专属诊断建议！</i></p>";

    QMessageBox::information(this, "支持格式清单", msg);
}

void MainWindow::onStartOrCancelLocalClicked() {
    if (m_ffmpegHelper->isRunning()) {
        m_ffmpegHelper->cancelExtraction();
        return;
    }

    QString inputPath = ui->inputVideoEdit->text().trimmed();
    QString outputPath = ui->outputPathEdit->text().trimmed();

    if (inputPath.isEmpty() || !QFileInfo::exists(inputPath)) {
        appendLocalLog("❌ 请先输入或选择有效的源视频文件！", "ERROR");
        return;
    }

    if (outputPath.isEmpty()) {
        outputPath = QDir(getDesktopPath()).filePath("extracted_audio.mp3");
        ui->outputPathEdit->setText(outputPath);
    }

    ExtractionOptions options;
    options.inputFilePath = inputPath;
    options.outputFilePath = outputPath;
    options.overwriteOutput = ui->overwriteCheck->isChecked();
    options.mode = ui->radioDirectCopy->isChecked() ? ExtractMode::DirectCopy : ExtractMode::Transcode;
    options.targetFormat = ui->formatCombo->currentText();

    QString br = ui->bitrateCombo->currentText();
    if (br.contains("320k")) options.bitrate = "320k";
    else if (br.contains("256k")) options.bitrate = "256k";
    else if (br.contains("192k")) options.bitrate = "192k";
    else if (br.contains("128k")) options.bitrate = "128k";
    else if (br.contains("64k")) options.bitrate = "64k";
    else options.bitrate = "Auto";

    QString sr = ui->sampleRateCombo->currentText();
    if (sr.contains("48000")) options.sampleRate = "48000";
    else if (sr.contains("44100")) options.sampleRate = "44100";
    else if (sr.contains("32000")) options.sampleRate = "32000";
    else if (sr.contains("24000")) options.sampleRate = "24000";
    else if (sr.contains("16000")) options.sampleRate = "16000";
    else options.sampleRate = "Auto";

    int chIdx = ui->channelCombo->currentIndex();
    if (chIdx == 1) options.channels = "2";
    else if (chIdx == 2) options.channels = "1";
    else options.channels = "Auto";

    m_localTimer.restart();
    m_ffmpegHelper->startExtraction("", options);
}

void MainWindow::onLocalFFmpegStarted() {
    setLocalExtractionUiState(true);
    ui->progressBar->setValue(0);
    ui->progressDetailLabel->setText("🚀 正在调用内置 FFmpeg 引擎抽取音频...");
}

void MainWindow::onLocalFFmpegLog(const QString &message, const QString &type) {
    appendLocalLog(message, type);
}

void MainWindow::onLocalFFmpegProgress(int percentage, double currentSec, double totalDurationSec) {
    ui->progressBar->setValue(percentage);

    qint64 elapsedMs = m_localTimer.elapsed();
    QString elapsedStr = formatSeconds(elapsedMs / 1000.0);

    if (totalDurationSec > 0.0) {
        ui->progressDetailLabel->setText(QString("⚡ 进度: %1% (%2 / %3) | 已耗时: %4")
                                             .arg(percentage)
                                             .arg(formatSeconds(currentSec))
                                             .arg(formatSeconds(totalDurationSec))
                                             .arg(elapsedStr));
    } else {
        ui->progressDetailLabel->setText(QString("⚡ 提取中: %1 | 耗时: %2")
                                             .arg(formatSeconds(currentSec), elapsedStr));
    }
}

void MainWindow::onLocalFFmpegFinished(bool success, const QString &outputFilePath, const QString &errorMsg) {
    Q_UNUSED(errorMsg);
    setLocalExtractionUiState(false);

    if (success) {
        m_lastLocalAudioPath = outputFilePath;
        ui->openFolderBtn->setEnabled(true);
        ui->playAudioBtn->setEnabled(true);
        ui->progressDetailLabel->setText("🎉 音频分离提取完成！(100% 已保存至桌面)");
    } else {
        ui->progressDetailLabel->setText("❌ 提取中止或发生错误");
    }
}

void MainWindow::setLocalExtractionUiState(bool extracting) {
    if (extracting) {
        ui->startBtn->setText("⏹️ 终止提取任务");
        ui->startBtn->setObjectName("cancelActionBtn");
        ui->startBtn->setStyle(ui->startBtn->style());
        ui->browseInputBtn->setEnabled(false);
        ui->browseOutputBtn->setEnabled(false);
        ui->openFolderBtn->setEnabled(false);
        ui->playAudioBtn->setEnabled(false);
    } else {
        ui->startBtn->setText("🚀 开始分离提取音频");
        ui->startBtn->setObjectName("primaryActionBtn");
        ui->startBtn->setStyle(ui->startBtn->style());
        ui->browseInputBtn->setEnabled(true);
        ui->browseOutputBtn->setEnabled(true);
    }
}

void MainWindow::onOpenLocalOutputFolder() {
    QString path = m_lastLocalAudioPath.isEmpty() ? ui->outputPathEdit->text().trimmed() : m_lastLocalAudioPath;
    if (!path.isEmpty()) {
        QString folder = QFileInfo(path).absolutePath();
        QDesktopServices::openUrl(QUrl::fromLocalFile(folder));
    }
}

void MainWindow::onPlayLocalAudio() {
    if (!m_lastLocalAudioPath.isEmpty() && QFileInfo::exists(m_lastLocalAudioPath)) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(m_lastLocalAudioPath));
    }
}

void MainWindow::onClearLocalLog() {
    ui->consoleLog->clear();
    appendLocalLog("📟 控制台日志已清空", "INFO");
}

void MainWindow::appendLocalLog(const QString &message, const QString &type) {
    QString timeStr = QDateTime::currentDateTime().toString("HH:mm:ss");
    QString tagColor = "#00f0ff";
    QString tagText = "INFO";

    if (type == "SUCCESS") { tagColor = "#00ff88"; tagText = "SUCCESS"; }
    else if (type == "WARNING") { tagColor = "#ffaa00"; tagText = "WARN"; }
    else if (type == "ERROR") { tagColor = "#ff3366"; tagText = "ERROR"; }

    QString html = QString("<span style='color: #64748b;'>[%1]</span> "
                           "<span style='color: %2; font-weight: bold;'>[%3]</span> "
                           "<span style='color: #cbd5e1;'>%4</span>")
                       .arg(timeStr, tagColor, tagText, message.toHtmlEscaped());

    ui->consoleLog->append(html);
    ui->consoleLog->verticalScrollBar()->setValue(ui->consoleLog->verticalScrollBar()->maximum());
}

// ==========================================
// 网络网址提取模式交互
// ==========================================
void MainWindow::onPasteUrl() {
    QString text = QGuiApplication::clipboard()->text().trimmed();
    if (!text.isEmpty()) {
        ui->urlEdit->setText(text);
        appendUrlLog(QString("📋 已从剪贴板粘贴网址: %1").arg(text), "INFO");
    } else {
        appendUrlLog("⚠️ 剪贴板当前为空", "WARNING");
    }
}

void MainWindow::onBrowseUrlOutputDir() {
    QString current = ui->urlOutputPathEdit->text().trimmed();
    QString folder = QFileDialog::getExistingDirectory(this, "选择网络音频保存目录", current);
    if (!folder.isEmpty()) {
        ui->urlOutputPathEdit->setText(folder);
        appendUrlLog(QString("💾 已更新网络保存目录: %1").arg(folder), "INFO");
    }
}

void MainWindow::onStartOrCancelUrlClicked() {
    if (m_urlHelper->isRunning()) {
        m_urlHelper->cancel();
        return;
    }

    QString url = ui->urlEdit->text().trimmed();
    if (url.isEmpty()) {
        appendUrlLog("❌ 请先粘贴或输入视频网页链接！", "ERROR");
        return;
    }

    QString outDir = ui->urlOutputPathEdit->text().trimmed();
    if (outDir.isEmpty()) {
        outDir = getDesktopPath();
        ui->urlOutputPathEdit->setText(outDir);
    }

    UrlExtractionOptions opt;
    opt.url = url;
    opt.outputDirectory = outDir;
    opt.targetFormat = ui->urlFormatCombo->currentText();
    opt.singleVideoOnly = ui->singleVideoCheck->isChecked();

    int qIdx = ui->urlQualityCombo->currentIndex();
    if (qIdx == 0) opt.audioQuality = "0"; // 最佳 320k
    else if (qIdx == 1) opt.audioQuality = "2"; // 256k
    else if (qIdx == 2) opt.audioQuality = "5"; // 192k
    else opt.audioQuality = "8"; // 128k

    m_urlTimer.restart();
    m_urlHelper->startExtraction(FFmpegHelper::findFFmpegBinary(), opt);
}

void MainWindow::onUrlYtDlpStarted() {
    setUrlExtractionUiState(true);
    ui->urlProgressBar->setValue(0);
    ui->urlProgressDetailLabel->setText("🚀 正在分析网络网页多媒体流...");
}

void MainWindow::onUrlYtDlpLog(const QString &message, const QString &type) {
    appendUrlLog(message, type);
}

void MainWindow::onUrlYtDlpProgress(int percentage, const QString &speedStr, const QString &etaStr) {
    ui->urlProgressBar->setValue(percentage);
    ui->urlProgressDetailLabel->setText(QString("⚡ 下载与转码进度: %1% | 速度: %2 | 剩余: %3")
                                            .arg(percentage)
                                            .arg(speedStr, etaStr));
}

void MainWindow::onUrlYtDlpFinished(bool success, const QString &outputFolder, const QString &lastExtractedFile, const QString &errorMsg) {
    Q_UNUSED(errorMsg);
    setUrlExtractionUiState(false);

    if (success) {
        m_lastUrlAudioPath = lastExtractedFile;
        ui->openUrlFolderBtn->setEnabled(true);
        if (!lastExtractedFile.isEmpty() && QFileInfo::exists(lastExtractedFile)) {
            ui->playUrlAudioBtn->setEnabled(true);
        }
        ui->urlProgressDetailLabel->setText("🎉 网络视频音频提取完成！(已保存至桌面)");
    } else {
        ui->urlProgressDetailLabel->setText("❌ 网络提取中止或发生错误");
    }
}

void MainWindow::setUrlExtractionUiState(bool extracting) {
    if (extracting) {
        ui->startUrlBtn->setText("⏹️ 终止提取任务");
        ui->startUrlBtn->setObjectName("cancelActionBtn");
        ui->startUrlBtn->setStyle(ui->startUrlBtn->style());
        ui->pasteUrlBtn->setEnabled(false);
        ui->browseUrlOutputBtn->setEnabled(false);
        ui->openUrlFolderBtn->setEnabled(false);
        ui->playUrlAudioBtn->setEnabled(false);
    } else {
        ui->startUrlBtn->setText("🚀 开始解析并提取网络音频");
        ui->startUrlBtn->setObjectName("primaryActionBtn");
        ui->startUrlBtn->setStyle(ui->startUrlBtn->style());
        ui->pasteUrlBtn->setEnabled(true);
        ui->browseUrlOutputBtn->setEnabled(true);
    }
}

void MainWindow::onOpenUrlOutputFolder() {
    QString dir = ui->urlOutputPathEdit->text().trimmed();
    if (!dir.isEmpty()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(dir));
    }
}

void MainWindow::onPlayUrlAudio() {
    if (!m_lastUrlAudioPath.isEmpty() && QFileInfo::exists(m_lastUrlAudioPath)) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(m_lastUrlAudioPath));
    }
}

void MainWindow::onClearUrlLog() {
    ui->urlConsoleLog->clear();
    appendUrlLog("📟 控制台日志已清空", "INFO");
}

void MainWindow::appendUrlLog(const QString &message, const QString &type) {
    QString timeStr = QDateTime::currentDateTime().toString("HH:mm:ss");
    QString tagColor = "#00f0ff";
    QString tagText = "INFO";

    if (type == "SUCCESS") { tagColor = "#00ff88"; tagText = "SUCCESS"; }
    else if (type == "WARNING") { tagColor = "#ffaa00"; tagText = "WARN"; }
    else if (type == "ERROR") { tagColor = "#ff3366"; tagText = "ERROR"; }

    QString html = QString("<span style='color: #64748b;'>[%1]</span> "
                           "<span style='color: %2; font-weight: bold;'>[%3]</span> "
                           "<span style='color: #cbd5e1;'>%4</span>")
                       .arg(timeStr, tagColor, tagText, message.toHtmlEscaped());

    ui->urlConsoleLog->append(html);
    ui->urlConsoleLog->verticalScrollBar()->setValue(ui->urlConsoleLog->verticalScrollBar()->maximum());
}

QString MainWindow::formatSeconds(double totalSeconds) {
    int total = static_cast<int>(totalSeconds);
    int h = total / 3600;
    int m = (total % 3600) / 60;
    int s = total % 60;

    if (h > 0) {
        return QString("%1:%2:%3")
            .arg(h, 2, 10, QChar('0'))
            .arg(m, 2, 10, QChar('0'))
            .arg(s, 2, 10, QChar('0'));
    }
    return QString("%1:%2")
        .arg(m, 2, 10, QChar('0'))
        .arg(s, 2, 10, QChar('0'));
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    const QList<QUrl> urls = event->mimeData()->urls();
    if (!urls.isEmpty()) {
        QString localPath = urls.first().toLocalFile();
        if (!localPath.isEmpty()) {
            // 如果在首页，自动跳转到本地模式
            if (ui->stackedWidget->currentIndex() == 0) {
                ui->stackedWidget->setCurrentIndex(1);
            }
            ui->inputVideoEdit->setText(localPath);
            appendLocalLog(QString("📥 拖拽载入文件: %1").arg(localPath), "INFO");
        }
    }
}
