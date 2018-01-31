#include "qlocalspreadmainwindow.hpp"

#include <QAction>
#include <QToolBar>
#include <QLayout>
#include <QSplitter>

#include "qlocalspreadopenglwidget.hpp"
#include "ui_dlgtrain3d.h"
QLocalSpreadMainWindow::QLocalSpreadMainWindow(QWidget * parent) 
	: QMainWindow(parent), 
	 ui(new Ui_DlgTrain3D)
{
	ui->setupUi(this);
	this->setCentralWidget(ui->centralwidget);
	setWindowFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint);

	this->createWidget();

	this->createAction();

	//connect(localSpreadWidget, SIGNAL(PickWidth(float)), this, SIGNAL(PickWidth(float)));//?

	connect(localSpreadWidget, SIGNAL(PickSectionAngle(float)), this, SIGNAL(PickSectionAngle(float)));

	connect(localSpreadWidget, SIGNAL(PickSectionRadian(float)), this, SIGNAL(PickSectionRadian(float)));
}
void QLocalSpreadMainWindow::createWidget()
{
	this->localSpreadWidget = new QLocalSpreadOpenglWidget(ui->centralwidget);
	this->setCentralWidget(localSpreadWidget);
}
void QLocalSpreadMainWindow::createAction()
{
	actionBigger = new QAction(QIcon(":/images/bigger.png"), "放大");
	connect(actionBigger, SIGNAL(triggered()), localSpreadWidget, SLOT(Bigger()));

	actionSmaller = new QAction(QIcon(":/images/smaller.png"), "缩小");
	connect(actionSmaller, SIGNAL(triggered()), localSpreadWidget, SLOT(Smaller()));

	actionRotate = new QAction(QIcon(":/images/rotate.png"), "旋转");
	actionRotate->setCheckable(true);
	connect(actionRotate, SIGNAL(triggered()), localSpreadWidget, SLOT(Rotate()));
	connect(localSpreadWidget, SIGNAL(RotateSignal(bool)), this, SLOT(updateActionRotate(bool)));

	actionRecovery = new QAction(QIcon(":/images/recoveryDefault.png"), "恢复原图");
	connect(actionRecovery, SIGNAL(triggered()), localSpreadWidget, SLOT(InitCamera()));

	actionSectionPick = new QAction("SectionPickEnable");
	actionSectionPick->setCheckable(true);
	connect(actionSectionPick, SIGNAL(triggered()), localSpreadWidget, SLOT(EnableSectionPick()));
	actionSectionPick->setChecked(true);

	actionCircumferencePick = new QAction("WidthPickEnable");
	actionCircumferencePick->setCheckable(true);
	connect(actionCircumferencePick, SIGNAL(triggered()), localSpreadWidget, SLOT(EnableWidthPick()));
	actionCircumferencePick->setChecked(true);

	actionColorMark = new QAction("ColorMarkVisible");
	actionColorMark->setCheckable(true);
	connect(actionColorMark, SIGNAL(triggered()), localSpreadWidget, SLOT(ColorMarkVisible()));
	actionColorMark->setChecked(true);

	actionCoordinate = new QAction("CoordinateVisible");
	actionCoordinate->setCheckable(true);
	connect(actionCoordinate, SIGNAL(triggered()), localSpreadWidget, SLOT(CoordinateVisible()));
	actionCoordinate->setChecked(true);

	actionRuler = new QAction("RulerVisible");
	actionRuler->setCheckable(true);
	connect(actionRuler, SIGNAL(triggered()), localSpreadWidget, SLOT(RulerVisible()));
	actionRuler->setChecked(true);
	//actionRuler

	ui->toolBar->addAction(actionBigger);
	ui->toolBar->addAction(actionSmaller);
	ui->toolBar->addAction(actionRotate);
	ui->toolBar->addAction(actionRecovery);
	ui->toolBar->addAction(actionSectionPick);
	ui->toolBar->addAction(actionCircumferencePick);
	ui->toolBar->addAction(actionColorMark);
	ui->toolBar->addAction(actionRuler);
	ui->toolBar->addAction(actionCoordinate);
}

QLocalSpreadMainWindow::~QLocalSpreadMainWindow() {
	delete ui;
}

void QLocalSpreadMainWindow::updateActionRotate(bool t)
{
	this->actionRotate->setChecked(t);
}

void QLocalSpreadMainWindow::appendAction(QAction *action, int index)
{
	ui->toolBar->insertAction(ui->toolBar->actions().at(index), action);
}

void QLocalSpreadMainWindow::appendAction(QAction *action)
{
	ui->toolBar->addAction(action);
}

//接收轮子信息
void QLocalSpreadMainWindow::AddWheelSpread(Wheel::WheelSpreadMsg msg)
{
	this->localSpreadWidget->AddWheelSpread(msg);
}

void QLocalSpreadMainWindow::SetCurrentWheel(int id)
{
	this->localSpreadWidget->SetCurrentWheel(id);
}

//void QLocalSpreadMainWindow::SetPickWidth(double width)
//{
//	this->localSpreadWidget->SetPickWidth(width);
//}
void QLocalSpreadMainWindow::SetPickRadian(double radian)
{
	this->localSpreadWidget->SetPickRadian(radian);
}
void QLocalSpreadMainWindow::SetPickAngle(double angle)
{
	this->localSpreadWidget->SetPickAngle(angle);
}
