#include "dlgplot.h"
#include "ui_dlgplot.h"
#include "arithmetic.h"
#include "wheelsinfothread.h"
#include <QSharedPointer>

DlgPlot::DlgPlot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DlgPlot)
{
    ui->setupUi(this);
    //初始化参数
    initParam();
    ui->plot->setLayout(ui->gridLayout);
    ui->plot->installEventFilter(this);
}

DlgPlot::~DlgPlot()
{
    delete ui;
}

//初始化
void DlgPlot::initParam()
{
    //初始化背景色
    backRole.setColor("skyblue");
    backRole.setStyle(Qt::SolidPattern);
    ui->plot->setBackground(backRole);
    qDebug() <<"初始化背景色OK";
    //绘点
    myScatter.setShape(QCPScatterStyle::ssDisc);
    myScatter.setPen(QPen(Qt::red));
    myScatter.setBrush(QBrush(Qt::white));
    myScatter.setSize(4);
    qDebug() <<"绘点OK";
    //构造一个文本
    phaseTracerText = new QCPItemText(ui->plot);
    //设置笔
    pen.setColor(Qt::black);
    pen.setWidth(1);
    //设置放大缩小
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    connect(ui->plot->xAxis,SIGNAL(rangeChanged(QCPRange)),this,SIGNAL(xRangeChanged(QCPRange)));
    connect(ui->plot->yAxis,SIGNAL(rangeChanged(QCPRange)),this,SIGNAL(yRangeChanged(QCPRange)));
    //坐标轴距离
    xMin=0;
    xMax=150;
    yMin=380.00;
    yMax=415.00;
    ui->plot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag | QCP::iSelectItems);
    //70偏移
    offValue=70.0;
    textOffset=5;
    isCopyDialog=true;
    isSelectPoint=false;

    minradius=0.4;
    yOffValue=12;
    SdLineOut=3.0;

    horShift=0.1;
    isHide=false;

    //设置隐藏tools
    ui->actioneconomyplan->setVisible(false);
    ui->actionplanline->setVisible(false);
    //缩放倍数
    mRangeZoomFactor=0.85;
    //放大缩小次数
    scaleNum=0;
    xCoordOffset=50;
    //点距离判断
    disPoint=40.0;
    disLine=60.0;
    //设置边框
    setWindowFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint);
    //设置移动的直线
    isMoveLine=false;
}

void DlgPlot::removeAllGraph()
{

    while(ui->plot->graphCount() >0)
        ui->plot->removeGraph(0);
    while(ui->plot->itemCount())
            ui->plot->removeItem(0);

}
//获得最近点的索引
int DlgPlot::getNearestValueIndex(double value)
{
    for(int i= 1; i< datas.length()-1; i++)
            {
                if(((datas.at(i).x() - value <= 0) && (datas.at(i+1).x() - value >= 0)) ||
                    ((datas.at(i).x() - value >= 0)&& (datas.at(i+1).x() - value <= 0)))
                {
                    if(qAbs(datas.at(i).x() - value) < qAbs(datas.at(i+1).x() - value))
                        return i;
                    else
                        return i+1;
                }
            }
    return -1;
}

void DlgPlot::newDialog()
{
    if(datas.length()>0){
        if(isCopyDialog)
        {
            this->isCopyDialog = false;
            ui->actionmodify->setEnabled(false);
            DlgPlot *dlgplot = new DlgPlot();
            memcpy(dlgplot, this, sizeof(this));
            dlgplot->setWindowTitle("半径最小的踏面轮廓图");
            dlgplot->setWindowFlags(Qt::Dialog);
            dlgplot->ui->actionmodify->setVisible(false);
            dlgplot->ui->actioneconomyplan->setVisible(true);
            dlgplot->ui->actionplanline->setVisible(true);
            dlgplot->minradius=0.5;
            dlgplot->isCopyDialog = false;
            connect(this,SIGNAL(onDrawPlot(QVector<QVector2D>)),dlgplot,SLOT(drawPlot(QVector<QVector2D>)));
            connect(dlgplot, SIGNAL(closed()), this, SLOT(onRecoveryDialog()));
            emit onDrawPlot(this->datas);
            dlgplot->showMaximized();
        }
    }
}

