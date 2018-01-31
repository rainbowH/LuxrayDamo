#include "dlgpolarview.h"
#include "ui_dlgpolarview.h"

#include <QLineSeries>
#include <QDebug>
#include <QWheelEvent>
#include <QSplitter>
#include "dialogs/dlgpolarcoord.h"
#include "dialogs/dlgplot.h"


DlgPolarView::DlgPolarView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPolarView)
{
    ui->setupUi(this);
    qDebug() <<"init polarview";
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);

    ui->panel_chartview->setLayout(ui->panelLayout);
    ui->panel_plot->setLayout(ui->gridLayout);
    ui->panelLayout->addWidget(ui->Btn_OK, 4, 3, 1, 2);
    ui->gridLayout->addWidget(ui->btn_PlotOK, 4, 3, 1, 2);
    connect(this, SIGNAL(PickSectionMsg(QVector<QVector2D>)), ui->plot, SLOT(drawPlot(QVector<QVector2D>)));
    connect(this, SIGNAL(PickWidth(float)), ui->chartView, SLOT(onChangeDatas(float)));
//    ui->plot->setBackground(QBrush(QColor(173, 202, 232)));
    initPolarCoord();
    initPlotCoord();

//    ui->mainLayout->setStretch(0, 2);
//屏幕分割
    QSplitter *mainSplitter = new QSplitter(Qt::Vertical, this);
    QSplitter *topSplitter = new QSplitter(Qt::Horizontal, mainSplitter);
    QSplitter *btmSplitter = new QSplitter(Qt::Horizontal, mainSplitter);
    ui->chartView->setParent(topSplitter);
    ui->panel_chartview->setParent(topSplitter);
    topSplitter->setStretchFactor(0, 3);
    topSplitter->setStretchFactor(1, 1);
    ui->plot->setParent(btmSplitter);
    ui->panel_plot->setParent(btmSplitter);
    btmSplitter->setStretchFactor(0, 3);
    btmSplitter->setStretchFactor(1, 1);

    mainSplitter->setStretchFactor(0,1);
    mainSplitter->setStretchFactor(1,1);

    ui->mainLayout->addWidget(mainSplitter);
    this->setLayout(ui->mainLayout);


   //对输入框进行数字限制
    QRegExp regx("\-?[0-9]+\.?[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx);
    ui->Txt_AngleMax->setValidator(validator);
    ui->Txt_AngleMin->setValidator(validator);
    ui->Txt_AxisMax->setValidator(validator);
    ui->Txt_AxisMin->setValidator(validator);
    ui->line_XMax->setValidator(validator);
    ui->line_XMin->setValidator(validator);
    ui->line_YMax->setValidator(validator);
    ui->line_YMin->setValidator(validator);

}

void DlgPolarView::initPolarCoord()
{
    connect(ui->chartView, SIGNAL(angleRangeChanged(double,double)), this, SLOT(onAngleRangeChange(double,double)));
    connect(ui->chartView, SIGNAL(radiaRangeChanged(double,double)), this, SLOT(onRadiaRangeChange(double,double)));
    connect(this, SIGNAL(angleRangeChanged(double,double)), ui->chartView, SLOT(onChangeAngleRange(double,double)));
    connect(this, SIGNAL(radiaRangeChanged(double,double)), ui->chartView, SLOT(onChangeRadiaRange(double,double)));
}

void DlgPolarView::initPlotCoord()
{
    connect(ui->plot, SIGNAL(xRangeChanged(QCPRange)), this, SLOT(onXRangeChange(QCPRange)));
    connect(ui->plot, SIGNAL(yRangeChanged(QCPRange)), this, SLOT(onYRangechange(QCPRange)));
    connect(this, SIGNAL(xRangeChanged(double,double)), ui->plot, SLOT(xChangeRange(double,double)));
    connect(this, SIGNAL(yRangeChanged(double,double)), ui->plot, SLOT(yChangeRange(double,double)));
}

DlgPolarView::~DlgPolarView()
{
    emit widgetEnable(true);
    delete ui;
}

void DlgPolarView::onAngleRangeChange(double min, double max)
{
    ui->Txt_AngleMin->setText(QString::number(min));
    ui->Txt_AngleMax->setText(QString::number(max));
}

void DlgPolarView::onRadiaRangeChange(double min, double max)
{
    ui->Txt_AxisMin->setText(QString::number(min));
    ui->Txt_AxisMax->setText(QString::number(max));
}

void DlgPolarView::onXRangeChange(QCPRange range)
{
    ui->line_XMin->setText(QString::number(range.lower));
    ui->line_XMax->setText(QString::number(range.upper));
}

void DlgPolarView::onYRangechange(QCPRange range)
{
    ui->line_YMin->setText(QString::number(range.lower));
    ui->line_YMax->setText(QString::number(range.upper));
}


//    鼠标滚轮配合ctrl键
//   else if(event->type() == QEvent::Wheel)
//   {
//       if(QApplication::keyboardModifiers() == Qt::ControlModifier && chart->series().length() > 0)
//       {
//           QWheelEvent *e = static_cast<QWheelEvent *>(event);
//           int delta = e->delta();
//           if(delta >0)
//               chart->scroll(0, -0.5);
//           else
//               chart->scroll(0, 0.5);
//       }
//   }


void DlgPolarView::on_Btn_OK_clicked()
{
    qDebug() << "clicked";
    emit angleRangeChanged(ui->Txt_AngleMin->text().toDouble(), ui->Txt_AngleMax->text().toDouble());
    emit radiaRangeChanged(ui->Txt_AxisMin->text().toDouble(), ui->Txt_AxisMax->text().toDouble());
}

void DlgPolarView::on_btn_PlotOK_clicked()
{
    emit xRangeChanged(ui->line_XMin->text().toDouble(), ui->line_XMax->text().toDouble());
    emit yRangeChanged(ui->line_YMin->text().toDouble(), ui->line_YMax->text().toDouble());
}
