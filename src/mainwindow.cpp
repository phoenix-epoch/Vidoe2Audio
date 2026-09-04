#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDateTime>
#include <QScrollBar>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_starryBg(nullptr),
      m_scrollArea(nullptr),
      m_contentContainer(nullptr),
      m_ffmpegHelper(new FFmpegHelper(this))
{
    setupUi();
    checkAndDetectFFmpeg();

    // 连接 FFmpeg 信号
    connect(m_ffmpegHelper, &FFmpegHelper::started, this, &MainWindow::onFFmpegStarted);
    connect(m_ffmpegHelper, &FFmpegHelper::logMessage, this, &MainWindow::onFFmpegLog);
    connect(m_ffmpegHelper, &FFmpegHelper::progressUpdated, this, &MainWindow::onFFmpegProgress);
    connect(m_ffmpegHelper, &FFmpegHelper::extractionFinished, this, &MainWindow::onFFmpegFinished);

    appendLog("🌟 星空音频分离系统 (Starry Audio Extractor) 已就绪", "INFO");
    appendLog("💡 支持将音视频文件直接拖拽至窗口任意位置快速载入", "INFO");
}

void MainWindow::setupUi() {
    setWindowTitle("🌌 星空音频分离系统 - Starry Audio Extractor (FFmpeg Powered)");
    // 灵活的自适应尺寸：兼容 1366x768、1080p 150% 缩放及高分屏
    setMinimumSize(620, 480);
    resize(860, 680);
    setAcceptDrops(true);

    // 1. 底层全屏动态星空背景
    m_starryBg = new StarryBackground(this);
    setCentralWidget(m_starryBg);

    QVBoxLayout *bgLayout = new QVBoxLayout(m_starryBg);
    bgLayout->setContentsMargins(0, 0, 0, 0);
    bgLayout->setSpacing(0);

    // 2. 创建半透明自适应滚动区域 (确保在小屏幕/高DPI下不被挤压截断)
    m_scrollArea = new QScrollArea(m_starryBg);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setStyleSheet("QScrollArea { background: transparent; border: none; }");

    // 3. 内部主容器
    m_contentContainer = new QWidget();
    m_contentContainer->setStyleSheet("background: transparent;");

    QVBoxLayout *mainLayout = new QVBoxLayout(m_contentContainer);
    mainLayout->setContentsMargins(20, 16, 20, 16);
    mainLayout->setSpacing(12);

    // ==========================================
    // 顶部 Header 标题
    // ==========================================
    QHBoxLayout *headerLayout = new QHBoxLayout();
    QVBoxLayout *titleTextLayout = new QVBoxLayout();
    titleTextLayout->setSpacing(2);

    QLabel *titleLabel = new QLabel("🌌 STARRY AUDIO EXTRACTOR", this);
    titleLabel->setObjectName("headerTitle");
    QLabel *subTitleLabel = new QLabel("✦ 深度时空多格式音视频音频抽取与高精转码控制台 ✦", this);
    subTitleLabel->setObjectName("headerSubtitle");
    titleTextLayout->addWidget(titleLabel);
    titleTextLayout->addWidget(subTitleLabel);

    headerLayout->addLayout(titleTextLayout);
    headerLayout->addStretch();
    mainLayout->addLayout(headerLayout);

    // ==========================================
    // 区域 1: 输入源视频设置
    // ==========================================
    QGroupBox *inputGroup = new QGroupBox("📁 输入源视频 (Source Video)", m_contentContainer);
    QVBoxLayout *inputGroupLayout = new QVBoxLayout(inputGroup);
    inputGroupLayout->setContentsMargins(12, 12, 12, 10);
    inputGroupLayout->setSpacing(6);

    QHBoxLayout *inputRow = new QHBoxLayout();
    inputRow->setSpacing(8);
    m_inputVideoEdit = new QLineEdit(inputGroup);
    m_inputVideoEdit->setPlaceholderText("请选择或拖拽音视频文件至此 (支持 MP4, MKV, AVI, MOV, FLV, TS, RMVB, WebM 等)...");
    m_browseInputBtn = new QPushButton("📂 浏览视频...", inputGroup);
    m_browseInputBtn->setCursor(Qt::PointingHandCursor);
    inputRow->addWidget(m_inputVideoEdit, 1);
    inputRow->addWidget(m_browseInputBtn);
    inputGroupLayout->addLayout(inputRow);

    m_fileInfoLabel = new QLabel("文件状态: 未载入视频", inputGroup);
    m_fileInfoLabel->setStyleSheet("color: #64748b; font-size: 11px;");
    inputGroupLayout->addWidget(m_fileInfoLabel);

    mainLayout->addWidget(inputGroup);

    // ==========================================
    // 区域 2: 分离提取与转码配置
    // ==========================================
    QGroupBox *settingsGroup = new QGroupBox("⚙️ 分离提取参数 (Extraction Settings)", m_contentContainer);
    QVBoxLayout *settingsGroupLayout = new QVBoxLayout(settingsGroup);
    settingsGroupLayout->setContentsMargins(12, 12, 12, 10);
    settingsGroupLayout->setSpacing(8);

    // 模式选择单选
    QHBoxLayout *modeRow = new QHBoxLayout();
    m_radioTranscode = new QRadioButton("🎛️ 格式重编码转换 (自定义格式、码率与采样率)", settingsGroup);
    m_radioDirectCopy = new QRadioButton("⚡ 极速无损流拷贝 (-c:a copy 毫秒级提取原音轨)", settingsGroup);
    m_radioTranscode->setChecked(true);
    modeRow->addWidget(m_radioTranscode);
    modeRow->addWidget(m_radioDirectCopy);
    modeRow->addStretch();
    settingsGroupLayout->addLayout(modeRow);

    // 参数网格布局
    QGridLayout *paramGrid = new QGridLayout();
    paramGrid->setHorizontalSpacing(14);
    paramGrid->setVerticalSpacing(6);

    // 目标格式
    QLabel *formatLbl = new QLabel("目标音频格式:", settingsGroup);
    m_formatCombo = new QComboBox(settingsGroup);
    m_formatCombo->addItems(FFmpegHelper::supportedAudioFormats());
    m_formatCombo->setCurrentText("MP3");
    paramGrid->addWidget(formatLbl, 0, 0);
    paramGrid->addWidget(m_formatCombo, 0, 1);

    // 音频码率
    QLabel *bitrateLbl = new QLabel("音频比特率:", settingsGroup);
    m_bitrateCombo = new QComboBox(settingsGroup);
    m_bitrateCombo->addItems({"Auto (自适应)", "320k (超清音质)", "256k (高品质)", "192k (标准品质)", "128k (流畅)", "64k (低码率)"});
    m_bitrateCombo->setCurrentIndex(1); // 默认 320k
    paramGrid->addWidget(bitrateLbl, 0, 2);
    paramGrid->addWidget(m_bitrateCombo, 0, 3);

    // 采样率
    QLabel *sampleRateLbl = new QLabel("音频采样率:", settingsGroup);
    m_sampleRateCombo = new QComboBox(settingsGroup);
    m_sampleRateCombo->addItems({"Auto (源采样率)", "48000 Hz", "44100 Hz", "32000 Hz", "24000 Hz", "16000 Hz"});
    m_sampleRateCombo->setCurrentIndex(0);
    paramGrid->addWidget(sampleRateLbl, 1, 0);
    paramGrid->addWidget(m_sampleRateCombo, 1, 1);

    // 声道设置
    QLabel *channelLbl = new QLabel("声道配置:", settingsGroup);
    m_channelCombo = new QComboBox(settingsGroup);
    m_channelCombo->addItems({"Auto (保持源声道)", "立体声 (2 Channels)", "单声道 (Mono)"});
    m_channelCombo->setCurrentIndex(0);
    paramGrid->addWidget(channelLbl, 1, 2);
    paramGrid->addWidget(m_channelCombo, 1, 3);

    settingsGroupLayout->addLayout(paramGrid);

    // 覆盖已有文件
    m_overwriteCheck = new QCheckBox("如果目标文件已存在则直接覆盖 (Overwrite)", settingsGroup);
    m_overwriteCheck->setChecked(true);
    settingsGroupLayout->addWidget(m_overwriteCheck);

    mainLayout->addWidget(settingsGroup);

    // ==========================================
    // 区域 3: 目标输出与环境路径
    // ==========================================
    QGroupBox *pathGroup = new QGroupBox("💾 目标输出与 FFmpeg 环境 (Output & Environment)", m_contentContainer);
    QGridLayout *pathGrid = new QGridLayout(pathGroup);
    pathGrid->setContentsMargins(12, 12, 12, 10);
    pathGrid->setHorizontalSpacing(10);
    pathGrid->setVerticalSpacing(6);

    // 输出文件路径
    QLabel *outputLbl = new QLabel("音频输出路径:", pathGroup);
    m_outputPathEdit = new QLineEdit(pathGroup);
    m_outputPathEdit->setPlaceholderText("选择视频后自动生成输出路径，也可手动修改...");
    m_browseOutputBtn = new QPushButton("📂 更改目标...", pathGroup);
    m_browseOutputBtn->setCursor(Qt::PointingHandCursor);
    pathGrid->addWidget(outputLbl, 0, 0);
    pathGrid->addWidget(m_outputPathEdit, 0, 1);
    pathGrid->addWidget(m_browseOutputBtn, 0, 2);

    // FFmpeg 路径
    QLabel *ffmpegLbl = new QLabel("FFmpeg 执行路径:", pathGroup);
    m_ffmpegPathEdit = new QLineEdit(pathGroup);
    m_ffmpegPathEdit->setPlaceholderText("自动检测系统 PATH 或程序同级 ffmpeg.exe...");
    m_browseFFmpegBtn = new QPushButton("🛠️ 指定 FFmpeg...", pathGroup);
    m_browseFFmpegBtn->setCursor(Qt::PointingHandCursor);
    m_ffmpegStatusBadge = new QLabel("🔍 检测中...", pathGroup);
    m_ffmpegStatusBadge->setStyleSheet("color: #ffaa00; font-weight: bold;");

    QHBoxLayout *ffmpegRow = new QHBoxLayout();
    ffmpegRow->setSpacing(8);
    ffmpegRow->addWidget(m_ffmpegPathEdit, 1);
    ffmpegRow->addWidget(m_ffmpegStatusBadge);
    ffmpegRow->addWidget(m_browseFFmpegBtn);

    pathGrid->addWidget(ffmpegLbl, 1, 0);
    pathGrid->addLayout(ffmpegRow, 1, 1, 1, 2);

    mainLayout->addWidget(pathGroup);

    // ==========================================
    // 区域 4: 进度与核心操作按钮
    // ==========================================
    QVBoxLayout *actionSection = new QVBoxLayout();
    actionSection->setSpacing(6);

    // 进度与耗时
    QHBoxLayout *progressInfoRow = new QHBoxLayout();
    QLabel *progressTitle = new QLabel("⚡ 处理进度:", m_contentContainer);
    m_progressDetailLabel = new QLabel("就绪 (0%) - 00:00 / 00:00", m_contentContainer);
    m_progressDetailLabel->setStyleSheet("color: #00f0ff; font-weight: bold;");
    progressInfoRow->addWidget(progressTitle);
    progressInfoRow->addWidget(m_progressDetailLabel);
    progressInfoRow->addStretch();
    actionSection->addLayout(progressInfoRow);

    m_progressBar = new QProgressBar(m_contentContainer);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    actionSection->addWidget(m_progressBar);

    // 动作按钮行
    QHBoxLayout *actionBtnRow = new QHBoxLayout();
    actionBtnRow->setSpacing(10);

    m_startBtn = new QPushButton("🚀 开始分离提取音频", m_contentContainer);
    m_startBtn->setObjectName("primaryActionBtn");
    m_startBtn->setCursor(Qt::PointingHandCursor);
    m_startBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_openFolderBtn = new QPushButton("📁 打开输出目录", m_contentContainer);
    m_openFolderBtn->setCursor(Qt::PointingHandCursor);
    m_openFolderBtn->setEnabled(false);

    m_playAudioBtn = new QPushButton("🎵 播放提取音频", m_contentContainer);
    m_playAudioBtn->setCursor(Qt::PointingHandCursor);
    m_playAudioBtn->setEnabled(false);

    actionBtnRow->addWidget(m_startBtn, 3);
    actionBtnRow->addWidget(m_openFolderBtn, 2);
    actionBtnRow->addWidget(m_playAudioBtn, 2);
    actionSection->addLayout(actionBtnRow);

    mainLayout->addLayout(actionSection);

    // ==========================================
    // 区域 5: 终端控制台日志
    // ==========================================
    QGroupBox *logGroup = new QGroupBox("📟 任务控制台日志 (Terminal Logs)", m_contentContainer);
    QVBoxLayout *logLayout = new QVBoxLayout(logGroup);
    logLayout->setContentsMargins(10, 10, 10, 8);
    logLayout->setSpacing(6);

    m_consoleLog = new QTextEdit(logGroup);
    m_consoleLog->setObjectName("consoleOutput");
    m_consoleLog->setReadOnly(true);
    m_consoleLog->setMinimumHeight(90);
    m_consoleLog->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    logLayout->addWidget(m_consoleLog);

    QHBoxLayout *logBottomRow = new QHBoxLayout();
    logBottomRow->addStretch();
    m_clearLogBtn = new QPushButton("🗑️ 清空控制台", logGroup);
    m_clearLogBtn->setCursor(Qt::PointingHandCursor);
    logBottomRow->addWidget(m_clearLogBtn);
    logLayout->addLayout(logBottomRow);

    mainLayout->addWidget(logGroup, 1);

    m_scrollArea->setWidget(m_contentContainer);
    bgLayout->addWidget(m_scrollArea);

    // ==========================================
    // 信号与槽绑定
    // ==========================================
    connect(m_browseInputBtn, &QPushButton::clicked, this, &MainWindow::onBrowseInputVideo);
    connect(m_browseOutputBtn, &QPushButton::clicked, this, &MainWindow::onBrowseOutputDir);
    connect(m_browseFFmpegBtn, &QPushButton::clicked, this, &MainWindow::onBrowseFFmpeg);
    connect(m_startBtn, &QPushButton::clicked, this, &MainWindow::onStartOrCancelClicked);
    connect(m_openFolderBtn, &QPushButton::clicked, this, &MainWindow::onOpenOutputFolder);
    connect(m_playAudioBtn, &QPushButton::clicked, this, &MainWindow::onPlayAudio);
    connect(m_clearLogBtn, &QPushButton::clicked, this, &MainWindow::onClearLog);

    connect(m_radioDirectCopy, &QRadioButton::toggled, this, &MainWindow::onModeChanged);
    connect(m_radioTranscode, &QRadioButton::toggled, this, &MainWindow::onModeChanged);
    connect(m_formatCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onFormatChanged);
    connect(m_inputVideoEdit, &QLineEdit::textChanged, this, [this](const QString &) {
        updateDefaultOutputPath();
    });
    connect(m_ffmpegPathEdit, &QLineEdit::textChanged, this, [this](const QString &) {
        checkAndDetectFFmpeg();
    });
}

