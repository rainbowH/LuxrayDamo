#ifndef CHART_H
#define CHART_H

#include <QMainWindow>
#include <QList>
#include "qcustomplot.h"
#include <QFile>
#include "objects/point.h"
#include <QDebug>
namespace Ui {
class Chart;
}

class Chart : public QMainWindow
{
    Q_OBJECT

public:
    explicit Chart(QWidget *parent = 0);
    ~Chart();
    int getNearValueIndex(double x);
    int getFirstYOffsetIndex(double y);
    int getLastYOffsetIndex(double y);
    void getRequirePoints();
    void drawCoordPoint(Point point);
    void drawPolt();
    void drawLineSegment(Point first,Point Second);
    void drawLineSegmentWithoutArrow(Point first,Point Second);
    void drawLineSegmentWithEndArrow(Point first,Point Second);
    void drawTextAroundLine(double x,double y,QString name);
    void drawHintMessage(double x,double y,QString name);
    void drawLines();
    void initParameter();
    void uploadDatas();

private:
    Ui::Chart *ui;
    //x,y点参数
    QList<double> xValues;
    QList<double> yValues;
    //X，Y轴最大最小
    double xmin;
    double ymin;
    double xmax;
    double ymax;
    double xoffset; //x轴偏移
    //线延伸距离
    double lineExtend;
    //画刷,画笔
    QBrush backRole;
    QPen pen;
    //绘点
    QCPScatterStyle myScatter;
    QList<Point> coordPoints;
    //数据
    QList<Point> datas;
    //点数据
    Point point70;
    Point pointyMax;
    Point pointa1;
    Point pointa2;
    Point point2;
    Point pointD;
    Point point2cm;
    //x轴数据整体平移
    double xCoordOffset;
    QString hintMessage;

};

#endif // CHART_H
