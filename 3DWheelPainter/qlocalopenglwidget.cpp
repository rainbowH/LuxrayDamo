#include "qlocalopenglwidget.hpp"

#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <QOpenGLTexture>
#include <qevent.h>
#include <glm/gtc/matrix_transform.hpp>
#include <qmessagebox.h>

#include "LoadModel.h"
#include "Mesh.h"
#include "Vertex.h"
#include "FilePath.h"

const float QLocalOpenGLWidget::kYaw = -160.0f;
const float QLocalOpenGLWidget::kPitch = 0.0f;
const float QLocalOpenGLWidget::kZoom = 45.0f;
const float QLocalOpenGLWidget::kR = 10.0f;
const float QLocalOpenGLWidget::kMouseSensitivity = 0.005f;//鼠标灵敏度
const float QLocalOpenGLWidget::kMiddleMouseSensitivity = 0.03f;
const float QLocalOpenGLWidget::kWheelSensitivity = 0.01f;
const float QLocalOpenGLWidget::kCameraLocalAngle = -90.0f;

QLocalOpenGLWidget::QLocalOpenGLWidget(QWidget * parent)
	: QOpenGLWidget(parent), 
	leftButtonDown(false),
	rightButtonDown(false),
	rotateAction(false),
	middleButtonDown(false),
	bigEnable(true),
	smallerEnable(true),
	pickSectionEnable(true),
	pickWidthEnable(true),
	currentWheelId(-1),
	keyWPressEvent(false),
	keySPressEvent (false),
	keyAPressEvent(false),
    keyDPressEvent(false),
    pickMsg(-1.0f,70.0f),
    //coordinateAxisVisible(true),
	colorMarkVisible(true)
{	

	this->scr_width = this->size().width();
	this->scr_height = this->size().height();
		
	//this->startTimer(TIMESPAN);//ms
	this->timer.start(TIMESPAN, this);
}

