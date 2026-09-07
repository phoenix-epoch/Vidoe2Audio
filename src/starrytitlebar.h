#ifndef STARRYTITLEBAR_H
#define STARRYTITLEBAR_H

#include <QWidget>

class QLabel;
class QPushButton;

class StarryTitleBar : public QWidget {
    Q_OBJECT

public:
    explicit StarryTitleBar(QWidget *parent = nullptr);
    ~StarryTitleBar() override = default;

    void setTitle(const QString &title);
    void setSubTitle(const QString &subTitle);
    void setBadge(const QString &badge);
    void updateMaximizeButton(bool isMaximized);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private slots:
    void onMinimizeClicked();
    void onMaximizeClicked();
    void onCloseClicked();

private:
    QLabel *m_iconLabel;
    QLabel *m_titleLabel;
    QLabel *m_subTitleLabel;
    QLabel *m_badgeLabel;

    QPushButton *m_minBtn;
    QPushButton *m_maxBtn;
    QPushButton *m_closeBtn;
};

#endif // STARRYTITLEBAR_H
