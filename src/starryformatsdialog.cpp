#include "starryformatsdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QLinearGradient>

StarryFormatsDialog::StarryFormatsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(700, 650);

    QVBoxLayout *dialogLayout = new QVBoxLayout(this);
    dialogLayout->setContentsMargins(18, 16, 18, 18);
    dialogLayout->setSpacing(12);

    // ==========================================
    // 1. 顶部标题栏
    // ==========================================
    QHBoxLayout *titleRow = new QHBoxLayout();
    titleRow->setSpacing(10);
    titleRow->setContentsMargins(4, 0, 0, 0);

    QLabel *iconLbl = new QLabel("🌌", this);
    iconLbl->setStyleSheet("font-size: 18px;");

    QLabel *titleLbl = new QLabel("支持媒体格式与提取矩阵 (Formats Matrix)", this);
    titleLbl->setStyleSheet("color: #00f0ff; font-size: 15px; font-weight: bold; letter-spacing: 1px;");

    QLabel *badgeLbl = new QLabel("FFmpeg & yt-dlp 双核驱动", this);
    badgeLbl->setStyleSheet(
        "color: #38ef7d; font-size: 10px; font-weight: bold;"
        "background-color: rgba(56, 239, 125, 0.12);"
        "border: 1px solid rgba(56, 239, 125, 0.4);"
        "border-radius: 4px; padding: 2px 6px;"
    );

    QPushButton *closeBtn = new QPushButton("✕", this);
    closeBtn->setCursor(Qt::PointingHandCursor);
    closeBtn->setToolTip("关闭窗口 (Esc)");
    closeBtn->setFixedSize(28, 28);
    closeBtn->setStyleSheet(
        "QPushButton {"
        "  background: transparent; border: none; border-radius: 6px; color: #94a3b8; font-size: 14px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #ff3366; color: #ffffff; }"
        "QPushButton:pressed { background-color: #d91b4c; color: #ffffff; }"
    );
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

    titleRow->addWidget(iconLbl);
    titleRow->addWidget(titleLbl);
    titleRow->addWidget(badgeLbl);
    titleRow->addStretch();
    titleRow->addWidget(closeBtn);
    dialogLayout->addLayout(titleRow);

    // 分割流光线
    QFrame *divLine = new QFrame(this);
    divLine->setFixedHeight(1);
    divLine->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0,240,255,10), stop:0.2 rgba(0,240,255,140), stop:0.8 rgba(0,240,255,140), stop:1 rgba(0,240,255,10));");
    dialogLayout->addWidget(divLine);

    // ==========================================
    // 2. 中间卡片滚动区域 (超高清无模糊呈现)
    // ==========================================
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(
        "QScrollArea { background: transparent; border: none; }"
        "QScrollBar:vertical { background: rgba(10, 16, 32, 0.5); width: 6px; border-radius: 3px; }"
        "QScrollBar::handle:vertical { background: rgba(0, 240, 255, 0.35); min-height: 20px; border-radius: 3px; }"
        "QScrollBar::handle:vertical:hover { background: rgba(0, 240, 255, 0.7); }"
    );

    QWidget *scrollContainer = new QWidget();
    scrollContainer->setStyleSheet("background: transparent;");
    QVBoxLayout *cardsLayout = new QVBoxLayout(scrollContainer);
    cardsLayout->setContentsMargins(2, 4, 8, 4);
    cardsLayout->setSpacing(10);

    // 卡片 1: 本地主流音视频格式
    cardsLayout->addWidget(createCategoryCard(
        "📁 本地主流通用视频格式 (秒级无损剥离 / 极速高精转码)",
        {"MP4 (通用推荐)", "MKV (全轨复合)", "MOV (苹果无损)", "WebM (谷歌高清)", "AVI (经典无损)", "FLV (流媒体)"},
        "说明：支持音轨直接流拷贝（0秒转码抽取），亦可无损解码并转码为任意目标音频格式。"
    ));

    // 卡片 2: 高清原盘、摄像机与广播流
    cardsLayout->addWidget(createCategoryCard(
        "📼 高清原盘、摄像机素材与网络传输流",
        {"TS (广播流/HLS)", "M2TS (蓝光BDAV)", "MTS (索尼/松下)", "VOB (DVD原盘)", "WMV (微软媒体)", "M4V (苹果影视)", "OGV", "F4V"},
        "说明：完整支持多分段视频流聚合、蓝光盘原生高清音频轨道及 DVD 原盘多声道剥离。"
    ));

    // 卡片 3: 经典与冷门视频封装
    cardsLayout->addWidget(createCategoryCard(
        "🎞️ 经典老旧与特殊行业视频封装",
        {"RMVB (经典动态流)", "RM", "3GP (手机录像)", "MPG / MPEG", "ASF", "DIVX", "DAV (监控安防)", "MXF (广电素材)"},
        "说明：针对老旧及安防监控视频进行底层精准解码校准，自动转码为标准 MP3 / WAV 消除播放异常。"
    ));

    // 卡片 4: 目标可提取音频规格
    cardsLayout->addWidget(createCategoryCard(
        "🎵 目标音频导出格式全矩阵 (12 种主流格式支持)",
        {"MP3 (320k超清/最兼容)", "M4A (苹果AAC/ALAC)", "WAV (无损母带级)", "FLAC (无损压缩)", "AAC (流媒体标准)", "OGG (Vorbis)", "OPUS (高保真)", "WMA", "AC3", "MKA", "AIFF"},
        "说明：支持 320k / 256k / 192k / 128k 自定义比特率，支持 48000Hz 采样率及立体声/单声道声道转换。"
    ));

    // 卡片 5: 全网流媒体视频提取支持
    cardsLayout->addWidget(createCategoryCard(
        "🌐 全网网页流媒体视频下载与音频提取 (yt-dlp 驱动)",
        {"哔哩哔哩 (Bilibili)", "抖音 / TikTok", "YouTube", "小红书", "新浪微博", "Twitter / X", "m3u8 网页分片流", "mp4 网络直链"},
        "说明：输入网址可一键下载完整高清视频（MP4/MKV，最高支持4K/1080P），或一键分离纯音频（MP3/M4A）。"
    ));

    // 智能诊断提示框
    QWidget *tipCard = new QWidget(scrollContainer);
    tipCard->setStyleSheet(
        "QWidget {"
        "  background-color: rgba(56, 189, 248, 0.08);"
        "  border: 1px dashed rgba(0, 240, 255, 0.35);"
        "  border-radius: 8px;"
        "}"
    );
    QVBoxLayout *tipLayout = new QVBoxLayout(tipCard);
    tipLayout->setContentsMargins(12, 10, 12, 10);
    QLabel *tipLbl = new QLabel(
        "💡 <b>智能后缀诊断技巧</b>：您无需记忆格式特性，直接在主界面源视频输入框内输入后缀名（如 <code>rmvb</code>、<code>mkv</code>、<code>ts</code>），系统将即刻提供专属诊断建议与最佳码率方案！",
        tipCard
    );
    tipLbl->setStyleSheet("color: #7dd3fc; font-size: 11px; border: none; background: transparent;");
    tipLbl->setWordWrap(true);
    tipLayout->addWidget(tipLbl);
    cardsLayout->addWidget(tipCard);

    scrollArea->setWidget(scrollContainer);
    dialogLayout->addWidget(scrollArea);

    // ==========================================
    // 3. 底部操作按钮
    // ==========================================
    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->addStretch();
    QPushButton *confirmBtn = new QPushButton("✨ 了解并返回工作台", this);
    confirmBtn->setCursor(Qt::PointingHandCursor);
    confirmBtn->setFixedSize(180, 34);
    confirmBtn->setStyleSheet(
        "QPushButton {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0284c7, stop:0.5 #3b82f6, stop:1 #6366f1);"
        "  border: 1px solid rgba(0, 240, 255, 0.75);"
        "  border-radius: 8px;"
        "  color: #ffffff;"
        "  font-weight: bold;"
        "  font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0ea5e9, stop:0.5 #60a5fa, stop:1 #818cf8);"
        "  border: 1px solid #ffffff;"
        "}"
        "QPushButton:pressed {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0369a1, stop:1 #1d4ed8);"
        "}"
    );
    connect(confirmBtn, &QPushButton::clicked, this, &QDialog::accept);
    btnRow->addWidget(confirmBtn);
    btnRow->addStretch();
    dialogLayout->addLayout(btnRow);
}