//响应事件
bool DlgPlot::eventFilter(QObject *obj, QEvent *event){

    //弹出窗口
    if((event->type()==QEvent::MouseButtonDblClick) && (((QMouseEvent*)event)->buttons() &Qt::LeftButton))
    {
        if(isCopyDialog){
            newDialog();
        }
    }
    //鼠标点击事件
    if(event->type() == QEvent::MouseButtonPress)
    {
        //获得鼠标点击坐标
        QMouseEvent * mevent = static_cast<QMouseEvent *>(event);
        double x_pos=mevent->x();
        double y_pos=mevent->y();
        //转化为相应坐标轴位置
        double x_val = ui->plot->xAxis->pixelToCoord(x_pos);
        double y_val = ui->plot->yAxis->pixelToCoord(y_pos);

        if(datas.length() > 0)
        {
            if(!diagram_Sd->itemArrowLine->selected()){
                oldPoint.setX(x_val);
                oldPoint.setY(y_val);
            }

            if(isSelectPoint){
                if(diagram_Sh->isSelect)
                    addPointOnPlot(diagram_Sh);
            }
            else{
                isSelectPoint=judgeIsClickPoint(diagram_Sh,x_val,y_val);
                if(isSelectPoint){
                    diagram_Sh->isSelect=isSelectPoint;
                }
            }
        }
    }

    if((event->type() == QEvent::MouseButtonRelease) || (((QMouseEvent*)event)->button() &Qt::RightButton)){

        if(datas.length() > 0)
        {
            if(diagram_Sd->itemArrowLine->selected()){
                diagram_Sd->itemArrowLine->setSelected(false);

            }
        }
    }
    //鼠标移动事件
    if(event->type()==QEvent::MouseMove){
        //获得鼠标点击坐标
        QMouseEvent * mevent = static_cast<QMouseEvent *>(event);
        double x_pos=mevent->x();
        double y_pos=mevent->y();
        //转化为相应坐标轴位置
        double x_val = ui->plot->xAxis->pixelToCoord(x_pos);
        double y_val = ui->plot->yAxis->pixelToCoord(y_pos);

        if(datas.length() > 0)
        {
            if(diagram_Sd->itemArrowLine->selected()){
                drawMovingDiagramSd(y_val);
                oldPoint.setX(x_val);
                oldPoint.setY(y_val);
            }
            if(isSelectPoint){
                if(diagram_Sh->isSelect)
                    drawMovingDiagram(diagram_Sh,x_val,y_val);
            }
        }
    }
    return false;
}


//X轴改变
void DlgPlot::xChangeRange(double xMin, double xMax){

    this->xMin=xMin;
    this->xMax=xMax;
    ui->plot->replot();

}

//Y轴改变
void DlgPlot::yChangeRange(double yMin, double yMax){

    this->yMin=yMin;
    this->yMax=yMax;
    ui->plot->replot();
}

void DlgPlot::onRecoveryDialog()
{
    this->isCopyDialog = !this->isCopyDialog;
    ui->actionmodify->setEnabled(true);
}

void DlgPlot::acceptComboBoxSelected(QString list)
{
    qDebug()<<"------"<<list;
}

void DlgPlot::isDestroyedplan(QObject* object){


    ui->actionplanline->setEnabled(true);
    qDebug()<<tr("结束");
}

void DlgPlot::isDestroyedeco(QObject* object){


    ui->actioneconomyplan->setEnabled(true);
    qDebug()<<tr("结束");
}

void DlgPlot::drawPlot(QVector<QVector2D> datas)
{
    this->datas = datas;
    if(this->datas.length() <= 0)
        return;

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

    ui->plot->xAxis->setRange(xMin, xMax);
    ui->plot->yAxis->setRange(yMin, yMax);


    drawCoodinate();
    initDiagram();

    drawDiagramSh();

    drawDiagramSd();
    ui->plot->replot();
}
//获得Y最大值索引
int DlgPlot::getYMaxIndex()
{
        double yMax=0.0;
        int index=0;
        for(int i=0;i<datas.length();i++){

            if(yMax<datas.at(i).y()){
                yMax=datas.at(i).y();
                index=i;
            }
        }
        return index;
}

void DlgPlot::closeEvent(QCloseEvent *event)
{
    emit closed();
}

//放大
void DlgPlot::on_ZoomIn_triggered()
{
    ui->plot->xAxis->scaleRange(0.5,xMin);
    ui->plot->yAxis->scaleRange(0.5,yMin);
    ui->plot->replot();
}

