#include "qworldopenglwidget.hpp"

#include <qevent.h>

#include "LoadModel.h"
#include "FilePath.h"
#include "MeshObject.h"

const float QWorldOpenglWidget::kStartFocus = 1500.f - 15500.0f * 4.0f + 1250.0f;
const float QWorldOpenglWidget::kEndFocus = 1500.0f + 15500.0f + 11250.0f;
const float QWorldOpenglWidget::kFocusYMov = -465.0f - 1747.77f;
const float QWorldOpenglWidget::kFocusZMov = -900.5f;
const float QWorldOpenglWidget::kValueMax = 1000.0;
const float QWorldOpenglWidget::kYaw = -38.75f;
const float QWorldOpenglWidget::kPitch = -17.9f;
const float QWorldOpenglWidget::kZoom = 45.0f;
const float QWorldOpenglWidget::kR = 10.0f;
const float QWorldOpenglWidget::kMouseSensitivity = 0.05f;//鼠标灵敏度
const float QWorldOpenglWidget::kMiddleMouseSensitivity = 0.03f;
const float QWorldOpenglWidget::kWheelSensitivity = 0.005f;
QWorldOpenglWidget::QWorldOpenglWidget(QWidget * parent) 
	: QOpenGLWidget(parent),
	leftButtonDown(false),
	rightButtonDown(false),
	rotateAction(false),
	middleButtonDown(false),
	//bigEnable(true),
	//smallerEnable(true),
	trainBoxVisible(true),
	trainFrameVisible(true),

	keyWPressEvent(false),
	keySPressEvent(false),
	keyAPressEvent(false),
	keyDPressEvent(false)
{
	this->scr_width = this->size().width();
	this->scr_height = this->size().height();

	this->timer.start(TIMESPAN, this);
}

QWorldOpenglWidget::~QWorldOpenglWidget() {
	this->makeCurrent();

	this->doneCurrent();
}

void QWorldOpenglWidget::initializeGL()
{
	initializeOpenGLFunctions();//glad

	glViewport(0, 0, scr_width, scr_height);

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_NORMALIZE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Enable back face culling
	//glEnable(GL_CULL_FACE);

	initShaders();

    initEngine();

	//this->camera.ShowMsg();
	this->InitCamera();
	this->MoveCameraFocus(0);
	//this->camera.setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
}


void QWorldOpenglWidget::resizeGL(int w, int h)
{
	this->scr_width = w;
	this->scr_height = h;
	glViewport(0, 0, w, h);
}
void QWorldOpenglWidget::paintGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//设定清屏所用的颜色
	//glClear(GL_COLOR_BUFFER_BIT);//接受一个缓冲位(Buffer Bit)来指定要清空的缓冲
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram.use();
    //shaderProgram.setVec3("objectColor", 0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	shaderProgram.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shaderProgram.setVec3("lightPos", camera.position);
	shaderProgram.setVec3("viewPos", camera.position);

	//view/projection
	view = camera.GetViewMatrix();//3d摄像机
	shaderProgram.setMat4("view", view);

	projection = glm::perspective(glm::radians(camera.zoom), ((float)this->scr_width / this->scr_height), 0.1f, 100.0f);
	shaderProgram.setMat4("projection", projection);

	if(this->trainBoxVisible)
		engine.DrawTrainBox(shaderProgram);
	if(this->trainFrameVisible)
		engine.DrawTrainFrame(shaderProgram);
	//engine.DrawObjects(shaderProgram);
	engine.DrawWheelGroup(shaderProgram);
}
void QWorldOpenglWidget::initShaders()
{
    shaderProgram.initShaderProgram(FilePath::kVTrainShaderPath,
        FilePath::kFTrainShaderPath);
}
void QWorldOpenglWidget::initEngine()
{
	LoadModel loadModel;

    //Object Arrow(loadModel.loadModel("W:/CarWheel/LuxrayDamo/LuxrayDamo/3DWheelPainter/Resources/Model/Arrow.obj"));

	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));  //不允许 非y轴360度 的翻转
    glm::vec3 lightSkyBlue =glm::vec3(135.0f/255.0f,206.0f/255.0f,250.0f/255.0f);
    Object::MeshObject TrainBox(loadModel.loadModel(std::string(FilePath::kTrainBoxPath)), model,lightSkyBlue);

	model=glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));  //不允许 非y轴360度 的翻转
    glm::vec3 lightSteelBlue=glm::vec3(119.0f/255.0f,136.0f/255.0f,153.0f/255.0f);
	Object::MeshObject TrainFrame(loadModel.loadModel(std::string(FilePath::kTrainFramePath)), model,lightSteelBlue);

    glm::vec3 lightSlateGray=glm::vec3(192.0f/255.0f, 192.0f / 255.0f, 192.0f / 255.0f);
	Object::MeshObject TrainWheel(loadModel.loadModel(std::string(FilePath::kTrainWheelPath)),glm::mat4(1.0f),lightSlateGray);

	engine.SetTrainBox(TrainBox);
	engine.SetTrainFrame(TrainFrame);
	//engine.AddObject(TrainBox);
	//engine.AddObject(TrainFrame);
	engine.SetWheelGroup(TrainWheel);
	//engine.AddObject(TrainWheel);
}


