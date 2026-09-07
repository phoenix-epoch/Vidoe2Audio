#include "starrytitlebar.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QWindow>
#include <QLinearGradient>

StarryTitleBar::StarryTitleBar(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("starryTitleBar");
    setFixedHeight(38);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(14, 0, 8, 0);
    mainLayout->setSpacing(8);

    // 1. 图标
    m_iconLabel = new QLabel("🌌", this);
    m_iconLabel->setObjectName("titleIconLabel");

    // 2. 主标题
    m_titleLabel = new QLabel("星空音视频分离中心", this);
    m_titleLabel->setObjectName("titleMainLabel");

    // 3. 英文副标
    m_subTitleLabel = new QLabel("STARRY EXTRACTOR", this);
    m_subTitleLabel->setObjectName("titleSubLabel");

    // 4. 版本微光胶囊
    m_badgeLabel = new QLabel("✦ v1.0 ✦", this);
    m_badgeLabel->setObjectName("titleBadgeLabel");

    mainLayout->addWidget(m_iconLabel);
    mainLayout->addWidget(m_titleLabel);
    mainLayout->addWidget(m_subTitleLabel);
    mainLayout->addWidget(m_badgeLabel);

    // 中间弹簧（使用户可在空白区域自由拖拽移动窗口）
    mainLayout->addStretch();

    // 5. 最小化按钮
    m_minBtn = new QPushButton("─", this);
    m_minBtn->setObjectName("titleBtnMin");
    m_minBtn->setToolTip("最小化窗口");
    m_minBtn->setCursor(Qt::PointingHandCursor);
    connect(m_minBtn, &QPushButton::clicked, this, &StarryTitleBar::onMinimizeClicked);

    // 6. 最大化/还原按钮
    m_maxBtn = new QPushButton("▢", this);
    m_maxBtn->setObjectName("titleBtnMax");
    m_maxBtn->setToolTip("最大化 / 还原");
    m_maxBtn->setCursor(Qt::PointingHandCursor);
    connect(m_maxBtn, &QPushButton::clicked, this, &StarryTitleBar::onMaximizeClicked);

    // 7. 关闭按钮
    m_closeBtn = new QPushButton("✕", this);
    m_closeBtn->setObjectName("titleBtnClose");
    m_closeBtn->setToolTip("关闭系统");
    m_closeBtn->setCursor(Qt::PointingHandCursor);
    connect(m_closeBtn, &QPushButton::clicked, this, &StarryTitleBar::onCloseClicked);

    mainLayout->addWidget(m_minBtn);
    mainLayout->addWidget(m_maxBtn);
    mainLayout->addWidget(m_closeBtn);
}

void StarryTitleBar::setTitle(const QString &title) {
    m_titleLabel->setText(title);
}

void StarryTitleBar::setSubTitle(const QString &subTitle) {
    m_subTitleLabel->setText(subTitle);
}

void StarryTitleBar::setBadge(const QString &badge) {
    m_badgeLabel->setText(badge);
}

void StarryTitleBar::updateMaximizeButton(bool isMaximized) {
    if (isMaximized) {
        m_maxBtn->setText("❐");
        m_maxBtn->setToolTip("向下还原窗口");
    } else {
        m_maxBtn->setText("▢");
        m_maxBtn->setToolTip("最大化窗口");
    }
}

void StarryTitleBar::onMinimizeClicked() {
    if (window()) {
        window()->showMinimized();
    }
}

void StarryTitleBar::onMaximizeClicked() {
    if (window()) {
        if (window()->isMaximized()) {
            window()->showNormal();
            updateMaximizeButton(false);
        } else {
            window()->showMaximized();
            updateMaximizeButton(true);
        }
    }
}

void StarryTitleBar::onCloseClicked() {
    if (window()) {
        window()->close();
    }
}

void StarryTitleBar::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    bool isMax = (window() && window()->isMaximized());
    double r = isMax ? 0.0 : 14.0;

    // 1. 绘制带有柔和上圆角的半透明磨砂顶栏
    QPainterPath path;
    path.moveTo(0, height());
    path.lineTo(0, r);
    if (r > 0.0) {
        path.arcTo(0, 0, r * 2, r * 2, 180, -90);
        path.lineTo(width() - r, 0);
        path.arcTo(width() - r * 2, 0, r * 2, r * 2, 90, -90);
    } else {
        path.lineTo(width(), 0);
    }
    path.lineTo(width(), height());
    path.closeSubpath();

    p.fillPath(path, QColor(7, 13, 28, 225));

    // 2. 绘制柔和流光横线 (两端渐隐，柔和微光，不突兀)
    QLinearGradient glowGrad(0, height() - 1, width(), height() - 1);
    glowGrad.setColorAt(0.0, QColor(0, 240, 255, 10));
    glowGrad.setColorAt(0.12, QColor(0, 240, 255, 130));
    glowGrad.setColorAt(0.5, QColor(0, 240, 255, 45));
    glowGrad.setColorAt(0.88, QColor(0, 240, 255, 130));
    glowGrad.setColorAt(1.0, QColor(0, 240, 255, 10));

    p.setPen(QPen(QBrush(glowGrad), 1.0));
    p.drawLine(0, height() - 1, width(), height() - 1);
}

void StarryTitleBar::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (window() && window()->windowHandle()) {
            window()->windowHandle()->startSystemMove();
        }
    }
    QWidget::mousePressEvent(event);
}

void StarryTitleBar::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        onMaximizeClicked();
    }
    QWidget::mouseDoubleClickEvent(event);
}
