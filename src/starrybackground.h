#ifndef STARRYBACKGROUND_H
#define STARRYBACKGROUND_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QColor>
#include <QPointF>

// 单个星星粒子属性
struct StarParticle {
    double x;               // 0.0 ~ 1.0 (归一化坐标)
    double y;               // 0.0 ~ 1.0
    double size;            // 星星半径 (像素)
    double speedX;          // 漂移速度 X
    double speedY;          // 漂移速度 Y
    double brightness;      // 当前亮度 (0.1 ~ 1.0)
    double twinkleSpeed;    // 闪烁频率
    double twinklePhase;    // 闪烁相位
    QColor baseColor;       // 星星基色（纯白、淡青、幽蓝、粉紫）
};

// 流星特效
struct ShootingStar {
    QPointF startPos;
    QPointF currentPos;
    double speed;
    double length;
    double angle;           // 运动角度 (弧度)
    double opacity;         // 透明度
    bool active;
};

class StarryBackground : public QWidget {
    Q_OBJECT

public:
    explicit StarryBackground(QWidget *parent = nullptr);
    ~StarryBackground() override = default;

    void setStarCount(int count);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateAnimation();

private:
    void initStars(int count);
    void spawnMeteor();
    void updateMeteors();

    QVector<StarParticle> m_stars;
    QVector<ShootingStar> m_meteors;
    QTimer *m_timer;
    int m_starCount;
    int m_meteorSpawnCounter;
};

#endif // STARRYBACKGROUND_H