//缩小
void DlgPlot::on_ZoomOut_triggered()
{
    ui->plot->xAxis->scaleRange(1.5,xMin);
    ui->plot->yAxis->scaleRange(1.5,yMin);
    ui->plot->replot();
}

//坐标轴
void DlgPlot::drawCoodinate()
{
    //Y70索引
    int index;
    index=getNearestValueIndex(datas.at(0).x()+offValue);
    if(index!=-1){
        yMin=datas.at(index).y()-20;
        yMax=datas.at(index).y()+50;
    }

    //扩张x轴的范围
    for(int i=0;i<datas.length();i++){
        if(xMax<datas.at(i).x()){
            xMax += xCoordOffset;
        }
    }

    QCPAxisTickerFixed *tick=new QCPAxisTickerFixed();
    QSharedPointer<QCPAxisTicker> ticker=QSharedPointer<QCPAxisTicker>(tick);
    tick->setTickStep(10);
    ui->plot->xAxis->setRange(xMin,xMax);
    ui->plot->xAxis->setTicker(ticker);

    tick->setTickStep(5);
    ui->plot->yAxis->setRange(yMin,yMax);
    ui->plot->yAxis->setTicker(ticker);
}
//初始化图表
void DlgPlot::initDiagram()
{
    diagram_Sh=new Diagram(ui->plot);
    diagram_Sh->graphIndex=1;

    diagram_Sd=new Diagram(ui->plot);
    diagram_Sd->graphIndex=2;
}

void DlgPlot::drawDiagramSh()
{
        int yMaxIndex,yOffsetIndex;
       //获得点
       yMaxIndex=getYMaxIndex();
       if(yMaxIndex!=-1){
           diagram_Sh->coordPoints.firstPoint.setX(datas.at(yMaxIndex).x());
           diagram_Sh->coordPoints.firstPoint.setY(datas.at(yMaxIndex).y());
       }

       yOffsetIndex=getNearestValueIndex(datas.at(0).x()+offValue);
       if(yOffsetIndex!=-1){
           diagram_Sh->coordPoints.secondPoint.setX(datas.at(yOffsetIndex).x());
           diagram_Sh->coordPoints.secondPoint.setY(datas.at(yOffsetIndex).y());
       }

       //绘图
       diagram_Sh->drawCoordPoints();
       drawDiagramShLinesAndText();
}

void DlgPlot::drawDiagramSd()
{
        int y70index,firIndex,secondIndex;
        y70index=getNearestValueIndex(datas.at(0).x()+offValue);
        if(y70index!=-1){
            firIndex=getFirstYOffset10or12cm(datas.at(y70index).y()+yOffValue);
            secondIndex=getSecondYOffset10or12cm(datas.at(y70index).y()+yOffValue);
        }
        if(firIndex!=-1){
            diagram_Sd->coordPoints.firstPoint.setX(datas.at(firIndex).x());
            diagram_Sd->coordPoints.firstPoint.setY(datas.at(firIndex).y());
        }
        if(secondIndex!=-1){
            diagram_Sd->coordPoints.secondPoint.setX(datas.at(secondIndex).x());
            diagram_Sd->coordPoints.secondPoint.setY(datas.at(secondIndex).y());
        }

        //绘图
        diagram_Sd->drawCoordPoints();
        drawDiagramSdLinesAndText();
}

