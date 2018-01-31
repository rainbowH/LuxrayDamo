#ifndef DIAGRAM_H
#define DIAGRAM_H
#include "qcustomplot.h"
#include <QObject>
#include "coordpoints.h"
#include <QtGui>
#include <QtCore>
#include <QPainter>
#include <QFont>

class Diagram
{
public:
    //functions
    Diagram(QCustomPlot *Plot);
    void drawCoordPoints();
    void drawFirstLine(Point start,Point end);
    void drawSecondLine(Point start,Point end);
    void drawArrowLine(Point start,Point end);
    void addTextAroundLine(double x,double y,double interval);

    void reDrawCoordPoint(Point point);
    void clearCoordPoints();


    //Variables
    int graphIndex;
    CoordPoints coordPoints;
    QCPItemText *itemText;
    QCustomPlot *plot;
    QCPItemLine *itemLinefirst,*itemLinesecond;  //经过点的两条直线
    QCPItemLine *itemArrowLine;     //带箭头的直线
    QCPScatterStyle myScatter;  //绘点
    QFont Font;
    bool isSelect;

};

#endif // DIAGRAM_H
