#include "diagram.h"
#include <QFont>
Diagram::Diagram(QCustomPlot *Plot)
{
   plot=Plot;
   itemText=new QCPItemText(Plot);
   itemLinefirst=new QCPItemLine(Plot);
   itemLinesecond=new QCPItemLine(Plot);
   itemArrowLine=new QCPItemLine(Plot);

   //绘点
   myScatter.setShape(QCPScatterStyle::ssDisc);
   myScatter.setPen(QPen(Qt::black));
   myScatter.setBrush(QBrush(Qt::white));
   myScatter.setSize(4);

   //graphIndex
   graphIndex=0;

   //字体设置
   Font.setPointSize(10);
   Font.setFamily(("simsun"));

   isSelect=false;
}

//初始化所有点
void Diagram::drawCoordPoints()
{
    plot->addGraph();
    plot->graph(graphIndex)->setLineStyle(QCPGraph::lsNone);
    plot->graph(graphIndex)->setScatterStyle(myScatter);
    plot->graph(graphIndex)->addData(coordPoints.firstPoint.x(),coordPoints.firstPoint.y());
    qDebug() <<coordPoints.firstPoint.x()<<","<<coordPoints.firstPoint.y();
    plot->graph(graphIndex)->addData(coordPoints.secondPoint.x(),coordPoints.secondPoint.y());
    qDebug() <<coordPoints.secondPoint.x()<<","<<coordPoints.secondPoint.y();
    plot->replot();
    qDebug() << "005";
}

//第一条线段
void Diagram::drawFirstLine(Point start, Point end)
{
    itemLinefirst->start->setCoords(start.x(),start.y());
    itemLinefirst->end->setCoords(end.x(),end.y());
    plot->replot();
}

//第二条线段
void Diagram::drawSecondLine(Point start, Point end)
{
    itemLinesecond->start->setCoords(start.x(),start.y());
    itemLinesecond->end->setCoords(end.x(),end.y());
    plot->replot();
}

//带箭头的线段
void Diagram::drawArrowLine(Point start, Point end)
{
    itemArrowLine->start->setCoords(start.x(),start.y());
    itemArrowLine->end->setCoords(end.x(),end.y());
    itemArrowLine->setHead(QCPLineEnding::esSpikeArrow);
    itemArrowLine->setTail(QCPLineEnding::esSpikeArrow);
    plot->replot();
}

//文本表示距离
void Diagram::addTextAroundLine(double x, double y, double interval)
{
    itemText->position->setType(QCPItemPosition::ptPlotCoords);
    itemText->setPositionAlignment(Qt::AlignRight | Qt::AlignBottom);
    itemText->position->setCoords(x, y);
    QString disText = QString("%1").arg(qAbs(interval));
    itemText->setText(disText+"mm");
    itemText->setTextAlignment(Qt::AlignCenter);
    itemText->setFont(Font);//设置文本的字体
    //itemText->setPadding(QMargins(8, 0, 0, 0));//设置文本所在矩形的margins
    plot->replot();
}

//重绘点
void Diagram::reDrawCoordPoint(Point point)
{
    plot->graph(graphIndex)->addData(point.x(),point.y());
    plot->replot();
}

//删除层上的所有点
void Diagram::clearCoordPoints()
{
    plot->graph(graphIndex)->data().data()->clear();
    plot->replot();
}