void MainWindow::checkAndDetectFFmpeg() {
    QString current = m_ffmpegPathEdit->text().trimmed();
    QString found = FFmpegHelper::findFFmpegBinary(current);

    if (!found.isEmpty()) {
        QString ver;
        if (FFmpegHelper::checkFFmpegExecutable(found, &ver)) {
            m_ffmpegPathEdit->setText(found);
            m_ffmpegStatusBadge->setText("● FFmpeg 就绪");
            m_ffmpegStatusBadge->setStyleSheet("color: #00ff88; font-weight: bold;");
            m_ffmpegStatusBadge->setToolTip(ver);
            return;
        }
    }

    m_ffmpegStatusBadge->setText("● 未检测到 FFmpeg");
    m_ffmpegStatusBadge->setStyleSheet("color: #ff3366; font-weight: bold;");
    m_ffmpegStatusBadge->setToolTip("请点击【指定 FFmpeg】选择本地 ffmpeg.exe 或将其添加至环境变量 PATH");
}

void MainWindow::onModeChanged() {
    bool transcode = m_radioTranscode->isChecked();
    m_formatCombo->setEnabled(transcode);
    m_bitrateCombo->setEnabled(transcode);
    m_sampleRateCombo->setEnabled(transcode);
    m_channelCombo->setEnabled(transcode);

    updateDefaultOutputPath();
}