QWidget *StarryFormatsDialog::createCategoryCard(const QString &iconAndTitle, const QStringList &tags, const QString &description) {
    QWidget *card = new QWidget(this);
    card->setStyleSheet(
        "QWidget {"
        "  background-color: rgba(11, 19, 40, 0.82);"
        "  border: 1px solid rgba(0, 240, 255, 0.20);"
        "  border-radius: 10px;"
        "}"
    );

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(14, 10, 14, 12);
    cardLayout->setSpacing(8);

    // 标题
    QLabel *titleLbl = new QLabel(iconAndTitle, card);
    titleLbl->setStyleSheet("color: #38bdf8; font-weight: bold; font-size: 13px; border: none; background: transparent;");
    cardLayout->addWidget(titleLbl);

    // 标签网格分行布局
    QHBoxLayout *rowLayout = new QHBoxLayout();
    rowLayout->setSpacing(6);
    rowLayout->setContentsMargins(0, 0, 0, 0);
    int count = 0;

    for (const QString &t : tags) {
        bool hi = t.contains("无损") || t.contains("推荐") || t.contains("MP4") || t.contains("MP3") || t.contains("FLAC") || t.contains("B站");
        rowLayout->addWidget(createTag(t, hi));
        count++;
        if (count % 4 == 0) {
            rowLayout->addStretch();
            cardLayout->addLayout(rowLayout);
            rowLayout = new QHBoxLayout();
            rowLayout->setSpacing(6);
            rowLayout->setContentsMargins(0, 0, 0, 0);
        }
    }
    if (count % 4 != 0) {
        rowLayout->addStretch();
        cardLayout->addLayout(rowLayout);
    }

    // 底部说明
    if (!description.isEmpty()) {
        QLabel *descLbl = new QLabel(description, card);
        descLbl->setStyleSheet("color: #94a3b8; font-size: 11px; border: none; background: transparent;");
        descLbl->setWordWrap(true);
        cardLayout->addWidget(descLbl);
    }

    return card;
}

