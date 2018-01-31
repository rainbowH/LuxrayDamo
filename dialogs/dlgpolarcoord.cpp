#include "dlgpolarcoord.h"
#include "ui_dlgpolarcoord.h"
#include "arithmetic.h"
#include <QAction>
#include <QDebug>
#include <mainwindow.h>
#include <QPaintEvent>
#include <QPainter>
#include <QBoxLayout>
#include <QPixmap>
#include <QImage>
#include <QScreen>
#include "dlgdamageanalyze.h"

DlgPolarCoord::DlgPolarCoord(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DlgPolarCoord)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint);
    ui->grazeTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->grazeTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->grazeTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->grazeTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
//    ui->polarView->installEventFilter(this);
    connect(ui->paintWidget, SIGNAL(signalPolarRange(double,double)), this, SLOT(onChangeRadiaRange(double,double)));
    connect(ui->paintWidget, SIGNAL(signalPolarTickCounts(int)), this, SLOT(onChangeTickCounts(int)));
    connect(ui->paintWidget, SIGNAL(signalPolarAngelRange(double,double)), this, SLOT(onChangeAngleRange(double,double)));
    connect(this, SIGNAL(angleRangeChanged(double,double)), ui->paintWidget, SLOT(onSetAngelRange(double,double)));
//测试代码========
    rMean = 0;
    getRList();
//===============
    initView();
    initToolBar();

    ui->damageWidget->setVisible(false);
    isFirstFloor = true;


    emit angleRangeChanged(angleMin, angleMax);
    emit radiaRangeChanged(axisMin, axisMax);

    if(!MainWindow::wheel->map_rList.isEmpty())
    {
        QList<float> radius = MainWindow::wheel->map_rList.value(70.0).toList();
        double maxD, minD, meanD = 0;
        maxD = radius.at(Arithmetic::getMaxIndex(radius));
        minD = radius.at(Arithmetic::getMinIndex(radius));
        for(int i=0; i<radius.length(); i++)
        {
            meanD += radius.at(i);
        }
        meanD /= radius.length();
    }

//    ui->polarView1->setVisible(false);

//    mainSplitter->setStretchFactor(1, 1);
//    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, 0);
//    ui->paintWidget->setParent(mainSplitter);
//    ui->polarView->setParent(mainSplitter);
//    mainSplitter->setStretchFactor(1,1);
//    this->setCentralWidget(mainSplitter);

//   this->setCentralWidget(ui->polarView);
//    this->setLayout(ui->mainLayout);
    this->centralWidget()->setLayout(ui->mainLayout);
    ui->polarView->setLayout(ui->gridLayout);
//    ui->gridLayout->addWidget(ui->btnDamageAnalyze, 0, 0, 1, 2);
    ui->gridLayout->addWidget(ui->label, 0, 2, 1, 1);
    ui->gridLayout->addWidget(ui->lblR, 0, 3, 1,1);
}

DlgPolarCoord::~DlgPolarCoord()
{
    delete ui;
}

void DlgPolarCoord::wheelEvent(QWheelEvent *event)
{
    if(QApplication::keyboardModifiers() == Qt::ControlModifier && chart->series().length() > 0)
    {
        int delta = event->delta();
        if(delta >0)
            chart->scroll(0, -1);
        else
            chart->scroll(0, 1);
    }
}

void DlgPolarCoord::mouseDoubleClickEvent(QMouseEvent *event)
{
    onPopup();
}

void DlgPolarCoord::closeEvent(QCloseEvent *event)
{
    emit closed();
}