void DlgPlot::drawDiagramShLinesAndText()
{
        Point startPoint,endPoint;
        double xMed,yMed,interval;

        startPoint.setX(diagram_Sh->coordPoints.firstPoint.x());
        startPoint.setY(diagram_Sh->coordPoints.firstPoint.y());
        endPoint.setX(diagram_Sh->coordPoints.secondPoint.x());
        endPoint.setY(diagram_Sh->coordPoints.firstPoint.y());
        diagram_Sh->drawFirstLine(startPoint,endPoint);

        startPoint.setX(diagram_Sh->coordPoints.firstPoint.x());
        startPoint.setY(diagram_Sh->coordPoints.secondPoint.y());
        endPoint.setX(diagram_Sh->coordPoints.secondPoint.x());
        endPoint.setY(diagram_Sh->coordPoints.secondPoint.y());
        diagram_Sh->drawSecondLine(startPoint,endPoint);

        xMed=qAbs(diagram_Sh->coordPoints.firstPoint.x()-(diagram_Sh->coordPoints.firstPoint.x()-diagram_Sh->coordPoints.secondPoint.x())/2.0);
        startPoint.setX(xMed);
        startPoint.setY(diagram_Sh->coordPoints.firstPoint.y());
        endPoint.setX(xMed);
        endPoint.setY(diagram_Sh->coordPoints.secondPoint.y());
        diagram_Sh->drawArrowLine(startPoint,endPoint);

        yMed=qAbs(diagram_Sh->coordPoints.firstPoint.y()-(diagram_Sh->coordPoints.firstPoint.y()-diagram_Sh->coordPoints.secondPoint.y())/2.0);
        interval=qAbs(diagram_Sh->coordPoints.firstPoint.y()-diagram_Sh->coordPoints.secondPoint.y());
        diagram_Sh->addTextAroundLine(xMed+textOffset*2.3,yMed,interval);

        ui->plot->replot();
}

void DlgPlot::drawDiagramSdLinesAndText()
{
        Point startPoint,endPoint;
        double xMed,yMed,interval;

        startPoint.setX(diagram_Sd->coordPoints.firstPoint.x());
        startPoint.setY(diagram_Sd->coordPoints.firstPoint.y()+SdLineOut);
        endPoint.setX(diagram_Sd->coordPoints.firstPoint.x());
        endPoint.setY(diagram_Sd->coordPoints.firstPoint.y()-SdLineOut);
        diagram_Sd->drawFirstLine(startPoint,endPoint);

        startPoint.setX(diagram_Sd->coordPoints.secondPoint.x());
        startPoint.setY(diagram_Sd->coordPoints.secondPoint.y()+SdLineOut);
        endPoint.setX(diagram_Sd->coordPoints.secondPoint.x());
        endPoint.setY(diagram_Sd->coordPoints.secondPoint.y()-SdLineOut);
        diagram_Sd->drawSecondLine(startPoint,endPoint);

        startPoint.setX(diagram_Sd->coordPoints.firstPoint.x());
        startPoint.setY(diagram_Sd->coordPoints.firstPoint.y());
        endPoint.setX(diagram_Sd->coordPoints.secondPoint.x());
        endPoint.setY(diagram_Sd->coordPoints.secondPoint.y());
        diagram_Sd->drawArrowLine(startPoint,endPoint);

        xMed=qAbs(diagram_Sd->coordPoints.firstPoint.x()-(diagram_Sd->coordPoints.firstPoint.x()-diagram_Sd->coordPoints.secondPoint.x())/2.0);
        interval=qAbs(diagram_Sd->coordPoints.firstPoint.x()-diagram_Sd->coordPoints.secondPoint.x());
        yMed=diagram_Sd->coordPoints.firstPoint.y();
        diagram_Sd->addTextAroundLine(xMed+textOffset/2.0,yMed-textOffset/3.0,interval);

        ui->plot->replot();
}

int DlgPlot::getFirstYOffset10or12cm(double value)
{
    for(int i=0;i<datas.length();i++){
            if((datas.at(i).y() - value <= 0)&& (datas.at(i+1).y() - value >= 0))
            {
                if(qAbs(datas.at(i).y() - value) < qAbs(datas.at(i+1).y() - value))
                    return i;
                else
                    return i+1;
            }
        }
        return -1;
}

int DlgPlot::getSecondYOffset10or12cm(double value)
{
    for(int i=0;i<datas.length();i++){
            if((datas.at(i).y() - value >= 0)&& (datas.at(i+1).y() - value <= 0))
            {
                if(qAbs(datas.at(i).y() - value) < qAbs(datas.at(i+1).y() - value))
                    return i;
                else
                    return i+1;
            }
        }
    return -1;
}

void DlgPlot::addPointOnPlot(Diagram *diagram)
{
    if(diagram->coordPoints.isFirstPoint){

            diagram->reDrawCoordPoint(diagram->coordPoints.firstPoint);
            diagram->coordPoints.isFirstPoint=false;
        }
        if(diagram->coordPoints.isSecondPoint){

            diagram->reDrawCoordPoint(diagram->coordPoints.secondPoint);

            diagram->coordPoints.isSecondPoint=false;
        }
        diagram->isSelect=false;
        isSelectPoint=false;
}

