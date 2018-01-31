#ifndef WHEELICON_H
#define WHEELICON_H

#include <QFrame>

namespace Ui {
class WheelIcon;
}

class WheelIcon : public QFrame
{
    Q_OBJECT

public:
    explicit WheelIcon(QWidget *parent = 0);
    ~WheelIcon();

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    Ui::WheelIcon *ui;
    bool isPressed;
};

#endif // WHEELICON_H