void MainWindow::onFormatChanged(int index) {
    Q_UNUSED(index);
    updateDefaultOutputPath();
}

void MainWindow::updateDefaultOutputPath() {
    QString inputPath = m_inputVideoEdit->text().trimmed();
    if (inputPath.isEmpty()) {
        m_fileInfoLabel->setText("文件状态: 未载入视频");
        return;
    }

    QFileInfo fi(inputPath);
    if (!fi.exists()) {
        m_fileInfoLabel->setText("❌ 视频文件不存在，请核实路径");
        return;
    }

    double sizeMB = fi.size() / (1024.0 * 1024.0);
    m_fileInfoLabel->setText(QString("✅ 已加载: %1 | 大小: %2 MB | 格式: %3")
                                 .arg(fi.fileName())
                                 .arg(sizeMB, 0, 'f', 2)
                                 .arg(fi.suffix().toUpper()));

    QString dir = fi.absolutePath();
    QString baseName = fi.completeBaseName();

    QString ext;
    if (m_radioDirectCopy->isChecked()) {
        QString vExt = fi.suffix().toLower();
        if (vExt == "mp4" || vExt == "m4v" || vExt == "mov") {
            ext = "m4a";
        } else if (vExt == "webm" || vExt == "mkv") {
            ext = "mka";
        } else if (vExt == "flv") {
            ext = "aac";
        } else {
            ext = "aac";
        }
    } else {
        ext = FFmpegHelper::getAudioExtensionForFormat(m_formatCombo->currentText());
    }

    QString defaultOut = QDir(dir).filePath(QString("%1_extracted.%2").arg(baseName, ext));
    m_outputPathEdit->setText(defaultOut);
}