void DlgPolarCoord::initView()
{
    ui->polarView->setBackgroundBrush(QBrush(QColor(142, 186, 240)));
    ui->damageWidget->setLayout(ui->verticalLayout);
//    ui->polarView->setBackgroundBrush(QBrush(Qt::red));
    //定义图层
    series1 = new QLineSeries();
    series1->setBrush(QBrush(QColor(107, 202, 232)));
    series1->setPen(QPen(Qt::red));
    series1->setName("标准车轮切面");

    series2 = new QLineSeries();
    series2->setBrush(QBrush(QColor(173, 202, 232)));
    series2->setPen(QPen(Qt::yellow));

    double step = 360.0/rList.length();
    for(qreal i= 0.0, j=0; i<=360.0; i+= step, j++)
    {
        series1->append(i, rList.at(j));
//        qDebug() << series1->points().at(i).x() << "," << series1->points().at(i).y();
    }

    chart = new QPolarChart();
//    chart->setBackgroundVisible(true);
    chart->addSeries(series1);
    chart->addSeries(series2);

    //legend
    chart->legend()->setVisible(true);
    chart->legend()->detachFromChart();
    chart->legend()->setGeometry(ui->polarView->geometry().width()-60, ui->polarView->y()+ 10,
                                  150, 75);
//    chart->legend()->setAlignment(Qt::AlignRight);

    //轴线
    angularAxis = new QValueAxis();
    angularAxis->setReverse(true);
    connect(angularAxis, SIGNAL(rangeChanged(qreal,qreal)), this, SIGNAL(angleRangeChanged(double,double)));
    QPen pen(Qt::black);
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    angularAxis->setLinePen(pen);
    angularAxis->setTickCount(9);
    angularAxis->setLabelFormat("%d");
    angularAxis->setShadesVisible(false);
//    angularAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
    angularAxis->setGridLinePen(QPen(Qt::black));
    chart->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);

    chart->setBackgroundVisible(false);

    radialAxis = new QValueAxis();
    connect(radialAxis, SIGNAL(rangeChanged(qreal,qreal)), this, SIGNAL(radiaRangeChanged(double,double)));
    QPen linePen(Qt::black);
    linePen.setStyle(Qt::SolidLine);
    radialAxis->setLinePen(linePen);
    QPen GridLinePen(Qt::blue);
    GridLinePen.setStyle(Qt::DashLine);
    radialAxis->setGridLinePen(GridLinePen);
    radialAxis->setTickCount(5);
    radialAxis->setLabelsVisible(false);
//    radialAxis->setLabelFormat("%d");
    chart->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);

//一定要有这两句话，不然画出来的函数搞死都看不懂！！！！！！
    series1->attachAxis(angularAxis);
    series1->attachAxis(radialAxis);

    angleMin = 0.0;
    angleMax = 360.0;
    axisMin = 0.0;
    axisMax = 500.0;

    startAngle = 40.0;
    for(qreal i= axisMin; i<=axisMax; i++)
    {
        series2->append(startAngle, i);
    }
    series2->attachAxis(angularAxis);
    series2->attachAxis(radialAxis);

    angularAxis->setRange(angleMin, angleMax);
    emit angleRangeChanged(angleMin, angleMax);
    radialAxis->setRange(axisMin, axisMax);

    ui->polarView->setChart(chart);


    connect(chart, SIGNAL(widthChanged()), this, SLOT(onChartWidgetChanged()));
}


void DlgPolarCoord::initToolBar()
{
    QAction *actionAntiRotate = new QAction(QIcon(":/images/anticlock.png"), "逆时针旋转");
    connect(actionAntiRotate, SIGNAL(triggered(bool)), this, SLOT(action_antirotate()));
    QAction *actionWiseRotate = new QAction(QIcon(":/images/clockwise.png"), "顺时针旋转");
    connect(actionWiseRotate, SIGNAL(triggered(bool)), this, SLOT(action_wiserotate()));
    QAction *actionDamageAnalyze = new QAction(QIcon(":/images/dataanalyze.png"), "损伤分析");
    connect(actionDamageAnalyze, SIGNAL(triggered(bool)), this, SLOT(onPopup()));
    QAction *actionSavePic = new QAction(QIcon(":/images/exporttophoto.png"), "导出为图片");
    connect(actionSavePic, SIGNAL(triggered(bool)), this, SLOT(action_exportToPhoto()));
    QAction *actionParaSetting = new QAction(QIcon(":/images/test.png"), "参数设置");
    connect(actionParaSetting, SIGNAL(triggered(bool)), this, SLOT(action_setParas()));

    ui->toolBar->addAction(actionAntiRotate);
    ui->toolBar->addAction(actionWiseRotate);
    ui->toolBar->addAction(actionDamageAnalyze);
    ui->toolBar->addAction(actionSavePic);
    ui->toolBar->addAction(actionParaSetting);
}