void QWorldOpenglWidget::MoveCameraFocus(int value)
{
	//this->camera.MoveCameraFocus(value);

	float ratio = value / kValueMax;
	float dis = kEndFocus - kStartFocus;

	glm::mat4 scale(1.0f);
	scale = glm::scale(scale, glm::vec3(0.001f, 0.001f, 0.001f));

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(kStartFocus + ratio*dis, kFocusYMov, kFocusZMov));

	glm::mat4 model = scale*translate;

	this->camera.SetFocus(model*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	//glm::translate(glm::mat4(1.0f), glm::vec3(1500.0f + 15500.0f * 2.0f - 1250.0f, -465.0f - 1747.77f, -900.5f));
	//this->camera.SetFocus(focus);
}

void QWorldOpenglWidget::mousePressEvent(QMouseEvent *e)
{
	this->setFocus();

	if (e->button() == Qt::LeftButton)
	{
		if (this->rotateAction)
		{
			setCursor(Qt::ClosedHandCursor);
			this->lastX = e->x();
			this->lastY = e->y();
		}
		else
		{
			int id=engine.RayPick_WheelGroup(e->x(), e->y(), projection, view, camera.position, this->scr_width, this->scr_height);
			if (id != -1)
			{
				emit PickID(id);
			}
		}
		this->leftButtonDown = true;
	}
	if (e->button() == Qt::RightButton)////右键取消
	{
		if (this->rotateAction)
		{
			this->rotateAction = false;
			emit rotateSignal(this->rotateAction);
			setCursor(Qt::ArrowCursor);
		}

		this->lastX = e->x();
		this->lastY = e->y();
		this->rightButtonDown = true;
	}
	if (e->button() == Qt::MiddleButton)
	{
		this->lastX = e->x();
		this->lastY = e->y();
		this->middleButtonDown = true;
	}
}
void QWorldOpenglWidget::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		if (this->rotateAction)
		{
			setCursor(Qt::OpenHandCursor);
		}

		this->leftButtonDown = false;
		//firstMouse = true;
	}
	if (e->button() == Qt::RightButton)
	{
		this->rightButtonDown = false;
	}

	if (e->button() == Qt::MiddleButton)
	{
		this->middleButtonDown = false;
	}
}
void QWorldOpenglWidget::mouseMoveEvent(QMouseEvent * e)
{
	if (rightButtonDown)
	{
		float xoffset = e->x() - this->lastX;
		float yoffset = this->lastY - e->y();

		//std::cout << "xoffset:" << xoffset << " yoffset:" << yoffset << endl;

		this->lastX = e->x();
		this->lastY = e->y();
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	if (leftButtonDown)
	{
		if (rotateAction)
		{
			float xoffset = e->x() - this->lastX;
			float yoffset = this->lastY - e->y();
			this->lastX = e->x();
			this->lastY = e->y();
			camera.ProcessMouseMovement(xoffset, yoffset);
		}
	}
	if (middleButtonDown)
	{
		float xoffset = e->x() - this->lastX;
		float yoffset = this->lastY - e->y();
		this->lastX = e->x();
		this->lastY = e->y();

		camera.ProcessMiddleMouseMovement(xoffset, yoffset);
		//this->camera.ShowMsg();
	}
}
void QWorldOpenglWidget::timerEvent(QTimerEvent *e)
{
	//if (this->keyWPressEvent)
	//	camera.ProcessKeyboard(Camera_Movement::FORWARD, 0.001*TIMESPAN);
	//if(this->keySPressEvent)
	//	camera.ProcessKeyboard(Camera_Movement::BACKWARD, 0.001*TIMESPAN);
	//if(this->keyAPressEvent)
	//	camera.ProcessKeyboard(Camera_Movement::LEFT, 0.001*TIMESPAN);
	//if (this->keyDPressEvent)
	//	camera.ProcessKeyboard(Camera_Movement::RIGHT, 0.001*TIMESPAN);
	this->update();
}
void QWorldOpenglWidget::wheelEvent(QWheelEvent * e)
{
	camera.ProcessMouseScroll(e->delta());
}
void QWorldOpenglWidget::Bigger()
{
	camera.ProcessMouseScroll(120);
	//if (!camera.ProcessMouseScroll(120))
	//{
	//	this->bigEnable = false;
	//	emit biggerSignal(this->bigEnable);
	//}
	//else
	//{
	//	if (!this->smallerEnable)
	//	{
	//		this->smallerEnable = true;
	//		emit smallerSignal(this->smallerEnable);
	//	}
	//}
}
void QWorldOpenglWidget::Smaller()
{
	camera.ProcessMouseScroll(-120);
	//if (!camera.ProcessMouseScroll(-120))
	//{
	//	this->smallerEnable = false;
	//	emit smallerSignal(this->smallerEnable);
	//}
	//else
	//{
	//	if (!this->bigEnable)
	//	{
	//		this->bigEnable = true;
	//		emit biggerSignal(this->bigEnable);
	//	}
	//}
}
void QWorldOpenglWidget::InitCamera()
{
	//camera.InitWorldCamera();
	glm::mat4 scale(1.0f);
	scale = glm::scale(scale, glm::vec3(0.001f, 0.001f, 0.001f));

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(kStartFocus, kFocusYMov, kFocusZMov));

	glm::mat4 model = scale*translate;

	camera.InitCamera(kYaw, kPitch, model*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), kR, kZoom);

	camera.SetMouseSensitivity(kMouseSensitivity);
	camera.SetMiddleMouseSensitivity(kMiddleMouseSensitivity);
	camera.SetWheelSensitivity(kWheelSensitivity);
}
void QWorldOpenglWidget::Rotate()
{
	if (!this->rotateAction)
	{
		this->rotateAction = true;
		setCursor(Qt::OpenHandCursor);
	}
	else  //再按一次关闭
	{
		this->rotateAction = false;
		setCursor(Qt::ArrowCursor);
	}
}