void MainWindow::onBrowseInputVideo() {
    QString filter = FFmpegHelper::getFilterStringForVideoFiles();
    QString path = QFileDialog::getOpenFileName(this, "选择需要提取音频的源音视频文件", "", filter);
    if (!path.isEmpty()) {
        m_inputVideoEdit->setText(path);
        appendLog(QString("📂 已选择源视频: %1").arg(path), "INFO");
    }
}

void MainWindow::onBrowseOutputDir() {
    QString currentOut = m_outputPathEdit->text().trimmed();
    QString defaultDir = "";
    if (!currentOut.isEmpty()) {
        defaultDir = QFileInfo(currentOut).absolutePath();
    }

    QString selected = QFileDialog::getSaveFileName(this, "指定提取后的音频输出文件", currentOut, "音频文件 (*.*)");
    if (!selected.isEmpty()) {
        m_outputPathEdit->setText(selected);
        appendLog(QString("💾 已指定输出文件: %1").arg(selected), "INFO");
    }
}

void MainWindow::onBrowseFFmpeg() {
    QString path = QFileDialog::getOpenFileName(this, "选择 ffmpeg.exe 可执行文件", "", "可执行程序 (ffmpeg.exe *.exe);;所有文件 (*.*)");
    if (!path.isEmpty()) {
        m_ffmpegPathEdit->setText(path);
        checkAndDetectFFmpeg();
    }
}

