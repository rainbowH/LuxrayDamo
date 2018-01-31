#ifndef DLGPOLARCOORD_H
#define DLGPOLARCOORD_H

#include <QMainWindow>
#include <QPolarChart>
#include <QChartView>
#include <QValueAxis>
#include <QLineSeries>

QT_CHARTS_USE_NAMESPACE
namespace Ui {
class DlgPolarCoord;
}

class DlgPolarCoord : public QMainWindow
{
    Q_OBJECT

public:
    explicit DlgPolarCoord(QWidget *parent = 0);
    ~DlgPolarCoord();
    void initView();
    void initToolBar();
    bool isFirstFloor;
    QList<double> rList;
    QList<double>deltarList;
    QList<double> damageList;
    double rMean;

public slots:
    void onChangeAngleRange(double min, double max);
    void onChangeRadiaRange(double min, double max);
    void onChangeTickCounts(int count);

    void onChangeLineAngle(double angle);
    void onPopup();
    void onChangeIsFirst();

    void action_Bigger();
    void action_Smaller();
    void action_antirotate();
    void action_wiserotate();
    void action_exportToPhoto();
    void action_setParas();

    void onSeriesDoubleClicked();
    void onChartWidgetChanged();
    void onShowDamageWidget();
//接收消息
    void onChangeDatas(float position);
signals:
    void angleRangeChanged(double, double);
    void radiaRangeChanged(double, double);
    void damageWidgetshowed();
    void closed();

protected:
    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void on_btnDamageAnalyze_clicked();

    void on_btnAnyShape_clicked();

    void on_btnGraze_clicked();


private:
    Ui::DlgPolarCoord *ui;
    QPolarChart *chart;
    QValueAxis *angularAxis, *radialAxis;
    double axisMin, axisMax, angleMin, angleMax;
    QLineSeries *series1;
    QLineSeries *series2;
    double startAngle;


    void getRList();
};

#endif // DLGPOLARCOORD_H
