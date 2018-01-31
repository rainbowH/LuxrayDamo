#ifndef TICKS_H
#define TICKS_H

#include <QObject>
#include <QWidget>
#include <QLabel>

class Ticks : public QWidget
{
    Q_OBJECT
public:
    explicit Ticks(QWidget *parent = nullptr, int tickCount= 5, double rangemin=0, double rangemax =500);
    ~Ticks();

signals:
    void signalTickCounts(int);
    void signalRange(double, double);
    void signalAngleRange(double, double);
    void signalPolarTickCounts(int);
    void signalPolarRange(double, double);
    void signalPolarAngelRange(double, double);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
//    void mousePressEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
public slots:
    void onSetTicks(int count);
    void onSetRange(double rangemin, double rangemax);
    void onSetAngelRange(double min, double max);
    void onSettingParas();

private:
    int tickCounts;
    double rangeMin, rangeMax;
    double AngleMin, AngleMax;
    QList<QLabel*> labelList;
    bool pressFlag;

    void removeLabels();
};

#endif // TICKS_H