void MainWindow::onStartOrCancelClicked() {
    if (m_ffmpegHelper->isRunning()) {
        m_ffmpegHelper->cancelExtraction();
        return;
    }

    QString inputPath = m_inputVideoEdit->text().trimmed();
    QString outputPath = m_outputPathEdit->text().trimmed();
    QString ffmpegPath = m_ffmpegPathEdit->text().trimmed();

    if (inputPath.isEmpty() || !QFileInfo::exists(inputPath)) {
        appendLog("❌ 请先选择有效的视频源文件！", "ERROR");
        return;
    }

    if (outputPath.isEmpty()) {
        appendLog("❌ 请指定输出音频文件保存路径！", "ERROR");
        return;
    }

    ExtractionOptions options;
    options.inputFilePath = inputPath;
    options.outputFilePath = outputPath;
    options.overwriteOutput = m_overwriteCheck->isChecked();
    options.mode = m_radioDirectCopy->isChecked() ? ExtractMode::DirectCopy : ExtractMode::Transcode;
    options.targetFormat = m_formatCombo->currentText();

    // 码率
    QString br = m_bitrateCombo->currentText();
    if (br.contains("320k")) options.bitrate = "320k";
    else if (br.contains("256k")) options.bitrate = "256k";
    else if (br.contains("192k")) options.bitrate = "192k";
    else if (br.contains("128k")) options.bitrate = "128k";
    else if (br.contains("64k")) options.bitrate = "64k";
    else options.bitrate = "Auto";

    // 采样率
    QString sr = m_sampleRateCombo->currentText();
    if (sr.contains("48000")) options.sampleRate = "48000";
    else if (sr.contains("44100")) options.sampleRate = "44100";
    else if (sr.contains("32000")) options.sampleRate = "32000";
    else if (sr.contains("24000")) options.sampleRate = "24000";
    else if (sr.contains("16000")) options.sampleRate = "16000";
    else options.sampleRate = "Auto";

    // 声道
    int chIdx = m_channelCombo->currentIndex();
    if (chIdx == 1) options.channels = "2";
    else if (chIdx == 2) options.channels = "1";
    else options.channels = "Auto";

    m_elapsedTimer.restart();
    m_ffmpegHelper->startExtraction(ffmpegPath, options);
}