QLocalOpenGLWidget::~QLocalOpenGLWidget() { //这个函数要修改上下文，所以需要makeCurrent
	this->makeCurrent();

	this->doneCurrent();
}
void QLocalOpenGLWidget::InitShaders()
{
	 //shaderProgram_other;//着色器
	 //shaderProgram_flange_tread;
	shaderProgram_other.initShaderProgram(FilePath::kVWheelOtherShaderPath,
       FilePath::kFWheelOtherShaderPath);
	shaderProgram_flange_tread.initShaderProgram(FilePath::kVWheelFlangeTreadShaderPath,
		FilePath::kFWheelFlangeTreadShaderPath);
}
void QLocalOpenGLWidget::initializeGL()
{
	initializeOpenGLFunctions();//glad

	glViewport(0, 0, scr_width, scr_height);

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	// Enable back face culling
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    InitShaders();
	
	//initEngine();
	//timer.start(12, this);

	//this->camera.SetDisToFocus(15.0f);
	this->InitCamera();
    //InitCoordinateAxis();

    ReadTxtMsg();//他调用了doneCurrent函数！不能放在前面！

	InitShaderValue();
	//this->camera.setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
}
void QLocalOpenGLWidget::InitShaderValue()
{
	this->makeCurrent();
	//shaderProgram_other.setFloat("pickRadian", pickMsg.GetRadian());
	//shaderProgram_other.setFloat("pickWidth", pickMsg.GetWidth());
	//haderProgram_other.setBool("redMark",this->redMarkVisible);
	shaderProgram_flange_tread.use();
	shaderProgram_flange_tread.setFloat("colorMark", this->colorMarkVisible);
	//shaderProgram_flange_tread.setFloat("pickRadian", pickMsg.GetRadian());
	//shaderProgram_flange_tread.setFloat("pickWidth", pickMsg.GetWidth());

	this->doneCurrent();
}
void QLocalOpenGLWidget::InitCoordinateAxis()
{
    //std::cout<<"1"<<std::endl;
    LoadModel loadModel;
    Object::MeshObject Arrow(loadModel.loadModel(std::string(FilePath::kArrowPath)));
	Object::MeshObject Line(loadModel.loadModel(std::string(FilePath::kLinePath)));
    //std::cout<<"2"<<std::endl;
    this->engine.SetCoordinateAxis(Arrow,Line);
    //std::cout<<"3"<<std::endl;
}
void QLocalOpenGLWidget::ReadTxtMsg()
{
	QMap<float, QVector<float>> surveyTread;

	std::ifstream file;
    file.open(FilePath::kSurveyTreadPath);
	if (!file.is_open()) {
		QString error = "ERROR::QLocalOpenGLWidget::ReadTxtMsg::UNABLE TO OPEN FILE Tread";
		QMessageBox::warning(NULL, "ERROR!", error, QMessageBox::Ok);
	}
	float y;
	QVector<float> circlePoint;
	float startX = 50.0f;
	float cicleSize = 2335;
	int i = 0;
	int j = 0;
	while (file >> y)
	{
		circlePoint.push_back(y);
		++i;
		if (i % 2335 == 0)
		{
			surveyTread.insert(startX+0.1f*j,circlePoint);
			circlePoint.clear();
			++j;
		}
	}
	file.close();

	QVector<QVector2D> surveyFlange;
    file.open(FilePath::kSurveyFlangePath);
	if (!file.is_open()) {
		QString error = "ERROR::QLocalOpenGLWidget::ReadTxtMsg::UNABLE TO OPEN FILE Flange";
		QMessageBox::warning(NULL, "ERROR!", error, QMessageBox::Ok);
	}
	float x;
	while (file >>x >> y)
	{
		surveyFlange.push_back(QVector2D(x,y));
	}
	file.close();

	this->AddWheel(surveyFlange, surveyTread, ".\\Resources\\CadMsg\\SH840-4L.txt");
	//this->AddWheel(surveyFlange, surveyTread,"W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Resources\\CadMsg\\SH840-4L.txt");
}
void QLocalOpenGLWidget::AddWheel(const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread, const QString filePath)
{
	this->makeCurrent();

	Wheel::WheelFlangeTread wheel(surveyFlange, surveyTread/*, filePath*/);
	this->wheels.push_back(wheel);

    //emit PickWidthRange(wheel.GetTreadRange().GetStart(),wheel.GetTreadRange().GetEnd());
	emit SpreadWheelOpenGLMsg(wheel.LoadSpreadPoint());

	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放

	glm::vec3 color(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f);

	if (this->floatPlants.find(filePath)==this->floatPlants.end())//找不到filePath,添加
	{
		std::vector<glm::vec2> floatPlant = Wheel::InitFloatPlateData(filePath);
		this->floatPlants.insert({ filePath,floatPlant });
		OpenglObj::OpenglPointMsg floatPlantMsg = Wheel::LoadFloatPlantPoint(floatPlant);
		this->engine.AddWheelFloatPlant(filePath, Object::MeshObject(OpenglObj::Mesh(floatPlantMsg.GetVertices(), floatPlantMsg.GetIndices()),model,color));
	}
	//添加wheelObject
	OpenglObj::OpenglPointMsg wheelFlangeTreadMsg = wheel.LoadFlangeTreadPoint();
	OpenglObj::OpenglPointMsg wheelConnectionMsg = Wheel::LoadConnectionPoint(wheel.get_flange_tread(), this->floatPlants[filePath]);
	this->engine.AddWheelObject(
		WheelObject(Object::MeshObject(OpenglObj::Mesh(wheelFlangeTreadMsg.GetVertices(), wheelFlangeTreadMsg.GetIndices()), model, color),//flangeTreadObj
			Object::MeshObject(OpenglObj::Mesh(wheelConnectionMsg.GetVertices(), wheelConnectionMsg.GetIndices()), model, color),//connectionObj
		filePath)
	);

	if (this->currentWheelId == -1)//默认currentWheelID
	{
		this->SetCurrentWheel(0);
	}

    this->doneCurrent();  //注意doneCurrent!
}
void QLocalOpenGLWidget::UpdateWheelFlangeTreadModel(float radian)
{
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放
	//...
	model = glm::rotate(model, radian, glm::vec3(0.0f, 0.0f, -1.0f));
	if (this->currentWheelId < 0 || this->currentWheelId >= this->wheels.size())
	{
		std::cout << "Error::QLocalOpenGLWidget::UpdateWheelFlangeTreadModel::illegal id!" << std::endl;
	}
	else
	{
		this->engine.UpdateWheelFlangeTreadModel(model, this->currentWheelId);
	}
}
void QLocalOpenGLWidget::SetCurrentWheel(int id)
{
	if (id < 0 || id >= this->wheels.size())
	{
		std::cout << "ERROR::LOCALWIDGET::SETCURRENTWHEEL::ILLEGAL ID!" << std::endl;
	}
	else
	{
		this->currentWheelId = id;
		pickMsg.SetRadian(-1.0f);
		pickMsg.SetWidth(70.0f);
		this->makeCurrent();
		shaderProgram_flange_tread.use();
		shaderProgram_flange_tread.setFloat("pickRadian", pickMsg.GetLength());
		shaderProgram_flange_tread.setFloat("pickWidth", pickMsg.GetWidth());
		shaderProgram_flange_tread.setFloat("maxGap", 1.0f);
		shaderProgram_flange_tread.setFloat("widthMax", 105.0f);

		auto standardTreadMsg = this->wheels[currentWheelId].GetStandardTreadMsg();
		int size = standardTreadMsg.size();
		for (int i = 0; i != size; ++i)
		{
			std::string name = "standardTreadMsg[" + std::to_string(i) + "]";
			shaderProgram_flange_tread.setVec2(name, standardTreadMsg[i]);
		}
		shaderProgram_flange_tread.setInt("standardTreadMsgSize", size);
		this->doneCurrent();
		//this->engine.SetCurrentWheel(id);
	}
}
int QLocalOpenGLWidget::GetWheelSize()
{
    return this->engine.GetWheelsSize();
}
//void QLocalOpenGLWidget::initEngine()
//{
//	Wheel wheel;
//	Mesh wheelMesh(wheel.GetVertices(), wheel.GetIndices());  //wheelMesh(std::vector<Vertex>(),std::vector<unsigned int>())不行？
//	std::cout << wheel.GetVertices().size() << std::endl;
//	glm::mat4 model(1.0f);
//	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));  //不允许 非y轴360度 的翻转
//	this->engine.addMesh(wheelMesh, model);
//}
void QLocalOpenGLWidget::resizeGL(int w, int h)
{
	//int minValue = min(w, h);
	this->scr_width = w;
	this->scr_height = h;
	glViewport(0, 0, w, h);
	
}
//void QLocalOpenGLWidget::showEvent(QShowEvent *e)
//{
//	QOpenGLWidget::showEvent(e);
//	//this->InitPickValue();
//}
void QLocalOpenGLWidget::paintGL()
{
	//std::cout << "1" << std::endl;
	//渲染指令
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//设定清屏所用的颜色
	//glClear(GL_COLOR_BUFFER_BIT);//接受一个缓冲位(Buffer Bit)来指定要清空的缓冲
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram_flange_tread.use();
	shaderProgram_flange_tread.setVec3("objectColor", 192.0f/255.0f, 192.0f / 255.0f, 192.0f / 255.0f);
	shaderProgram_flange_tread.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shaderProgram_flange_tread.setVec3("lightPos", camera.position);
	shaderProgram_flange_tread.setVec3("viewPos", camera.position);

	//view/projection
	view = camera.GetViewMatrix();//3d摄像机
	shaderProgram_flange_tread.setMat4("view", view);
	//glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(camera.zoom), ((float)this->scr_width / this->scr_height), 0.1f, 100.0f);
	shaderProgram_flange_tread.setMat4("projection", projection);
	
	if (this->currentWheelId != -1)
	{
        engine.DrawWheelFlangeTread(shaderProgram_flange_tread, this->currentWheelId);
	}

	shaderProgram_other.use();
	shaderProgram_other.setVec3("objectColor", 192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f);
	shaderProgram_other.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shaderProgram_other.setVec3("lightPos", camera.position);
	shaderProgram_other.setVec3("viewPos", camera.position);
	shaderProgram_other.setMat4("view", view);
	shaderProgram_other.setMat4("projection", projection);
	if (this->currentWheelId != -1)
	{
		engine.DrawWheelFloatPlantAndConnection(shaderProgram_other, this->currentWheelId);
	}
    /*if(this->coordinateAxisVisible)
        engine.DrawCoordinateAxis(shaderProgram_other);*/

}
void QLocalOpenGLWidget::SetPickRadian(float radian)
{
    if (currentWheelId != -1)
    {
		//std::cout << "ok?" << std::endl;
        if (radian >= 0 && radian <= 2 * Wheel::WheelFlangeTread::PIR)
        {
            if(abs(this->pickMsg.GetRadian() -radian)>0.001)
            {
                this->pickMsg.SetRadian(radian);
                this->makeCurrent();
				shaderProgram_flange_tread.use();
				shaderProgram_flange_tread.setFloat("pickRadian", radian);  //是-1可以刷新数据
                this->doneCurrent();
                //std::cout << "pickRadian:" << msg.radian << std::endl;
                emit PickSectionMsg(wheels[this->currentWheelId].GetSection(radian));//没有东西不发数据
            }
			this->UpdateWheelFlangeTreadModel(radian);
        }
        else
        {
            std::cout << "ERROR::LOCALWIDGET::SetPickRadian::ILLEGAL RADIAN" << std::endl;

        }
    }
    else
    {
        std::cout << "ERROR::LOCALWIDGET::SetPickRadian::NO WHEEL TO SET PICK RADIAN!" << std::endl;
    }
}
void QLocalOpenGLWidget::SetPickAngle(float angle)
{
    if (currentWheelId != -1)
    {
        if (angle >= 0 && angle <= 360)
        {
            float radian=(2* Wheel::WheelFlangeTread::PIR)*angle/360;
            if(abs(this->pickMsg.GetRadian()-radian)>0.001)
            {
                this->pickMsg.SetRadian(radian);
                this->makeCurrent();
				shaderProgram_flange_tread.use();
				shaderProgram_flange_tread.setFloat("pickRadian", radian);  //是-1可以刷新数据
                this->doneCurrent();
                //std::cout << "pickRadian:" << msg.radian << std::endl;
                emit PickSectionMsg(wheels[this->currentWheelId].GetSection(radian));//没有东西不发数据

            }
			this->UpdateWheelFlangeTreadModel(radian);
        }
        else
        {
            std::cout << "ERROR::LOCALWIDGET::SetPickAngle::ILLEGAL Angle" << std::endl;

        }
    }
    else
    {
        std::cout << "ERROR::LOCALWIDGET::SetPickAngle::NO WHEEL TO SET PICK Angle!" << std::endl;
    }
}
void QLocalOpenGLWidget::mousePressEvent(QMouseEvent *e)
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
            this->Pick(e->x(),e->y());
        }
		this->leftButtonDown = true;
	}
	if (e->button() == Qt::RightButton)////右键取消
	{
		if (this->rotateAction)
		{
			this->rotateAction = false;
			emit RotateSignal(this->rotateAction);
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
void QLocalOpenGLWidget::mouseReleaseEvent(QMouseEvent *e)  //在非opengl函数中调用opengAPI需要makeCurrent
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
	//this->paintGL();
}


void QLocalOpenGLWidget::mouseMoveEvent(QMouseEvent * e)
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
		else
		{
            this->Pick(e->x(),e->y());

		}
	}
	if (middleButtonDown)
	{
		float xoffset = e->x() - this->lastX;
		float yoffset = this->lastY - e->y();
		this->lastX = e->x();
		this->lastY = e->y();

		camera.ProcessMiddleMouseMovement(xoffset, yoffset);
	}
}
void QLocalOpenGLWidget::Pick(int posX,int posY)
{
    if (this->currentWheelId != -1)
    {
		//std::cout << "OK1?" << std::endl;
		PickMsg msg = engine.RayPick_Wheel(posX, posY, projection, view, camera.position, this->scr_width, this->scr_height);
		//std::cout << "OK2?" << std::endl;
        if (this->pickSectionEnable&&msg.GetRadian() != -1)//防止空
        {
            this->pickMsg.SetRadian(msg.GetRadian());
            this->makeCurrent();
			shaderProgram_flange_tread.use();
			shaderProgram_flange_tread.setFloat("pickRadian", msg.GetRadian());  //是-1可以刷新数据
            this->doneCurrent();
            //std::cout << "pickRadian:" << msg.radian << std::endl;
            emit PickSectionMsg(wheels[this->currentWheelId].GetSection(msg.GetRadian()));//没有东西不发数据
            float angle=360.0f*msg.GetRadian() /(2* Wheel::WheelFlangeTread::PIR);
            emit PickSectionAngle(angle);  //只有pick执行时才会发送改变gotoWidget的信号，set的时候不发
        }
   //     if (this->pickWidthEnable&&msg.GetWidth() != -1)
   //     {
			//Range range = wheels[currentWheelId].GetTreadRange();
   //         if (msg.GetWidth() >= range.GetStart() && msg.GetWidth() <= range.GetEnd())
   //         {
   //             this->pickMsg.SetWidth(msg.GetWidth());
   //             this->makeCurrent();
   //             shaderProgram.setFloat("pickWidth", msg.GetWidth());
   //             this->doneCurrent();
   //             //std::cout << "pickWidth:" << msg.width << std::endl;
   //             emit PickWidth(msg.GetWidth());
   //         }
   //     }
    }
}
//void QLocalOpenGLWidget::SetPickWidth(double width)
//{
//	if (currentWheelId != -1)
//	{
		//Range range = wheels[currentWheelId].GetTreadRange();
		//if (width >= range.GetStart() && width <= range.GetEnd())
		//{
  //          if(abs(this->pickMsg.GetWidth()-width)>0.001)//查重
  //          {
  //              this->pickMsg.SetWidth(width);
  //              this->makeCurrent();
  //              shaderProgram.setFloat("pickWidth", width);
  //              this->doneCurrent();
  //              //std::cout << "pickWidth:" << width << std::endl;
  //              emit PickWidth(width);//也要发！
  //          }
		//}
		//else
		//{
  //          std::cout << "ERROR::LOCALWIDGET::SETPICKWIDTH::ILLEGAL WIDTH! "<<range.GetStart()<<" "<<range.GetEnd() << std::endl;
		//}
