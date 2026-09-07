#ifndef STARRYFORMATSDIALOG_H
#define STARRYFORMATSDIALOG_H

#include <QDialog>
#include <QPoint>

class StarryFormatsDialog : public QDialog {
    Q_OBJECT

public:
    explicit StarryFormatsDialog(QWidget *parent = nullptr);
    ~StarryFormatsDialog() override = default;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QWidget *createCategoryCard(const QString &iconAndTitle, const QStringList &tags, const QString &description);
    QWidget *createTag(const QString &text, bool highlight = false);

    QPoint m_dragPos;
};

#endif // STARRYFORMATSDIALOG_H