QWidget *StarryFormatsDialog::createTag(const QString &text, bool highlight) {
    QLabel *tag = new QLabel(text, this);
    if (highlight) {
        tag->setStyleSheet(
            "background-color: rgba(56, 239, 125, 0.14);"
            "border: 1px solid rgba(56, 239, 125, 0.45);"
            "border-radius: 5px;"
            "color: #38ef7d;"
            "font-size: 11px;"
            "font-weight: bold;"
            "padding: 3px 8px;"
        );
    } else {
        tag->setStyleSheet(
            "background-color: rgba(0, 240, 255, 0.08);"
            "border: 1px solid rgba(0, 240, 255, 0.28);"
            "border-radius: 5px;"
            "color: #d1e8ff;"
            "font-size: 11px;"
            "font-weight: 500;"
            "padding: 3px 8px;"
        );
    }
    return tag;
}

void StarryFormatsDialog::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    int r = 14;
    QRectF rectF = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);

    QPainterPath path;
    path.addRoundedRect(rectF, r, r);

    // 1. 深邃宇宙暗夜蓝渐变背景
    QLinearGradient bgGrad(0, 0, width(), height());
    bgGrad.setColorAt(0.0, QColor(7, 13, 28, 252));
    bgGrad.setColorAt(0.6, QColor(10, 18, 38, 254));
    bgGrad.setColorAt(1.0, QColor(5, 9, 20, 255));
    p.fillPath(path, bgGrad);

    // 2. 双层极光柔和微光发光外轮廓 (与主窗口 100% 呼应)
    QPen softAuraPen(QColor(0, 240, 255, 28), 2.6);
    softAuraPen.setJoinStyle(Qt::RoundJoin);
    p.setPen(softAuraPen);
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(rectF.adjusted(1.0, 1.0, -1.0, -1.0), r - 1.0, r - 1.0);

    QPen corePen(QColor(0, 240, 255, 85), 1.0);
    corePen.setJoinStyle(Qt::RoundJoin);
    p.setPen(corePen);
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(rectF, r, r);
}

void StarryFormatsDialog::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragPos = event->globalPosition().toPoint() - frameGeometry().topLeft();
    }
    QDialog::mousePressEvent(event);
}

void StarryFormatsDialog::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - m_dragPos);
    }
    QDialog::mouseMoveEvent(event);
}
