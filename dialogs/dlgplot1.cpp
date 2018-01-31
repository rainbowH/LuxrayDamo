#include "dlgplot.h"
#include "ui_dlgplot.h"
#include "wheelsinfothread.h"
#include "arithmetic.h"
#include <QFile>

DlgPlot::DlgPlot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DlgPlot)
{
    ui->setupUi(this);
    qDebug()<<"plot";
    //初始化参数
    initParam();
    //从excel中读取数据
//    WheelsInfoThread * thread = new WheelsInfoThread("E:/Qt/Wheel/车轮踏面不同位置周向数据.xlsx");
//    connect(thread, SIGNAL(sendDatas(QList<QList<QVariant> >)), this, SLOT(drawPlot(QList<QList<QVariant> >)));
//    thread->start();
//    thread->exit();
}

DlgPlot::~DlgPlot()
{
    delete ui;
}


//添加点
void DlgPlot::drawCoordPoint(double x, double y)
{
    ui->plot->addGraph();
    ui->plot->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->plot->graph(1)->setScatterStyle(myScatter);
    ui->plot->graph(1)->addData(x,y);
    ui->plot->replot();
    //将当前点保存
    coordXArr.append(x);
    coordYArr.append(y);
    //绘线及文本
    if(coordXArr.count()==2){
        drawLines(coordXArr,coordYArr,coordXArr.count());
        phaseTracerText = new QCPItemText(ui->plot);
        addTextToVerLine(coordXArr[1],coordYArr[1]);
        coordXArr.clear();
        coordYArr.clear();
    }
}

//初始化
void DlgPlot::initParam()
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
    //构造一个文本
    phaseTracerText = new QCPItemText(ui->plot);
    //设置笔
    pen.setColor(Qt::white);
    pen.setWidth(1);
    //设置放大缩小
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    connect(ui->plot->xAxis,SIGNAL(rangeChanged(QCPRange)),this,SIGNAL(xRangeChanged(QCPRange)));
    connect(ui->plot->yAxis,SIGNAL(rangeChanged(QCPRange)),this,SIGNAL(yRangeChanged(QCPRange)));
    //坐标轴距离
    xmin=0;
    xmax=0;
    ymin=386.00;
    ymax=397.55;

    //设置边框
    setWindowFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint);
}

//判断点是否在曲线上
void DlgPlot::judgePointOnCurve(double x, double y)
{
    //曲线坐标点
    double x_val;
    double y_val;
    //两坐标的距离
    double x_dis;
    double y_dis;
    int i=0;
    for(int j=0;j<xValues.length();j++,i++){
            x_val=xValues[j];
            y_val=yValues[i];
            x_dis=x_val-x;
            y_dis=y_val-y;
            qreal currentDistance=qSqrt(x_dis*x_dis+y_dis*y_dis);
            if(currentDistance<0.1)
            {
               //画点
               drawCoordPoint(x_val,y_val);
            }
    }
}

void DlgPlot::drawLines(QList<double> coordXArr, QList<double> coordYArr,int index)
{
    QList<double> xPoint;
    QList<double> yPoint;

    //画水平线
    xPoint.append(coordXArr[0]);
    yPoint.append(coordYArr[0]);
    xPoint.append(coordXArr[index-1]);
    yPoint.append(coordYArr[0]);
    ui->plot->addGraph();
    ui->plot->graph()->setData(xPoint.toVector(),yPoint.toVector());
    ui->plot->graph()->setPen(pen);

    //画垂直线
    xPoint.clear();
    yPoint.clear();
    xPoint.append(coordXArr[index-1]);
    yPoint.append(coordYArr[0]);
    xPoint.append(coordXArr[index-1]);
    yPoint.append(coordYArr[index-1]);
    ui->plot->addGraph();
    ui->plot->graph()->setData(xPoint.toVector(),yPoint.toVector());
    ui->plot->graph()->setPen(pen);

    ui->plot->replot();
}

//鼠标移动
void DlgPlot::moveMouse(double xpos, double ypos)
{
    //曲线坐标点
    double x_val;
    double y_val;
    //两坐标的距离
    double x_dis;
    double y_dis;
    int i=0;
    for(int j=0;j<xValues.length();j++,i++){
            x_val=xValues[j];
            y_val=yValues[i];
            x_dis=x_val-xpos;
            y_dis=y_val-ypos;
            qreal currentDistance=qSqrt(x_dis*x_dis+y_dis*y_dis);
            if(currentDistance<0.1)
            {
               //画移动线
               moveLines(x_val,y_val);
               addTextToVerLine(x_val,y_val);
            }
    }
}

//画水平和垂直移动的线
void DlgPlot::moveLines(double coordX, double coordY)
{
    QList<double> pos_X;
    QList<double> pos_Y;
    //画上水平线
    pos_X.append(coordXArr[0]);
    pos_Y.append(coordYArr[0]);
    pos_X.append(coordX);
    pos_Y.append(coordYArr[0]);
    ui->plot->addGraph();
    ui->plot->graph(2)->setData(pos_X.toVector(),pos_Y.toVector());
    ui->plot->graph(2)->setPen(pen);

    //画右垂直线
    pos_X.clear();
    pos_Y.clear();
    pos_X.append(coordX);
    pos_Y.append(coordY);
    pos_X.append(coordX);
    pos_Y.append(coordYArr[0]);
    ui->plot->addGraph();
    ui->plot->graph(3)->setData(pos_X.toVector(),pos_Y.toVector());
    ui->plot->graph(3)->setPen(pen);

    ui->plot->replot();
}

