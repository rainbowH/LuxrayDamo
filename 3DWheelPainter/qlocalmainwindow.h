#ifndef DLGTRAIN3D_H
#define DLGTRAIN3D_H

#include <QMainWindow>
#include <qvector2d.h>

#include "Wheel.h"
namespace Ui {
class DlgTrain3D;
}
class QAction;
class QLocalOpenGLWidget;
//class QGoToWidget;
class QLocalMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QLocalMainWindow(QWidget *parent = 0);
    ~QLocalMainWindow();
    void appendAction(QAction *action, int index);
    void appendAction(QAction *action);

signals:
    //接口函数，输出信号
    void PickSectionMsg(QVector<QVector2D>);//每一个截面
    //void PickWidth(float width);//一圈的数据
	void SpreadWheelOpenGLMsg(Wheel::WheelSpreadMsg msg);//发给展开窗口的轮子数据
public slots:
	void SetPickRadian(float radian);
	void SetPickAngle(float angle);


	void updateActionRotate(bool t);//按下松开。。
	//void updateActionBigger(bool t);//变灰色。。
	//void updateActionSmaller(bool t);
private:
    void createAction();
    void createWidget();
    Ui::DlgTrain3D *ui;
    QLocalOpenGLWidget *localWidget;
    //QGoToWidget* gotoWidget;
    QAction *actionBigger;
    QAction *actionSmaller;
    QAction *actionRotate;
    QAction *actionRecovery;
    QAction *actionRadianPick;
    QAction *actionCircumferencePick;
  //  QAction *actionCoordinateAxis;
    QAction *actionColorMark;
    QToolBar *toolBar;

	
};

#endif // DLGTRAIN3D_H
