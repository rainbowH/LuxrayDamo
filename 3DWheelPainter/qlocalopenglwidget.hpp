//#ifndef QLOCALOPENGLWIDGET_H
//#define QLOCALOPENGLWIDGET_H
#pragma once
#include <qopenglwidget.h>
#include <qopenglfunctions_3_3_core.h>
#include <qvector2d.h>
#include <glm\glm.hpp>

//#include "EulerAngleCamera.h"
//#include "RoamingCamera.h"
#include "QuaternionsCamera.h"
#include "qbasictimer.h"
#include "Wheel.h"
#include "PickMsg.h"
#include "LocalEngine.h"
#include "ShaderProgram.h"
class GeometryEngine;
class QOpenGLTexture;
class QLocalOpenGLWidget : public QOpenGLWidget,protected QOpenGLFunctions_3_3_Core {
	Q_OBJECT

public:
	explicit QLocalOpenGLWidget(QWidget * parent = Q_NULLPTR);
	~QLocalOpenGLWidget();

	//接口函数
	//添加Wheel，添加后currentId默认为0
	void AddWheel(const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread, const QString type);//添加Wheel，添加后currentId默认为0
	void SetCurrentWheel(int id);//设定当前要显示的轮子
	int GetWheelSize();//获取已经录入的轮子数量
	void ReadTxtMsg();//读取txt测试数据的函数

	//mainwindow
	void SetPickRadian(float radian);
	void SetPickAngle(float angle);
signals:
	//接口函数，输出信号
	void PickSectionMsg(QVector<QVector2D>);//每一个截面
    //void PickWidth(float width);//一圈的数据
    void PickSectionAngle(float angle);
    //void PickSectionRadian(float radian);
    //void PickWidthRange(double min, double max);
	void SpreadWheelOpenGLMsg(Wheel::WheelSpreadMsg msg);
	//按钮响应
	//void BiggerSignal(bool t);
	//void SmallerSignal(bool t);
	void RotateSignal(bool t);
private slots:
   // void SetPickWidth(double width);  //接口函数，设定圈数据线位置

	//按钮响应
	void EnableSectionPick();
    void EnableWidthPick();
    //void CoordinateAxisVisible();
    void ColorMarkVisible();
	void Bigger();
	void Smaller();
	void InitCamera();
	void Rotate();
protected:
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	//void resizeEvent(QResizeEvent *e)override;
	void mouseMoveEvent(QMouseEvent * e)override;
	void timerEvent(QTimerEvent *e)override;
	void keyPressEvent(QKeyEvent * e)override;
	void keyReleaseEvent(QKeyEvent * e)override;
	void wheelEvent(QWheelEvent * e) override;
	//void showEvent(QShowEvent *e) override;
	//void enterEvent(QEvent *event) override;

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
public:
	static const float kYaw;
	static const float kPitch;
	static const float kZoom;
	static const float kR;
	static const float kMouseSensitivity;//鼠标灵敏度
	static const float kMiddleMouseSensitivity;
	static const float kWheelSensitivity;
	static const float kCameraLocalAngle;
private:
	void UpdateWheelFlangeTreadModel(float radian);
    void Pick(int posX,int posY);
	void InitShaderValue();
    void InitShaders();
    void InitCoordinateAxis();
	//void initEngine();

	ShaderProgram shaderProgram_other;//着色器
	ShaderProgram shaderProgram_flange_tread;
	Engine::LocalEngine engine;//存储mesh
	//RoamingCamera camera;
	//EulerAngleCamera camera;
	Camera::QuaternionsCamera camera;
	QBasicTimer timer;
	//GeometryEngine *geometries;

	glm::mat4 view;
	glm::mat4 projection;
	

	//键鼠判断
	bool keyWPressEvent;
	bool keySPressEvent;
	bool keyAPressEvent;
	bool keyDPressEvent;
	float lastX;
	float lastY;
	bool rightButtonDown;
	bool leftButtonDown;
	bool middleButtonDown;
	bool rotateAction;
	bool bigEnable;
	bool smallerEnable;
	bool pickSectionEnable;
	bool pickWidthEnable;
    //bool coordinateAxisVisible;
    bool colorMarkVisible;

	PickMsg pickMsg;

	const unsigned int TIMESPAN=12;

	//const unsigned int SCR_WIDTH = 800;
	//const unsigned int SCR_HEIGHT = 600;
	unsigned int scr_width;
	unsigned int scr_height;

	int currentWheelId;
	std::map<QString, std::vector<glm::vec2>> floatPlants;
	std::vector<Wheel::WheelFlangeTread> wheels;
};
