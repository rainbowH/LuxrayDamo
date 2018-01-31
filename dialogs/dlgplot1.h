#ifndef DLGPLOT_H
#define DLGPLOT_H

#include <QMainWindow>
#include <QList>
#include <QtGui>
#include <QtCore>
#include <QPainter>
#include <QToolBar>
#include "qcustomplot.h"

namespace Ui {
class DlgPlot;
}

class DlgPlot : public QMainWindow
{
    Q_OBJECT

public:
    explicit DlgPlot(QWidget *parent = 0);
    ~DlgPlot();
    void drawCoordPoint(double x,double y);
    void initParam();
    void judgePointOnCurve(double x,double y);
    void drawLines(QList<double> coordXArr,QList<double> coordYArr,int index);
    void moveMouse(double xpos,double ypos);
    void moveLines(double coordX,double coordY);
    void addTextToVerLine(double coordX,double coordY);
signals:
    void xRangeChanged(QCPRange);
    void yRangeChanged(QCPRange);


    void pickWidth(float);

public slots:
    void drawPlot(QList<QList<QVariant>> datas);

    void xChangeRange(double xMin, double xMax);
    void yChangeRange(double yMin, double yMax);

    void drawPlot(QVector<QVector2D> datas);
private:
    Ui::DlgPlot *ui;
    //x,y点参数
    QList<double> xValues;
    QList<double> yValues;
    //画刷,画笔
    QBrush backRole;
    QPen pen;
    //获得坐标点
    QList<double> coordX;
    QList<double> coordY;
    //绘点
    QCPScatterStyle myScatter;
    //文本
    QCPItemText *phaseTracerText;
    //保存点坐标
    QList<double> coordXArr;
    QList<double> coordYArr;

    //添加工具栏
    QAction *m_zoomIn;
    QAction *m_ZoomOut;

    //X，Y轴最大最小
    double xmin;
    double ymin;
    double xmax;
    double ymax;

    void removeAllGraph();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
private slots:
    void on_ZoomIn_triggered();
    void on_ZoomOut_triggered();
};

#endif // DLGPLOT_H