//向竖线边添加信息
void DlgPlot::addTextToVerLine(double coordX, double coordY)
{
    //水平距离
    double ver_Pos=coordYArr[0]-(coordYArr[0]-coordY)/2.0;
    phaseTracerText->position->setType(QCPItemPosition::ptPlotCoords);//设置文本坐标解析方式，前文中有提到QCPItemPosition类的PositionType枚举
    phaseTracerText->setPositionAlignment(Qt::AlignRight | Qt::AlignBottom);//设置位置在矩形区域的位置
    phaseTracerText->position->setCoords(coordX,ver_Pos); // 设置位置，注意第三行代码的枚举类型和这儿的值解析方式有关系
    QString disText = QString("%1").arg(qAbs(coordY-coordYArr[0]));
    phaseTracerText->setText(disText);//文本描述
    phaseTracerText->setTextAlignment(Qt::AlignCenter);//设置文本在矩形区域的位置
    phaseTracerText->setFont(QFont(font().family(), 9));//设置文本的字体
    phaseTracerText->setPadding(QMargins(8, 0, 0, 0));//设置文本所在矩形的margins
}

//响应事件
bool DlgPlot::eventFilter(QObject *obj, QEvent *event){

    //获得鼠标点击坐标
    QMouseEvent * mevent = static_cast<QMouseEvent *>(event);
    double x_pos=mevent->x();
    double y_pos=mevent->y();
    //转化为相应坐标轴位置
    double x_val = ui->plot->xAxis->pixelToCoord(x_pos);
    double y_val = ui->plot->yAxis->pixelToCoord(y_pos);

    //鼠标点击事件
    if(event->type() == QEvent::MouseButtonPress)
        {
            judgePointOnCurve(x_val,y_val);
        }
    //鼠标移动事件
    if(event->type()==QEvent::MouseMove){
        if(coordXArr.count()==1){
            moveMouse(x_val,y_val);
        }
    }
    return false;
}

//画出初始的曲线及其坐标
void DlgPlot::drawPlot(QList<QList<QVariant>> datas){

    //设置plot的触发事件
    ui->plot->installEventFilter(this);

    //点的参数
    for(int i=0; i<datas.at(0).length(); i+=2)
        {
            yValues << datas.at(0).at(i).toDouble();
            xValues << i/2;

        }
    //坐标轴
    xmax=xValues.length();
    ui->plot->xAxis->setRange(xmin,xmax);
    ui->plot->yAxis->setRange(ymin,ymax);

    //曲线图层
    ui->plot->addGraph();
    ui->plot->graph(0)->setData(xValues.toVector(), yValues.toVector());
    ui->plot->graph(0)->installEventFilter(this);
    ui->plot->replot();

}

//X轴改变
void DlgPlot::xChangeRange(double xMin, double xMax){

    xmin=xMin;
    xmax=xMax;
    ui->plot->replot();
}

//Y轴改变
void DlgPlot::yChangeRange(double yMin, double yMax){

    ymin=yMin;
    ymax=yMax;
    ui->plot->replot();
}

//放大
void DlgPlot::on_ZoomIn_triggered()
{
    ui->plot->xAxis->scaleRange(0.5,xmin);
    ui->plot->yAxis->scaleRange(0.5,ymin);
    ui->plot->replot();
}

//缩小
void DlgPlot::on_ZoomOut_triggered()
{
    ui->plot->xAxis->scaleRange(1.5,xmin);
    ui->plot->yAxis->scaleRange(1.5,ymin);
    ui->plot->replot();
}

void DlgPlot::drawPlot(QVector<QVector2D> datas)
{
    ui->plot->installEventFilter(this);

    QFile file("./section.txt");
    file.open(QFile::WriteOnly);
    for(int i=0; i< datas.length(); i++)
    {
        QString text = QString("%1\t%2\n").arg(datas.at(i).x()).arg(datas.at(i).y());
        file.write(text.toLatin1());
    }
    file.close();

    xValues.clear();
    yValues.clear();
    for(int i=0; i<datas.length(); i++)
    {
        xValues.append(datas.at(i).x());
        yValues.append(datas.at(i).y());
    }
    removeAllGraph();
    ui->plot->addGraph();
    ui->plot->graph(0)->setData(xValues.toVector(), yValues.toVector());
//    QFile file1("./section.txt");
//    file1.open(QFile::WriteOnly);
//    for(int i=0; i< datas.length(); i++)
//    {
//        QString str = QString("%1\t%2\t").arg(xValues.at(i)).arg(yValues.at(i));
//        file1.write(str.toLatin1());
//    }
//    file1.close();
    xmin = xValues.at(Arithmetic::getMinIndex(xValues));
    xmax = xValues.at(Arithmetic::getMaxIndex(xValues));
    ymin = yValues.at(Arithmetic::getMinIndex(yValues));
    ymax = yValues.at(Arithmetic::getMaxIndex(yValues));

    ui->plot->xAxis->setRange(xmin, xmax);
    ui->plot->yAxis->setRange(ymin, ymax);

    ui->plot->replot();

}

void DlgPlot::removeAllGraph()
{
    while(ui->plot->graphCount() >0)
        ui->plot->removeGraph(0);
}