void DlgPolarCoord::action_Bigger()
{
    chart->scroll(0, -0.5);

}

void DlgPolarCoord::action_Smaller()
{
    chart->scroll(0, 0.5);
}

void DlgPolarCoord::action_wiserotate()
{
//    chart->scroll(-0.5, 0);
    startAngle +=1;

    for(int i=0; i< series2->count(); i++)
    {
        series2->replace(i, startAngle, series2->at(i).y());
    }
    chart->update();
}
//导出为图片
void DlgPolarCoord::action_exportToPhoto()
{
    QScreen *screen = QApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(ui->centralWidget->winId());
    QImage image = pixmap.toImage();

    QString name = QFileDialog::getSaveFileName(this, "保存为", "./", "png图片(*.png);;jpg图片(*jpg)");
    if(name == "")
        return;
    image.save(name);
}

void DlgPolarCoord::action_setParas()
{
    ui->paintWidget->onSettingParas();
}
//双击坐标线
void DlgPolarCoord::onSeriesDoubleClicked()
{
    qDebug() <<"double clicked the series";
}
//chart的width改变
void DlgPolarCoord::onChartWidgetChanged()
{
    chart->setMinimumWidth(chart->rect().height());
}

void DlgPolarCoord::onShowDamageWidget()
{
    ui->damageWidget->setVisible(true);
    ui->mainLayout->removeItem(ui->horizontalSpacer_3);
    ui->mainLayout->removeItem(ui->horizontalSpacer_4);
    ui->mainLayout->addWidget(ui->damageWidget, 4);
    QFont font = ui->label->font();
    font.setPointSize(13);
    ui->label->setFont(font);
    ui->lblR->setFont(font);
}

void DlgPolarCoord::action_antirotate()
{
//    chart->scroll(0.5, 0);

    startAngle -=1;

    for(int i=0; i< series2->count(); i++)
    {
        series2->replace(i, startAngle, series2->at(i).y());
    }
    chart->update();
}

void DlgPolarCoord::onChangeAngleRange(double min, double max)
{
    qDebug() <<"coord recieved angle changed "<<min;
    angleMin = min;
    angleMax = max;
    angularAxis->setRange(angleMin, angleMax);
}

void DlgPolarCoord::onChangeRadiaRange(double min, double max)
{
    axisMin = min;
    axisMax = max;
    radialAxis->setRange(axisMin, axisMax);
}

void DlgPolarCoord::onChangeTickCounts(int count)
{
    radialAxis->setTickCount(count);
}

void DlgPolarCoord::onChangeLineAngle(double angle)
{
    series2->clear();
    for(qreal i=axisMin; i<=axisMax; i+=0.1)
    {
        series2->append(angle, i);
    }
}

void DlgPolarCoord::onPopup()
{
    if(isFirstFloor)
    {
        qDebug() <<"double clicked the series";
        DlgPolarCoord *polar = new DlgPolarCoord();
        memcpy(polar, this, sizeof(this));
        polar->setWindowFlags(Qt::Dialog);
        connect(this, SIGNAL(damageWidgetshowed()), polar, SLOT(onShowDamageWidget()));
        connect(polar, SIGNAL(closed()), this, SLOT(onChangeIsFirst()));
        polar->isFirstFloor = false;
        this->isFirstFloor = false;

        emit damageWidgetshowed();

        polar->showMaximized();
    }
}

void DlgPolarCoord::onChangeIsFirst()
{
    this->isFirstFloor = true;
}