void MainWindow::onFFmpegStarted() {
    setExtractionUiState(true);
    m_progressBar->setValue(0);
    m_progressDetailLabel->setText("🚀 正在初始化 FFmpeg 引擎抽取音频...");
}

void MainWindow::onFFmpegLog(const QString &message, const QString &type) {
    appendLog(message, type);
}

void MainWindow::onFFmpegProgress(int percentage, double currentSec, double totalDurationSec) {
    m_progressBar->setValue(percentage);

    qint64 elapsedMs = m_elapsedTimer.elapsed();
    QString elapsedStr = formatSeconds(elapsedMs / 1000.0);

    if (totalDurationSec > 0.0) {
        m_progressDetailLabel->setText(QString("⚡ 进度: %1% (%2 / %3) | 已耗时: %4")
                                           .arg(percentage)
                                           .arg(formatSeconds(currentSec))
                                           .arg(formatSeconds(totalDurationSec))
                                           .arg(elapsedStr));
    } else {
        m_progressDetailLabel->setText(QString("⚡ 处理中... | 已提取: %1 | 已耗时: %2")
                                           .arg(formatSeconds(currentSec))
                                           .arg(elapsedStr));
    }
}

void MainWindow::onFFmpegFinished(bool success, const QString &outputFilePath, const QString &errorMsg) {
    Q_UNUSED(errorMsg);
    setExtractionUiState(false);

    if (success) {
        m_lastGeneratedAudioPath = outputFilePath;
        m_openFolderBtn->setEnabled(true);
        m_playAudioBtn->setEnabled(true);
        m_progressDetailLabel->setText("🎉 音频分离提取完成！(100%)");
    } else {
        m_progressDetailLabel->setText("❌ 提取中止或发生错误");
    }
}

