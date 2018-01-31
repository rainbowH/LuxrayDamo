#pragma once
#include <QMainWindow>

#include "Wheel.h"


class Ui_DlgTrain3D;
class QLocalSpreadOpenglWidget;
class QLocalSpreadMainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit QLocalSpreadMainWindow(QWidget * parent = Q_NULLPTR);
	~QLocalSpreadMainWindow();
	void appendAction(QAction *action, int index);
	void appendAction(QAction *action);

signals:
	//接口函数，输出信号
	void PickSectionRadian(float radian);
	void PickSectionAngle(float angle);//每一个截面
	//void PickWidth(float width);//一圈的数据

private slots:
	//主窗口接收轮子信息
	void AddWheelSpread(Wheel::WheelSpreadMsg msg);
	void SetCurrentWheel(int id);//设定当前要显示的轮子

	//void SetPickWidth(double width);  
	void SetPickRadian(double radian);
	void SetPickAngle(double angle);

	//内部响应
	void updateActionRotate(bool t);
private:
	void createAction();
	void createWidget();

	Ui_DlgTrain3D *ui;
	QLocalSpreadOpenglWidget *localSpreadWidget;
	QAction *actionBigger;
	QAction *actionSmaller;
	QAction *actionRotate;
	QAction *actionRecovery;
	QAction *actionSectionPick;
	QAction *actionCircumferencePick;
	QAction *actionColorMark;
	QAction *actionCoordinate;
	QAction *actionRuler;
	QToolBar *toolBar;
	
};
