#ifndef DLGPOLARVIEW_H
#define DLGPOLARVIEW_H

#include <QDialog>
#include <QChartView>
#include <QPolarChart>
#include <QValueAxis>
#include "qcustomplot.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class DlgPolarView;
}

class DlgPolarView : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPolarView(QWidget *parent = 0);
    ~DlgPolarView();
    void initPolarCoord();
    void initPlotCoord();

signals:
    void angleRangeChanged(double, double);
    void radiaRangeChanged(double, double);

    void xRangeChanged(double, double);
    void yRangeChanged(double, double);

    void PickSectionMsg(QVector<QVector2D>);
    void PickWidth(float);

    void widgetEnable(bool);

public slots:
    void onAngleRangeChange(double min, double max);
    void onRadiaRangeChange(double min, double max);

    void onXRangeChange(QCPRange range);
    void onYRangechange(QCPRange range);



private slots:
    void on_Btn_OK_clicked();

    void on_btn_PlotOK_clicked();

private:
    Ui::DlgPolarView *ui;
    double axisMin, axisMax, angleMin, angleMax;
    QValueAxis*radialAxis;
    QValueAxis *angularAxis;
    QPolarChart *chart;
};

#endif // DLGPOLARVIEW_H