//	}
//	else
//	{
//		std::cout << "ERROR::LOCALWIDGET::SETPICKWIDTH::NO WHEEL TO SET PICK WIDTH!" << std::endl;
//	}
//}

void QLocalOpenGLWidget::timerEvent(QTimerEvent *e)
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

void QLocalOpenGLWidget::keyPressEvent(QKeyEvent * e)
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
void QLocalOpenGLWidget::keyReleaseEvent(QKeyEvent * e)
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

void QLocalOpenGLWidget::wheelEvent(QWheelEvent * e)
{
	camera.ProcessMouseScroll(e->delta());
	//this->paintGL();
}

void QLocalOpenGLWidget::Bigger()
{
	camera.ProcessMouseScroll(120);
	//if (!camera.ProcessMouseScroll(120))
	//{
	//	this->bigEnable = false;
	//	emit BiggerSignal(this->bigEnable);
	//}
	//else
	//{
	//	if (!this->smallerEnable)
	//	{
	//		this->smallerEnable = true;
	//		emit SmallerSignal(this->smallerEnable);
	//	}
	//}
}

void QLocalOpenGLWidget::Smaller()
{
	camera.ProcessMouseScroll(-120);
	/*if (!camera.ProcessMouseScroll(-120))
	{
		this->smallerEnable = false;
		emit SmallerSignal(this->smallerEnable);
	}
	else
	{
		if (!this->bigEnable)
		{
			this->bigEnable = true;
			emit BiggerSignal(this->bigEnable);
		}
	}*/
}

