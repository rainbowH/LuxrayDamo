#ifndef SLIDER_H
#define SLIDER_H

#include <QObject>
#include <QWidget>
#include <QSlider>

class Slider : public QSlider
{
public:
    Slider(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *ev);
};

#endif // SLIDER_H
