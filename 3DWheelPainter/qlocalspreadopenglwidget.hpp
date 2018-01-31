#pragma once
#include <qopenglwidget.h>
#include <qopenglfunctions_3_3_core.h>
#include <glm\glm.hpp>
#include <qbasictimer.h>

#include "Wheel.h"
#include "PickMsg.h"
#include "LocalSpreadEngine.h"
#include "EulerAngleCamera.h"
#include "ShaderProgram.h"
//#include "RoamingCamera.h"
//#include "QuaternionsCamera.h"

class QLocalSpreadOpenglWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
	Q_OBJECT

public:
	explicit QLocalSpreadOpenglWidget(QWidget * parent = Q_NULLPTR);
	~QLocalSpreadOpenglWidget();

	//拾取联动信号
	//void SetPickWidth(double width);  //接口函数，设定圈数据线位置
	void SetPickRadian(double radian);
	void SetPickAngle(double angle);

	//轮子的添加和切换
	void AddWheelSpread(Wheel::WheelSpreadMsg spmsg);
	void SetCurrentWheel(int id);//设定当前要显示的轮子
signals:
	//拾取联动信号
	void PickSectionRadian(float radian);//每一个截面
	//void PickWidth(float width);//一圈的数据
	void PickSectionAngle(float angle);

	//按钮响应
	void RotateSignal(bool t);
private slots:

	//按钮响应
	void CoordinateVisible();
	void RulerVisible();
	void ColorMarkVisible();
	void EnableSectionPick();
	void EnableWidthPick();
	void Bigger();
	void Smaller();
	void InitCamera();
	void Rotate();
protected:
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	void mouseMoveEvent(QMouseEvent * e)override;
	void timerEvent(QTimerEvent *e)override;
	void keyPressEvent(QKeyEvent * e)override;
	void keyReleaseEvent(QKeyEvent * e)override;
	void wheelEvent(QWheelEvent * e) override;
	//void showEvent(QShowEvent *e) override;
	void paintEvent(QPaintEvent *e) override;

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
private:
	void Draw2DText(QPainter& painter);
	void DrawOpengl();

	void LoadFontTexture();
	void LoadExampleMsg();
	void Pick(int posX, int posY);
	void InitShaderValue();
	void InitShaders();
	void InitRuler();
	void InitCartesianCoordinate();

	ShaderProgram wheel_spread_shader_program;//着色器
	ShaderProgram line_shader_program;//
	ShaderProgram font_shader_program;//

	Engine::LocalSpreadEngine engine;//存储mesh
	//RoamingCamera camera;
	Camera::EulerAngleCamera camera;
	//QuaternionsCamera camera;
	QBasicTimer timer;

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
	bool coordinate_visible;
	bool ruler_visible;

	PickMsg pickMsg;

	const unsigned int TIMESPAN = 12;

	//const unsigned int SCR_WIDTH = 800;
	//const unsigned int SCR_HEIGHT = 600;
	unsigned int scr_width;
	unsigned int scr_height;

	int currentWheelId;
	std::vector<Wheel::WheelSpreadParm> wheelSpreadParm;

	//font_texture
	Texture font_texture;
	//unsigned int font_texture;

	//test
	Object::LineObject coordinate_line;
};