void MainWindow::setExtractionUiState(bool extracting) {
    if (extracting) {
        m_startBtn->setText("⏹️ 终止提取任务");
        m_startBtn->setObjectName("cancelActionBtn");
        m_startBtn->setStyle(m_startBtn->style()); // 强制刷新 QSS
        m_browseInputBtn->setEnabled(false);
        m_browseOutputBtn->setEnabled(false);
        m_browseFFmpegBtn->setEnabled(false);
        m_openFolderBtn->setEnabled(false);
        m_playAudioBtn->setEnabled(false);
    } else {
        m_startBtn->setText("🚀 开始分离提取音频");
        m_startBtn->setObjectName("primaryActionBtn");
        m_startBtn->setStyle(m_startBtn->style());
        m_browseInputBtn->setEnabled(true);
        m_browseOutputBtn->setEnabled(true);
        m_browseFFmpegBtn->setEnabled(true);
    }
}

void MainWindow::onOpenOutputFolder() {
    if (!m_lastGeneratedAudioPath.isEmpty() && QFileInfo::exists(m_lastGeneratedAudioPath)) {
        QString folder = QFileInfo(m_lastGeneratedAudioPath).absolutePath();
        QDesktopServices::openUrl(QUrl::fromLocalFile(folder));
    } else {
        QString currentOut = m_outputPathEdit->text().trimmed();
        if (!currentOut.isEmpty()) {
            QString folder = QFileInfo(currentOut).absolutePath();
            QDesktopServices::openUrl(QUrl::fromLocalFile(folder));
        }
    }
}

void MainWindow::onPlayAudio() {
    if (!m_lastGeneratedAudioPath.isEmpty() && QFileInfo::exists(m_lastGeneratedAudioPath)) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(m_lastGeneratedAudioPath));
    }
}

void MainWindow::onClearLog() {
    m_consoleLog->clear();
    appendLog("📟 控制台日志已清空", "INFO");
}

void MainWindow::appendLog(const QString &message, const QString &type) {
    QString timeStr = QDateTime::currentDateTime().toString("HH:mm:ss");
    QString tagColor = "#00f0ff";
    QString tagText = "INFO";

    if (type == "SUCCESS") {
        tagColor = "#00ff88";
        tagText = "SUCCESS";
    } else if (type == "WARNING") {
        tagColor = "#ffaa00";
        tagText = "WARN";
    } else if (type == "ERROR") {
        tagColor = "#ff3366";
        tagText = "ERROR";
    } else if (type == "FFMPEG") {
        tagColor = "#c77dff";
        tagText = "FFMPEG";
    }

    QString html = QString("<span style='color: #64748b;'>[%1]</span> "
                           "<span style='color: %2; font-weight: bold;'>[%3]</span> "
                           "<span style='color: #cbd5e1;'>%4</span>")
                       .arg(timeStr, tagColor, tagText, message.toHtmlEscaped());

    m_consoleLog->append(html);
    m_consoleLog->verticalScrollBar()->setValue(m_consoleLog->verticalScrollBar()->maximum());
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
            m_inputVideoEdit->setText(localPath);
            appendLog(QString("📥 拖拽载入文件: %1").arg(localPath), "INFO");
        }
    }
}