void DlgPlot::hideMeasureLines()
{
    //隐藏Sh
    diagram_Sh->clearCoordPoints();
    diagram_Sh->itemArrowLine->setVisible(false);
    diagram_Sh->itemLinefirst->setVisible(false);
    diagram_Sh->itemLinesecond->setVisible(false);
    diagram_Sh->itemText->setVisible(false);

    //隐藏Sd
    diagram_Sd->clearCoordPoints();
    diagram_Sd->itemArrowLine->setVisible(false);
    diagram_Sd->itemLinefirst->setVisible(false);
    diagram_Sd->itemLinesecond->setVisible(false);
    diagram_Sd->itemText->setVisible(false);
}

void DlgPlot::showMeasureLines()
{
    //显示Sh
    diagram_Sh->drawCoordPoints();
    diagram_Sh->itemArrowLine->setVisible(true);
    diagram_Sh->itemLinefirst->setVisible(true);
    diagram_Sh->itemLinesecond->setVisible(true);
    diagram_Sh->itemText->setVisible(true);

    //显示Sd
    diagram_Sd->drawCoordPoints();
    diagram_Sd->itemArrowLine->setVisible(true);
    diagram_Sd->itemLinefirst->setVisible(true);
    diagram_Sd->itemLinesecond->setVisible(true);
    diagram_Sd->itemText->setVisible(true);

}
//判断鼠标是否点击到点
bool DlgPlot::judgeIsClickPoint(Diagram *diagram, double x, double y)
{
    double xfirstdis,yfirstdis,xlastdis,ylastdis;
    double firstradius,lastradius;
    xfirstdis=x-diagram->coordPoints.firstPoint.x();
    yfirstdis=y-diagram->coordPoints.firstPoint.y();
    firstradius=qSqrt(xfirstdis*xfirstdis+yfirstdis*yfirstdis);
    //判断距离
    if(firstradius<minradius){
        diagram->coordPoints.isFirstPoint=true;
        return true;
    }

    xlastdis=x-diagram->coordPoints.secondPoint.x();
    ylastdis=y-diagram->coordPoints.secondPoint.y();
    lastradius=qSqrt(xlastdis*xlastdis+ylastdis*ylastdis);
    if(lastradius<minradius){
        diagram->coordPoints.isSecondPoint=true;
        return true;
    }

    return false;
}

void DlgPlot::drawMovingDiagramSd(double y)
{
    //鼠标移动距离
    double mouseMoveDis;
    int index,leftindex,rightindex;
    mouseMoveDis=y-oldPoint.y();
    index=getYMaxIndex();
    if(index!=-1){
        if((diagram_Sd->coordPoints.firstPoint.y()+mouseMoveDis<datas.at(index).y())&&
                (diagram_Sd->coordPoints.firstPoint.y()+mouseMoveDis>=datas.at(0).y())&&(
                    (diagram_Sd->coordPoints.secondPoint.y()+mouseMoveDis<datas.at(index).y())&&
                    (diagram_Sd->coordPoints.secondPoint.y()+mouseMoveDis>=datas.at(0).y()))){
            leftindex=getFirstYOffset10or12cm(diagram_Sd->coordPoints.firstPoint.y()+mouseMoveDis);
            if(leftindex!=-1){
                diagram_Sd->coordPoints.firstPoint.setX(datas.at(leftindex).x());
                diagram_Sd->coordPoints.firstPoint.setY(datas.at(leftindex).y());
            }

            rightindex=getSecondYOffset10or12cm(diagram_Sd->coordPoints.secondPoint.y()+mouseMoveDis);
            if(rightindex!=-1){
                diagram_Sd->coordPoints.secondPoint.setX(datas.at(rightindex).x());
                diagram_Sd->coordPoints.secondPoint.setY(datas.at(rightindex).y());
            }
            diagram_Sd->clearCoordPoints();

            if(qAbs(diagram_Sd->coordPoints.firstPoint.y()-diagram_Sd->coordPoints.secondPoint.y())>horShift)
            {
                leftindex=getFirstYOffset10or12cm(diagram_Sd->coordPoints.secondPoint.y());
                if(leftindex!=-1){
                    diagram_Sd->coordPoints.firstPoint.setX(datas.at(leftindex).x());
                    diagram_Sd->coordPoints.firstPoint.setY(datas.at(leftindex).y());
                }
            }
            diagram_Sd->reDrawCoordPoint(diagram_Sd->coordPoints.firstPoint);
            diagram_Sd->reDrawCoordPoint(diagram_Sd->coordPoints.secondPoint);
            drawDiagramSdLinesAndText();

        }
    }
}

