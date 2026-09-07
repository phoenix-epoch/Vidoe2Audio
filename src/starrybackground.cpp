#include "starrybackground.h"
#include <QPainter>
#include <QPainterPath>
#include <QRadialGradient>
#include <QLinearGradient>
#include <QRandomGenerator>
#include <cmath>

StarryBackground::StarryBackground(QWidget *parent)
    : QWidget(parent),
      m_starCount(160),
      m_meteorSpawnCounter(0)
{
    setAttribute(Qt::WA_StyledBackground, false);
    setAttribute(Qt::WA_OpaquePaintEvent, false);

    initStars(m_starCount);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &StarryBackground::updateAnimation);
    m_timer->start(33); // ~30 FPS 平滑且轻量
}

void StarryBackground::setStarCount(int count) {
    m_starCount = count;
    initStars(m_starCount);
    update();
}

void StarryBackground::initStars(int count) {
    m_stars.clear();
    m_stars.reserve(count);

    QRandomGenerator *rng = QRandomGenerator::global();
    const QVector<QColor> starPalette = {
        QColor(255, 255, 255),       // 纯白
        QColor(200, 245, 255),       // 极光青白
        QColor(160, 210, 255),       // 幽邃蓝
        QColor(220, 190, 255),       // 量子紫
        QColor(255, 240, 180)        // 金光微星
    };

    for (int i = 0; i < count; ++i) {
        StarParticle p;
        p.x = rng->generateDouble();
        p.y = rng->generateDouble();
        p.size = 0.8 + rng->generateDouble() * 2.2; // 0.8 ~ 3.0 px
        p.speedX = (rng->generateDouble() - 0.5) * 0.00015;
        p.speedY = (rng->generateDouble() - 0.5) * 0.00015;
        p.brightness = 0.2 + rng->generateDouble() * 0.8;
        p.twinkleSpeed = 0.03 + rng->generateDouble() * 0.07;
        p.twinklePhase = rng->generateDouble() * 6.28;
        p.baseColor = starPalette[rng->bounded(starPalette.size())];
        m_stars.append(p);
    }
}

void StarryBackground::spawnMeteor() {
    QRandomGenerator *rng = QRandomGenerator::global();
    ShootingStar s;
    double startX = rng->bounded(width() * 0.8);
    double startY = rng->bounded(height() * 0.4);
    s.startPos = QPointF(startX, startY);
    s.currentPos = s.startPos;
    s.speed = 8.0 + rng->generateDouble() * 8.0;
    s.length = 60.0 + rng->generateDouble() * 90.0;
    s.angle = 0.65 + rng->generateDouble() * 0.25; // 约 35~50 度角划过
    s.opacity = 1.0;
    s.active = true;
    m_meteors.append(s);
}

void StarryBackground::updateMeteors() {
    for (int i = m_meteors.size() - 1; i >= 0; --i) {
        ShootingStar &s = m_meteors[i];
        if (!s.active) {
            m_meteors.removeAt(i);
            continue;
        }

        s.currentPos.setX(s.currentPos.x() + std::cos(s.angle) * s.speed);
        s.currentPos.setY(s.currentPos.y() + std::sin(s.angle) * s.speed);
        s.opacity -= 0.025;

        if (s.opacity <= 0.0 || s.currentPos.x() > width() || s.currentPos.y() > height()) {
            s.active = false;
        }
    }
}

void StarryBackground::updateAnimation() {
    // 更新星星漂移与闪烁
    for (StarParticle &p : m_stars) {
        p.x += p.speedX;
        p.y += p.speedY;

        // 环绕边界
        if (p.x < 0.0) p.x += 1.0;
        if (p.x > 1.0) p.x -= 1.0;
        if (p.y < 0.0) p.y += 1.0;
        if (p.y > 1.0) p.y -= 1.0;

        p.twinklePhase += p.twinkleSpeed;
        if (p.twinklePhase > 6.2831853) {
            p.twinklePhase -= 6.2831853;
        }
    }

    // 随机产生流星
    m_meteorSpawnCounter++;
    if (m_meteorSpawnCounter > 90) { // 大约每3秒检查一次
        if (QRandomGenerator::global()->generateDouble() < 0.35) {
            spawnMeteor();
            m_meteorSpawnCounter = 0;
        }
    }

    updateMeteors();
    update();
}

void StarryBackground::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
}

