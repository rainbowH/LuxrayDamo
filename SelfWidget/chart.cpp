#include "chart.h"
#include "ui_chart.h"

Chart::Chart(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Chart)
{
    ui->setupUi(this);
    initParameter();
    uploadDatas();
    drawPolt();
    getRequirePoints();
    drawLines();
    drawHintMessage(0.8,1.0,hintMessage);
}

Chart::~Chart()
{
    delete ui;
}

int Chart::getNearValueIndex(double x)
{
    for(int i= 1; i< datas.length()-1; i++)
        {
            if(((datas.at(i).x() - x <= 0) && (datas.at(i+1).x() - x >= 0)) ||
                ((datas.at(i).x() - x>= 0)&& (datas.at(i+1).x() - x<= 0)))
            {
                if(qAbs(datas.at(i).x() - x) < qAbs(datas.at(i+1).x() - x))
                    return i;
                else
                    return i+1;
            }
        }
    return -1;
}

int Chart::getFirstYOffsetIndex(double y)
{
    for(int i= 1; i< datas.length()-1; i++)
        {
            if((datas.at(i).y() - y<= 0)&& (datas.at(i+1).y() - y>= 0))
            {
                if(qAbs(datas.at(i).y() - y) < qAbs(datas.at(i+1).y() - y))
                    return i;
                else
                    return i+1;
            }
        }
    return -1;
}

int Chart::getLastYOffsetIndex(double y)
{
    for(int i= 1; i< datas.length()-1; i++)
        {
            if((datas.at(i).y() - y>= 0)&& (datas.at(i+1).y() - y<= 0))
            {
                if(qAbs(datas.at(i).y() - y) < qAbs(datas.at(i+1).y() - y))
                    return i;
                else
                    return i+1;
            }
        }
    return -1;
}

void Chart::getRequirePoints()
{
    double yMax=0.0;
    double xIndex;
    double xOffIndex;
    double yOffIndex;
    double y2cmIndex;
    for(int i=0;i<datas.length()-1;i++){
        if(yMax<datas.at(i).y()){
            yMax=datas.at(i).y();
            xIndex=i;
        }
    }
    //点的坐标
    pointyMax.setX(datas.at(xIndex).x());
    pointyMax.setY(datas.at(xIndex).y());

    y2cmIndex=getLastYOffsetIndex(pointyMax.y()-lineExtend/5.0);
    if(y2cmIndex!=-1){
        point2cm.setX(datas.at(y2cmIndex).x());
        point2cm.setY(datas.at(y2cmIndex).y());
    }

    xOffIndex=getNearValueIndex(datas.at(0).x()+xoffset);
    if(xoffset!=-1){
    point70.setX(datas.at(xOffIndex).x());
    point70.setY(datas.at(xOffIndex).y());
    }

    yOffIndex=getFirstYOffsetIndex(datas.at(xOffIndex).y()+lineExtend);
    if(yOffIndex!=-1){
    pointa1.setX(datas.at(yOffIndex).x());
    pointa1.setY(datas.at(yOffIndex).y());
    }

    yOffIndex=getLastYOffsetIndex(datas.at(xOffIndex).y()+lineExtend);
    if(yOffIndex!=-1){
    pointa2.setX(datas.at(yOffIndex).x());
    pointa2.setY(datas.at(yOffIndex).y());
    }

    xOffIndex=getNearValueIndex(datas.at(0).x()+xoffset+lineExtend/2);
    if(xoffset!=-1){
    pointD.setX(datas.at(xOffIndex).x());
    pointD.setY(datas.at(xOffIndex).y());
    }
}

void Chart::drawPolt()
{
   //点的参数
    for(int i=0; i<datas.length()-1; i++)
        {
            yValues << datas.at(i).y();
            xValues << datas.at(i).x();

        }
    //坐标轴

    ui->plot->xAxis->setRange(xmin,xmax);
    ui->plot->yAxis->setRange(ymin,ymax);

    //曲线图层
    ui->plot->addGraph();
    ui->plot->graph(0)->setData(xValues.toVector(), yValues.toVector());
    ui->plot->xAxis->setVisible(false);
    ui->plot->yAxis->setVisible(false);
    ui->plot->replot();

}

void Chart::drawLineSegment(Point first, Point second)
{

    QCPItemLine *itemline=new QCPItemLine(ui->plot);

    itemline->start->setCoords(first.x(),first.y());
    itemline->end->setCoords(second.x(),second.y());
    itemline->setHead(QCPLineEnding::esSpikeArrow);
    itemline->setTail(QCPLineEnding::esSpikeArrow);

}

void Chart::drawLineSegmentWithoutArrow(Point first, Point second)
{
    QCPItemLine *itemline=new QCPItemLine(ui->plot);
    itemline->start->setCoords(first.x(),first.y());
    itemline->end->setCoords(second.x(),second.y());

}