void QWorldOpenglWidget::keyPressEvent(QKeyEvent * e)
{
	if (e->key() == Qt::Key_W)
		this->keyWPressEvent = true;
	if (e->key() == Qt::Key_S)
		this->keySPressEvent = true;
	if (e->key() == Qt::Key_A)
		this->keyAPressEvent = true;
	if (e->key() == Qt::Key_D)
		this->keyDPressEvent = true;
}
void QWorldOpenglWidget::keyReleaseEvent(QKeyEvent * e)
{
	if (e->key() == Qt::Key_W)
		this->keyWPressEvent = false;
	if (e->key() == Qt::Key_S)
		this->keySPressEvent = false;
	if (e->key() == Qt::Key_A)
		this->keyAPressEvent = false;
	if (e->key() == Qt::Key_D)
		this->keyDPressEvent = false;
}

void QWorldOpenglWidget::ActivateWheel(WheelSide side, int id, int outputId)//设定要激活轮子的位置,和轮子被点击时输出的ID
{
	this->engine.ActivateWheel(side, id, outputId);
}
void QWorldOpenglWidget::FreezeWheel(WheelSide side, int id)
{
	this->engine.FreezeWheel(side, id);
}
void QWorldOpenglWidget::FreezeAllWheel()
{
	this->engine.FreezeAllWheel();
}

void QWorldOpenglWidget::TrainBoxVisible()
{
	if (this->trainBoxVisible)
	{
		this->trainBoxVisible = false;
	}
	else
	{
		this->trainBoxVisible = true;

	}
}
void QWorldOpenglWidget::TrainFrameVisible()
{
	if (this->trainFrameVisible)
	{
		this->trainFrameVisible = false;
	}
	else
	{
		this->trainFrameVisible = true;
	}
}