void DlgPlot::drawMovingDiagram(Diagram *diagram, double x, double y)
{
    int index;
    diagram->clearCoordPoints();
    if(diagram->coordPoints.isFirstPoint){

        index=judgeMouseMoveOnCurve(x,y);
        if(index!=-1){
            diagram->coordPoints.firstPoint.setX(datas.at(index).x());
            diagram->coordPoints.firstPoint.setY(datas.at(index).y());
        }
        diagram->reDrawCoordPoint(diagram->coordPoints.secondPoint);
    }

    if(diagram->coordPoints.isSecondPoint){

        index=judgeMouseMoveOnCurve(x,y);
        if(index!=-1){
            diagram->coordPoints.secondPoint.setX(datas.at(index).x());
            diagram->coordPoints.secondPoint.setY(datas.at(index).y());
        }
        diagram->reDrawCoordPoint(diagram->coordPoints.firstPoint);
    }
    if(diagram_Sh==diagram){
        drawDiagramShLinesAndText();
    }
}

int DlgPlot::judgeMouseMoveOnCurve(double x, double y)
{
       //两坐标的距离
       double x_dis;
       double y_dis;

       for(int i=0;i<datas.length();i++){

           x_dis=x-datas.at(i).x();
           y_dis=y-datas.at(i).y();

           qreal Distance=qSqrt(x_dis*x_dis+y_dis*y_dis);
              if(Distance<minradius)
                  return i;
       }
       return -1;
}

void DlgPlot::on_actionmodify_triggered()
{
   newDialog();
}
//显示隐藏计测线
void DlgPlot::on_actionhide_triggered()
{
    if(this->datas.length() > 0){
        if(isHide){
            showMeasureLines();
            QIcon icon(":/images/hide.png");
            ui->actionhide->setIcon(icon);
            isHide=false;
        }else{
            hideMeasureLines();
            QIcon icon(":/images/show.png");
            ui->actionhide->setIcon(icon);
            isHide=true;
        }
        ui->plot->replot();
    }
}
//输出图表
void DlgPlot::on_actionsaveDiagram_triggered()
{
    QString filename=QFileDialog::getSaveFileName(this,tr("输出图表"),tr("demo.png"));
    if(!filename.isEmpty()){
        if(ui->plot->savePng(filename,ui->plot->width(),ui->plot->height())){
            qDebug()<<tr("保存成功");
        }else{
            qDebug()<<tr("保存失败");
        }
    }
}
//经济旋
void DlgPlot::on_actioneconomyplan_triggered()
{
    ui->actioneconomyplan->setEnabled(false);
    QList<QString> list={"经济镟1","经济镟2","经济镟3","经济镟4"};
    DlgLathe *dlglathe=new DlgLathe(list);
    dlglathe->setWindowTitle("经济镟");
    dlglathe->show();
    connect(dlglathe,SIGNAL(isDestroyed(QObject*)),this,SLOT(isDestroyedeco(QObject*)));
    connect(dlglathe,SIGNAL(comboBoxSelected(QString)),this,SLOT(acceptComboBoxSelected(QString)));
    connect(dlglathe,SIGNAL(destroyed(QObject*)),this,SLOT(isDestroyedeco(QObject*)));
}
//计划旋
void DlgPlot::on_actionplanline_triggered()
{
    ui->actionplanline->setEnabled(false);
    QList<QString> list={"计划镟1","计划镟2","计划镟3","计划镟4"};
    DlgLathe *dlglathe=new DlgLathe(list);
    dlglathe->setWindowTitle("计划镟");
    dlglathe->show();
    connect(dlglathe,SIGNAL(comboBoxSelected(QString)),this,SLOT(acceptComboBoxSelected(QString)));
    connect(dlglathe,SIGNAL(isDestroyed(QObject*)),this,SLOT(isDestroyedplan(QObject*)));
    connect(dlglathe,SIGNAL(destroyed(QObject*)),this,SLOT(isDestroyedplan(QObject*)));
}
