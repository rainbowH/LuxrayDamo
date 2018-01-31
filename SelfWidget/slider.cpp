#include "slider.h"
#include <QLabel>
#include <QPainter>
#include <QDebug>

Slider::Slider(QWidget *parent):
    QSlider(parent)
{

}

void Slider::paintEvent(QPaintEvent *ev)
{
    QSlider::paintEvent(ev);
    this->setGeometry(this->geometry().x(), this->geometry().y(), 30, 150);
    QPainter painter(this);
    painter.drawText(QRectF(this->rect().x()+20, this->rect().y(), 20, 30), "11");
    qDebug()<< this->rect().width() << "--"<<this->rect().height();

}