void QLocalOpenGLWidget::InitCamera()
{
	//camera.InitCamera();
	camera.InitCamera(kYaw, kPitch, kCameraLocalAngle, glm::vec3(0.0f, 0.0f, 0.0f), kR, kZoom);
}

void QLocalOpenGLWidget::Rotate()
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

void QLocalOpenGLWidget::EnableSectionPick()
{
	if (this->pickSectionEnable)
	{
		this->pickSectionEnable = false;
		this->makeCurrent();
		shaderProgram_flange_tread.use();
		shaderProgram_flange_tread.setFloat("pickRadian", -1);
		this->doneCurrent();
	}
	else
	{
		this->pickSectionEnable = true;
	}
}
void QLocalOpenGLWidget::EnableWidthPick()
{
	if (this->pickWidthEnable)
	{
		this->pickWidthEnable = false;
		this->makeCurrent();
		shaderProgram_flange_tread.use();
		shaderProgram_flange_tread.setFloat("pickWidth", -1);
		this->doneCurrent();
	}
	else
	{
		this->pickWidthEnable = true;
		this->makeCurrent();
		shaderProgram_flange_tread.use();
		shaderProgram_flange_tread.setFloat("pickWidth", pickMsg.GetWidth());
		this->doneCurrent();
	}

}
//void QLocalOpenGLWidget::CoordinateAxisVisible()
//{
//    if(this->coordinateAxisVisible)
//    {
//        this->coordinateAxisVisible=false;
//    }
//    else
//    {
//        this->coordinateAxisVisible=true;
//    }
//}

void QLocalOpenGLWidget::ColorMarkVisible()
{
    if(this->colorMarkVisible)
    {
        this->colorMarkVisible =false;
    }
    else
    {
        this->colorMarkVisible =true;
    }
    this->makeCurrent();
	this->shaderProgram_flange_tread.use();
    this->shaderProgram_flange_tread.setBool("colorMark",this->colorMarkVisible);
    this->doneCurrent();
}
