#include "iconwidget.h"
#include <QPixmap>
#include <QPalette>
#include <QPainter>

IconWidget::IconWidget(QWidget *parent)
    : QWidget(parent)
{
    picName = ":/images/graylight.png";
}

void IconWidget::paint(QString pictureName)
{
    picName = pictureName;
    repaint();
}

void IconWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0,20, 20, QPixmap(picName));
}