void Chart::drawLineSegmentWithEndArrow(Point first, Point second)
{
    QCPItemLine *itemline=new QCPItemLine(ui->plot);
    itemline->setPen(pen);
    itemline->start->setCoords(first.x(),first.y());
    itemline->end->setCoords(second.x(),second.y());
    itemline->setTail(QCPLineEnding::esSpikeArrow);

}

void Chart::drawTextAroundLine(double x,double y, QString name)
{

     QCPItemText *phaseTracerText = new QCPItemText(ui->plot);//构造一个文本
     phaseTracerText->position->setType(QCPItemPosition::ptPlotCoords);
     phaseTracerText->setPositionAlignment(Qt::AlignRight | Qt::AlignBottom);
     phaseTracerText->position->setCoords(x, y);
     phaseTracerText->setText(name);
     phaseTracerText->setTextAlignment(Qt::AlignLeft);
     phaseTracerText->setFont(QFont(font().family(), 18));
     phaseTracerText->setPadding(QMargins(8, 0, 0, 0));//设置文本所在矩形的margins


}

void Chart::drawHintMessage(double x, double y, QString name)
{

    QCPItemText *phaseTracerText = new QCPItemText(ui->plot);//构造一个文本
    phaseTracerText->position->setType(QCPItemPosition::ptAxisRectRatio);
    phaseTracerText->setPositionAlignment(Qt::AlignRight | Qt::AlignBottom);
    phaseTracerText->position->setCoords(x, y);
    phaseTracerText->setText(name);
    phaseTracerText->setTextAlignment(Qt::AlignLeft);
    phaseTracerText->setFont(QFont(font().family(), 14));
    phaseTracerText->setPadding(QMargins(8, 0, 0, 0));//设置文本所在矩形的margins
    ui->plot->replot();
}

