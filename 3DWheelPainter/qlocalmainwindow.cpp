#include "qlocalmainwindow.h"

#include <QAction>
#include <QToolBar>
#include <QLayout>
#include <QSplitter>

#include "Wheel.h"
#include "ui_dlgtrain3d.h"
#include "qlocalopenglwidget.hpp"
//#include "qgotowidget.h"

QLocalMainWindow::QLocalMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DlgTrain3D)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->centralwidget);
    setWindowFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint);

    this->createWidget();

    this->createAction();

    //connect(localWidget,SIGNAL(PickWidth(float)),this,SIGNAL(PickWidth(float)));//?

    connect(localWidget,SIGNAL(PickSectionMsg(QVector<QVector2D>)),this,SIGNAL(PickSectionMsg(QVector<QVector2D>)));

	connect(localWidget, SIGNAL(SpreadWheelOpenGLMsg(Wheel::WheelSpreadMsg)), this, SIGNAL(SpreadWheelOpenGLMsg(Wheel::WheelSpreadMsg)));

    //QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, 0);
    //localWidget->setParent(mainSplitter);
    //gotoWidget->setParent(mainSplitter);
    //mainSplitter->setStretchFactor(0, 1);
    //this->setCentralWidget(mainSplitter);


}
void QLocalMainWindow::createWidget()
{
    this->localWidget = new QLocalOpenGLWidget(ui->centralwidget);

   /* this->gotoWidget=new QGoToWidget(ui->centralwidget);
    connect(this->gotoWidget,SIGNAL(sectionLocateSpinBoxValueChanged(double)),this->localWidget,SLOT(SetPickAngle(double)));
    connect(this->gotoWidget,SIGNAL(widthLocateSpinBoxValueChanged(double)),this->localWidget,SLOT(SetPickWidth(double)));
    connect(this->localWidget,SIGNAL(PickWidth(float)),this->gotoWidget,SLOT(setWidthLocateValue(float)));
    connect(this->localWidget,SIGNAL(PickSectionAngle(float)),this->gotoWidget,SLOT(setSectionLocateValue(float)));
    connect(this->localWidget,SIGNAL(PickWidthRange(double,double)),this->gotoWidget,SLOT(setWidthLocateRange(double,double)));*/

    QHBoxLayout *mainLayout= new QHBoxLayout;
    mainLayout->addWidget(this->localWidget);
    //mainLayout->addWidget(this->gotoWidget);

    ui->centralwidget->setLayout(mainLayout);
}
void QLocalMainWindow::createAction()
{
    actionBigger = new QAction(QIcon(":/images/bigger.png"), "放大");
    connect(actionBigger, SIGNAL(triggered()), localWidget, SLOT(Bigger()));

    actionSmaller = new QAction(QIcon(":/images/smaller.png"), "缩小");
    connect(actionSmaller, SIGNAL(triggered()), localWidget, SLOT(Smaller()));

    actionRotate = new QAction(QIcon(":/images/rotate.png"), "旋转");
    actionRotate->setCheckable(true);
    connect(actionRotate, SIGNAL(triggered()), localWidget, SLOT(Rotate()));
    connect(localWidget, SIGNAL(RotateSignal(bool)), this, SLOT(updateActionRotate(bool)));

    actionRecovery = new QAction(QIcon(":/images/recoveryDefault.png"), "恢复原图");
    connect(actionRecovery, SIGNAL(triggered()), localWidget, SLOT(InitCamera()));

    actionRadianPick = new QAction("AnglePickEnable");
    actionRadianPick->setCheckable(true);
    connect(actionRadianPick, SIGNAL(triggered()), localWidget, SLOT(EnableSectionPick()));
    actionRadianPick->setChecked(true);

    actionCircumferencePick= new QAction("WidthPickEnable");
    actionCircumferencePick->setCheckable(true);
    connect(actionCircumferencePick, SIGNAL(triggered()), localWidget, SLOT(EnableWidthPick()));
    actionCircumferencePick->setChecked(true);

   /* actionCoordinateAxis=new QAction("CoordinateAxisVisible");
    actionCoordinateAxis->setCheckable(true);
    connect(actionCoordinateAxis, SIGNAL(triggered()), localWidget, SLOT(CoordinateAxisVisible()));
    actionCoordinateAxis->setChecked(true);*/

    actionColorMark=new QAction("ColorMarkVisible");
	actionColorMark->setCheckable(true);
    connect(actionColorMark, SIGNAL(triggered()), localWidget, SLOT(ColorMarkVisible()));
	actionColorMark->setChecked(true);

    //connect(localWidget, SIGNAL(BiggerSignal(bool)), this, SLOT(updateActionBigger(bool)));
    //connect(localWidget, SIGNAL(SmallerSignal(bool)), this, SLOT(updateActionSmaller(bool)));
    //connect(localWidget, SIGNAL(RotateSignal(bool)), this, SLOT(updateActionRotate(bool)));

    ui->toolBar->addAction(actionBigger);
    ui->toolBar->addAction(actionSmaller);
    ui->toolBar->addAction(actionRotate);
    ui->toolBar->addAction(actionRecovery);
    ui->toolBar->addAction(actionRadianPick);
    ui->toolBar->addAction(actionCircumferencePick);
    ui->toolBar->addAction(actionColorMark);
   // ui->toolBar->addAction(actionCoordinateAxis);
}

QLocalMainWindow::~QLocalMainWindow()
{
    delete ui;
}

void QLocalMainWindow::updateActionRotate(bool t)
{
	this->actionRotate->setChecked(t);
}
//void DlgTrain3D::updateActionBigger(bool t)
//{
//	this->actionBigger->setEnabled(t);
//}
//void DlgTrain3D::updateActionSmaller(bool t)
//{
//	this->actionSmaller->setEnabled(t);
//}
void QLocalMainWindow::appendAction(QAction *action , int index)
{
    ui->toolBar->insertAction(ui->toolBar->actions().at(index), action);
}

void QLocalMainWindow::appendAction(QAction *action)
{
    ui->toolBar->addAction(action);
}

void QLocalMainWindow::SetPickRadian(float radian)
{
	this->localWidget->SetPickRadian(radian);
}
void QLocalMainWindow::SetPickAngle(float angle)
{
	this->localWidget->SetPickAngle(angle);
}