void DlgPolarCoord::onChangeDatas(float position)
{
    QMap<float, QVector<float>> map = MainWindow::wheel->map_rList;
    if(map.isEmpty())
        return;
    QVector<float> rVector = map.value(position);
    float step = 360/rVector.length();
    series1->clear();
    for(float i=0, j=0; i< 360; i+= step, j++)
    {
        series1->append(i, rVector.at(j));
    }
}

void DlgPolarCoord::on_btnDamageAnalyze_clicked()
{
//    int type = Arithmetic::DamageAnalyze(rList, rList.length());
//    DlgDamageAnalyze *dlg = new DlgDamageAnalyze(type);
//    dlg->show();
//    qDebug() << "损伤分析";
}

void DlgPolarCoord::getRList()
{
    QFile file("F:/wheels/数据/01-26/极坐标半径.txt");
    file.open(QFile::ReadOnly);
    QString str = file.readLine();
    while(str.trimmed() != "")
    {
        rList << str.toDouble();
        str = file.readLine();
    }

    for(int i=0; i<rList.length(); i++)
    {
        rMean += rList.at(i);
    }
    rMean /= rList.length();

    for(int i=0; i< rList.length(); i++)
    {
        deltarList << (rList.at(i) - rMean);
    }
    file.close();

//    QFile file2("F:/wheels/数据/01-26/极坐标半径差.txt");
//    file2.open(QFile::WriteOnly);
//    for(int i=0; i< deltarList.length(); i++)
//    {
//        file2.write(QString::number(deltarList.at(i)).toLatin1());
//        file2.write(QString("\n").toLatin1());
//    }
//    file2.close();
}
//多边形分析
void DlgPolarCoord::on_btnAnyShape_clicked()
{
    ui->plot->clearGraphs();
    damageList.clear();
    //界面切换
    ui->verticalLayout->removeWidget(ui->grazeTable);
    ui->grazeTable->setVisible(false);
    ui->verticalLayout->removeWidget(ui->plot);
    ui->verticalLayout->addWidget(ui->plot);
    ui->plot->setVisible(true);

    Arithmetic::DamageAnalyze(deltarList, deltarList.length(), &damageList);
    ui->plot->addGraph();
    QVector<double> xCoord;
    for(int i=0; i<damageList.length(); i++)
        xCoord.append(i);

    ui->plot->graph(0)->setData(xCoord, damageList.toVector());

    ui->plot->xAxis->setRange(0, 50);
    ui->plot->yAxis->setRange(damageList.at(Arithmetic::getMinIndex(damageList)), damageList.at(Arithmetic::getMaxIndex(damageList))+1);
    ui->plot->replot();
}
//擦伤分析
void DlgPolarCoord::on_btnGraze_clicked()
{
    //界面切换
    ui->verticalLayout->removeWidget(ui->grazeTable);
    ui->verticalLayout->removeWidget(ui->plot);
    ui->plot->setVisible(false);
    ui->verticalLayout->addWidget(ui->grazeTable);
    ui->grazeTable->setVisible(true);
    ui->grazeTable->clearContents();

    //擦伤分析
    QList<Graze *> grazeList = Arithmetic::GrazeAnalyze(rList, rMean);
//    for(int i=0; i< grazeList.length(); i++)
//    {
//        Graze *graze = grazeList.at(i);
//        qDebug() << QString("擦伤位置：%1-%2，擦伤深度：%3，擦伤长度：%4")
//                    .arg(graze->positionStart).arg(graze->positionEnd)
//                    .arg(graze->grazeDeep).arg(graze->grazeWide);
//    }
    for(int i=0; i< grazeList.length(); i++)
    {
        Graze *graze = grazeList.at(i);
        ui->grazeTable->setRowCount(grazeList.length());
        ui->grazeTable->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
        ui->grazeTable->setItem(i, 1, new QTableWidgetItem(QString("%1°——%2°").arg(graze->positionStart).arg(graze->positionEnd)));
        ui->grazeTable->setItem(i, 2, new QTableWidgetItem(QString::number(graze->grazeWide)));
        ui->grazeTable->setItem(i, 3, new QTableWidgetItem(QString::number(graze->grazeDeep)));
    }

}