void StarryBackground::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int w = width();
    int h = height();

    bool isMax = (window() && window()->isMaximized());
    double r = isMax ? 0.0 : 14.0;

    QPainterPath roundPath;
    roundPath.addRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), r, r);

    // 1. 开启抗锯齿圆角裁切并渲染深邃宇宙背景渐变
    painter.save();
    painter.setClipPath(roundPath);

    QLinearGradient bgGrad(0, 0, w, h);
    bgGrad.setColorAt(0.0, QColor(6, 10, 22));      // 顶部黑夜
    bgGrad.setColorAt(0.5, QColor(10, 18, 38));     // 中部幽邃星云蓝
    bgGrad.setColorAt(1.0, QColor(4, 7, 16));       // 底部深空
    painter.fillPath(roundPath, bgGrad);

    // 2. 渲染柔和星云光晕 (Nebula Glow)
    QRadialGradient nebula1(w * 0.2, h * 0.3, w * 0.45);
    nebula1.setColorAt(0.0, QColor(0, 160, 255, 30));
    nebula1.setColorAt(0.6, QColor(80, 0, 180, 15));
    nebula1.setColorAt(1.0, QColor(0, 0, 0, 0));
    painter.fillRect(rect(), nebula1);

    QRadialGradient nebula2(w * 0.8, h * 0.7, w * 0.5);
    nebula2.setColorAt(0.0, QColor(140, 20, 220, 25));
    nebula2.setColorAt(0.5, QColor(0, 220, 255, 12));
    nebula2.setColorAt(1.0, QColor(0, 0, 0, 0));
    painter.fillRect(rect(), nebula2);

    // 3. 渲染星星粒子与星芒光晕
    for (const StarParticle &p : m_stars) {
        double px = p.x * w;
        double py = p.y * h;
        double dynamicAlpha = 0.3 + 0.7 * (0.5 + 0.5 * std::sin(p.twinklePhase));
        int alpha = qBound(20, static_cast<int>(255 * dynamicAlpha * p.brightness), 255);

        QColor drawColor = p.baseColor;
        drawColor.setAlpha(alpha);

        // 如果是大星星，画一层外光晕
        if (p.size > 2.0) {
            QRadialGradient halo(px, py, p.size * 2.8);
            QColor haloColor = drawColor;
            haloColor.setAlpha(alpha / 4);
            halo.setColorAt(0.0, haloColor);
            halo.setColorAt(1.0, QColor(0, 0, 0, 0));
            painter.setBrush(halo);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(QPointF(px, py), p.size * 2.8, p.size * 2.8);
        }

        painter.setBrush(drawColor);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPointF(px, py), p.size, p.size);
    }

    // 4. 渲染流星 (Shooting Stars)
    for (const ShootingStar &s : m_meteors) {
        if (!s.active) continue;

        double tailX = s.currentPos.x() - std::cos(s.angle) * s.length;
        double tailY = s.currentPos.y() - std::sin(s.angle) * s.length;

        QLinearGradient meteorGrad(s.currentPos, QPointF(tailX, tailY));
        QColor headColor(255, 255, 255, static_cast<int>(255 * s.opacity));
        QColor midColor(0, 240, 255, static_cast<int>(200 * s.opacity));
        QColor tailColor(0, 150, 255, 0);

        meteorGrad.setColorAt(0.0, headColor);
        meteorGrad.setColorAt(0.3, midColor);
        meteorGrad.setColorAt(1.0, tailColor);

        QPen meteorPen(QBrush(meteorGrad), 2.0);
        meteorPen.setCapStyle(Qt::RoundCap);
        painter.setPen(meteorPen);
        painter.drawLine(s.currentPos, QPointF(tailX, tailY));

        // 流星头部亮点
        painter.setBrush(headColor);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(s.currentPos, 2.0, 2.0);
    }

    painter.restore(); // 恢复裁切

    // 5. 渲染双层极光柔和科技感微光边框 (Soft Aurora Luminous Border)
    if (!isMax) {
        // 外层：柔和极光微晕发光层 (Soft Aura)
        QPen softAuraPen(QColor(0, 240, 255, 28), 2.6);
        softAuraPen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(softAuraPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(QRectF(rect()).adjusted(1.2, 1.2, -1.2, -1.2), r - 1.0, r - 1.0);

        // 内层：主科技青蓝精细线条 (Crisp Neon Core)
        QPen corePen(QColor(0, 240, 255, 85), 1.0);
        corePen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(corePen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), r, r);
    } else {
        QPen borderPen(QColor(0, 240, 255, 50), 1.0);
        painter.setPen(borderPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(rect().adjusted(0, 0, -1, -1));
    }
}