void Chart::drawLines()
{
    //D的直线
    Point p1,p2;
    p1.setX(pointD.x());
    p1.setY(pointD.y()-lineExtend/1.5);
    drawLineSegmentWithEndArrow(pointD,p1);
    drawTextAroundLine(pointD.x(),pointD.y()-lineExtend/1.5,"∅D");

    //70的线
    p1.setX(point70.x());
    p1.setY(point70.y()-lineExtend/2);
    drawLineSegmentWithoutArrow(p1,point70);
    p1.setX(point70.x());
    p1.setY(point70.y()-lineExtend/3);
    p2.setX(datas.at(0).x());
    p2.setY(point70.y()-lineExtend/3);
    drawLineSegment(p1,p2);
    drawTextAroundLine(point70.x()-(point70.x()-datas.at(0).x())/2,point70.y()-lineExtend/2.5,"70");
    p1.setX(datas.at(0).x());
    p1.setY(point70.y()-lineExtend/2);
    p2.setX(datas.at(0).x());
    p2.setY(datas.at(0).y()+lineExtend*2);
    drawLineSegmentWithoutArrow(p1,p2);

    //SH线
    p1.setX(point70.x());
    p1.setY(point70.y());
    p2.setX(point70.x()-9*lineExtend);
    p2.setY(point70.y());
    drawLineSegmentWithoutArrow(p1,p2);
    p1.setX(pointyMax.x());
    p1.setY(pointyMax.y());
    p2.setX(point70.x()-9*lineExtend);
    p2.setY(pointyMax.y());
    drawLineSegmentWithoutArrow(p1,p2);
    p1.setX(point70.x()-8.5*lineExtend);
    p1.setY(point70.y());
    p2.setX(point70.x()-8.5*lineExtend);
    p2.setY(pointyMax.y());
    drawLineSegment(p1,p2);
    drawTextAroundLine(point70.x()-8.7*lineExtend,pointyMax.y()-(pointyMax.y()-point70.y())/2.0,"Sh");

    //Sd直线
    p1.setX(pointa2.x());
    p1.setY(pointa2.y());
    p2.setX(pointa2.x());
    p2.setY(datas.at(0).y()+lineExtend*2);
    drawLineSegmentWithoutArrow(p1,p2);
    p1.setX(pointa2.x());
    p1.setY(pointa2.y()+lineExtend*1.7);
    p2.setX(datas.at(0).x());
    p2.setY(pointa2.y()+lineExtend*1.7);
    drawLineSegment(p1,p2);
    drawTextAroundLine(pointa2.x()-(pointa2.x()-datas.at(0).x())/2.0,pointa2.y()+lineExtend*1.8,"Sd");

    //a线
    p1.setX(pointa2.x());
    p1.setY(pointa2.y());
    p2.setX(pointa2.x()-lineExtend/1.5);
    p2.setY(pointa2.y());
    drawLineSegmentWithoutArrow(p1,p2);
    p1.setX(pointa2.x()-lineExtend/2);
    p1.setY(pointa2.y());
    p2.setX(pointa2.x()-lineExtend/2);
    p2.setY(point70.y());
    drawLineSegmentWithoutArrow(p1,p2);
    drawTextAroundLine(pointa2.x()-lineExtend/2,pointa2.y()-(pointa2.y()-point70.y())/2,"a");
    p1.setX(pointa2.x()-lineExtend/2);
    p1.setY(point70.y());
    p2.setX(pointa2.x()-lineExtend/2);
    p2.setY(point70.y()-lineExtend/3.5);
    drawLineSegmentWithEndArrow(p1,p2);
    p1.setX(pointa2.x()-lineExtend/2);
    p1.setY(pointa2.y());
    p2.setX(pointa2.x()-lineExtend/2);
    p2.setY(pointa2.y()+lineExtend/3.5);
    drawLineSegmentWithEndArrow(p1,p2);

    //Qr
    p1.setX(point2cm.x());
    p1.setY(point2cm.y());
    p2.setX(point2cm.x());
    p2.setY(point2cm.y()+lineExtend/3.0);
    drawLineSegmentWithoutArrow(p1,p2);
    p1.setX(point2cm.x());
    p1.setY(point2cm.y()+lineExtend/3.5);
    p2.setX(pointa2.x());
    p2.setY(point2cm.y()+lineExtend/3.5);
    drawLineSegmentWithoutArrow(p1,p2);
    drawTextAroundLine(pointa2.x()-(pointa2.x()-point2cm.x())/2.0,point2cm.y()+lineExtend/3.5,"Qr");
    p1.setX(point2cm.x());
    p1.setY(point2cm.y()+lineExtend/3.5);
    p2.setX(point2cm.x()-lineExtend/2.0);
    p2.setY(point2cm.y()+lineExtend/3.5);
    drawLineSegmentWithEndArrow(p1,p2);
    p1.setX(pointa2.x()+lineExtend/2.0);
    p1.setY(point2cm.y()+lineExtend/3.5);
    p2.setX(pointa2.x());
    p2.setY(point2cm.y()+lineExtend/3.5);
    drawLineSegmentWithEndArrow(p2,p1);

    //2
    p1.setX(point2cm.x());
    p1.setY(point2cm.y());
    p2.setX(point2cm.x()-3*lineExtend);
    p2.setY(point2cm.y());
    drawLineSegmentWithoutArrow(p1,p2);
    p1.setX(point2cm.x()-2.6*lineExtend);
    p1.setY(point2cm.y());
    p2.setX(point2cm.x()-2.6*lineExtend);
    p2.setY(pointyMax.y());
    drawLineSegmentWithoutArrow(p1,p2);
    p1.setX(point2cm.x()-2.6*lineExtend);
    p1.setY(point2cm.y());
    p2.setX(point2cm.x()-2.6*lineExtend);
    p2.setY(point2cm.y()-lineExtend/2.0);
    drawLineSegmentWithEndArrow(p1,p2);
    p1.setX(point2cm.x()-2.6*lineExtend);
    p1.setY(pointyMax.y());
    p2.setX(point2cm.x()-2.6*lineExtend);
    p2.setY(pointyMax.y()+lineExtend/2.0);
    drawLineSegmentWithEndArrow(p1,p2);
    drawTextAroundLine(point2cm.x()-2.6*lineExtend,pointyMax.y()+lineExtend/2.5,"2");

    ui->plot->replot();

}

void Chart::initParameter()
{
    //初始化背景色
    backRole.setColor("skyblue");
    backRole.setStyle(Qt::SolidPattern);
    ui->plot->setBackground(backRole);
    //绘点
    myScatter.setShape(QCPScatterStyle::ssDisc);
    myScatter.setPen(QPen(Qt::red));
    myScatter.setBrush(QBrush(Qt::white));
    myScatter.setSize(4);
    //设置笔
    pen.setColor(Qt::black);
    pen.setWidth(1);
    //坐标轴距离
    xmin=0;
    xmax=180;
    ymin=380.00;
    ymax=425.00;
    xoffset=70;
    lineExtend=10;
    xCoordOffset=30;
    hintMessage="注：1、尺寸a的轮缘厚Sd及Qr测量起点\n\t\t2、尺寸a根据车轮不同踏面类型，其值为10or12cm";
}

void Chart::uploadDatas()
{
    Point point;
    QFile f(qApp->applicationDirPath()+"/section.txt");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Open failed." << qApp->applicationDirPath()<<"/section.txt";
        return ;
    }

    QTextStream txtInput(&f);
    QStringList lineStr;
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine().split("\t");
        point.setX(lineStr.first().toDouble()+xCoordOffset);
        point.setY(lineStr.last().toDouble());
        datas.append(point);
    }

    f.close();
}
