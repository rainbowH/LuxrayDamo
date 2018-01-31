#include "qlocalspreadopenglwidget.hpp"

#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <glm/gtc/matrix_transform.hpp>
#include <qmessagebox.h>
#include <qevent.h>
#include <qpainter.h>

#include "Vertex.h"
#include "Mesh.h"
#include "LoadModel.h"
#include "FilePath.h"
#include "Ruler.h"
#include "stb_image.h"
#include "Font.h"
#include "CoordinateObject.h"
#include "CartesianCoordinate.h"

const float QLocalSpreadOpenglWidget::kYaw = -90.0f;
const float QLocalSpreadOpenglWidget::kPitch = -89.9f;
const float QLocalSpreadOpenglWidget::kZoom = 45.0f;
const float QLocalSpreadOpenglWidget::kR = 2.0f;
const float QLocalSpreadOpenglWidget::kMouseSensitivity = 0.1f;//鼠标灵敏度
const float QLocalSpreadOpenglWidget::kMiddleMouseSensitivity = 0.03f;
const float QLocalSpreadOpenglWidget::kWheelSensitivity = 0.001f;

QLocalSpreadOpenglWidget::QLocalSpreadOpenglWidget(QWidget * parent)
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
	keySPressEvent(false),
	keyAPressEvent(false),
	keyDPressEvent(false),
	pickMsg(-1.0f, 70.0f),
	coordinate_visible(true),
	ruler_visible(true)
{

	this->scr_width = this->size().width();
	this->scr_height = this->size().height();

	//this->startTimer(TIMESPAN);//ms
	this->timer.start(TIMESPAN, this);
}

QLocalSpreadOpenglWidget::~QLocalSpreadOpenglWidget() { //这个函数要修改上下文，所以需要makeCurrent
	this->makeCurrent();

	this->doneCurrent();
}
void QLocalSpreadOpenglWidget::InitShaders()
{
	wheel_spread_shader_program.initShaderProgram(FilePath::kVWheelSpreadShaderPath,
		FilePath::kFWheelSpreadShaderPath);
	line_shader_program.initShaderProgram(FilePath::kVLineShaderPath, FilePath::kFLineShaderPath);

	//std::cout << "Font Shader!" << std::endl;
	font_shader_program.initShaderProgram(FilePath::kVFontShaderPath, FilePath::kFFontShaderPath);
}
void QLocalSpreadOpenglWidget::initializeGL()
{
	initializeOpenGLFunctions();//glad

	glViewport(0, 0, scr_width, scr_height);

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_DEBUG_OUTPUT);
	//glDisable(GL_DEPTH_TEST);
	// Enable back face culling
	//glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	this->InitShaders();

	this->LoadFontTexture();
	//Font::LoadFontTexture();
	//this->camera.SetDisToFocus(2.0f);
	this->InitRuler();
	this->InitCartesianCoordinate();

	this->LoadExampleMsg();

	this->InitCamera();

	this->InitShaderValue();
}
void QLocalSpreadOpenglWidget::LoadFontTexture()//仅用与initGL使用
{
	this->font_texture = Font::LoadFontTexture();
	font_shader_program.use();
	font_shader_program.setInt("font_texture", 0);//GL_TEXTURE0
}
void QLocalSpreadOpenglWidget::LoadExampleMsg()
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
			surveyTread.insert(startX + 0.1f*j, circlePoint);
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
	while (file >> x >> y)
	{
		surveyFlange.push_back(QVector2D(x, y));
	}
	file.close();

	//this->AddWheel(surveyFlange, surveyTread, "SH840-4L");

	Wheel::WheelFlangeTread wheel_flange_tread(surveyFlange, surveyTread);

	this->AddWheelSpread(wheel_flange_tread.LoadSpreadPoint());
}
void QLocalSpreadOpenglWidget::AddWheelSpread(Wheel::WheelSpreadMsg spmsg)
{
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放

	glm::vec3 color(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f);

	this->makeCurrent();
	this->engine.AddWheel(OpenglObj::Mesh(spmsg.wheelMsg.GetVertices(), spmsg.wheelMsg.GetIndices()), model, color);
	this->doneCurrent();

	this->wheelSpreadParm.push_back(spmsg.wheelSpreadParm);

	if (this->currentWheelId == -1)//默认currentWheelID
	{
		this->SetCurrentWheel(0);
	}
}
void QLocalSpreadOpenglWidget::InitCartesianCoordinate()
{
	this->makeCurrent();
	float coordinate_text_size = 8;
	Coordinate::CartesianCoordinate coordinate;
	coordinate.set_axis_length(150.0f, 700.0f, 100.0f);

	/*coordinate.AddMark(Coordinate::Axis::X, Coordinate::Mark(70.0f / 135.0f, Text("70", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::X, Coordinate::Mark(135.0f / 135.0f, Text("135", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	
	coordinate.AddMark(Coordinate::Axis::X, Coordinate::Mark(35.0f / 135.0f, Text("", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::X, Coordinate::Mark(105.0f / 135.0f, Text("", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));*/

	float start_mark = 50.0f;
	for (int i = 0; i != 3; ++i)
	{
		std::string text;
		std::stringstream ss;
		ss << start_mark;
		ss >> text;//或者 res = ss.str();
		coordinate.AddMark(Coordinate::Axis::X, Coordinate::Mark(start_mark / 150.0f, Text(text, coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
		start_mark += 50.0f;
	}

	start_mark = 25.0f;
	for (int i = 0; i != 3; ++i)
	{
		coordinate.AddMark(Coordinate::Axis::X, Coordinate::Mark(start_mark / 150.0f, Text("", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
		start_mark += 50.0f;
	}


	start_mark = 100.0f;
	for (int i = 0; i != 7; ++i)
	{
		std::string text;
		std::stringstream ss;
		ss << start_mark;
		ss >> text;//或者 res = ss.str();
		coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(start_mark / 700.0f, Text(text, coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
		start_mark += 100.0f;
	}
	//coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(100.0f / 700.0f, Text("100", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	//coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(200.0f / 700.0f, Text("200", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	//coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(300.0f / 700.0f, Text("300", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	//coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(400.0f / 700.0f, Text("400", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	//coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(500.0f / 700.0f, Text("500", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	//coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(600.0f / 700.0f, Text("600", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	//coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(700.0f / 700.0f, Text("700", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));

	start_mark = 50.0f;
	for (int i = 0; i != 7; ++i)
	{
		coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(start_mark / 700.0f, Text("", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
		start_mark += 100.0f;
	}


	coordinate.AddMark(Coordinate::Axis::Z, Coordinate::Mark(25.0f / 100.0f, Text("25", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::Z, Coordinate::Mark(50.0f / 100.0f, Text("50", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::Z, Coordinate::Mark(75.0f / 100.0f, Text("75", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::Z, Coordinate::Mark(100.0f / 100.0f, Text("100", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));

	glm::mat4 coordinate_model(1.0f);
	coordinate_model = scale*coordinate_model;
	coordinate_model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f))*coordinate_model;
	coordinate_model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f))*coordinate_model;
	coordinate_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.4f, 0.0f))*coordinate_model;

	//std::cout << "h_ruler!" << std::endl;
	auto coordinate_object = coordinate.LoadObject(this->font_texture);
	coordinate_object.SetModel(coordinate_model);
	
	engine.AddCoordinate(coordinate_object);

	this->doneCurrent();
}
void QLocalSpreadOpenglWidget::InitRuler()
{
	this->makeCurrent();//注意
	float ruler_text_size = 8;
	Coordinate::Ruler h_ruler(135.0f);//竖直
	h_ruler.set_mark_text_dis_to_mark(10.0f);
	h_ruler.add_mark(Coordinate::Mark(0.0f / 135.0f, Text("0(mm)", ruler_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	h_ruler.add_mark(Coordinate::Mark(70.0f / 135.0f,Text("70", ruler_text_size,glm::vec3(1.0f,1.0f,1.0f))));
	h_ruler.add_mark(Coordinate::Mark(135.0f / 135.0f, Text("135", ruler_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));

	glm::mat4 scale= glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));

	glm::mat4 h_ruler_model(1.0f);
	h_ruler_model = scale*h_ruler_model;
	h_ruler_model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f))*h_ruler_model;
	h_ruler_model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f))*h_ruler_model;
	h_ruler_model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.39f, 0.0f, 0.0f))*h_ruler_model;

	//std::cout << "h_ruler!" << std::endl;
	auto h_ruler_object = h_ruler.LoadObject(this->font_texture);
	h_ruler_object.SetModel(h_ruler_model);

	Coordinate::Ruler v_ruler(600.0f);
	v_ruler.set_mark_text_dis_to_mark(10.0f);
	v_ruler.add_mark(Coordinate::Mark(0.0f / 360.0f, Text("0(mm)", ruler_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	v_ruler.add_mark(Coordinate::Mark(45.0f / 360.0f, Text("45", ruler_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	v_ruler.add_mark(Coordinate::Mark(90.0f / 360.0f, Text("90", ruler_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	v_ruler.add_mark(Coordinate::Mark(135.0f / 360.0f, Text("135", ruler_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	v_ruler.add_mark(Coordinate::Mark(180.0f / 360.0f, Text("180", ruler_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	v_ruler.add_mark(Coordinate::Mark(225.0f / 360.0f, Text("225", ruler_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	v_ruler.add_mark(Coordinate::Mark(270.0f / 360.0f, Text("270", ruler_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	v_ruler.add_mark(Coordinate::Mark(315.0f / 360.0f, Text("315", ruler_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	v_ruler.add_mark(Coordinate::Mark(360.0f / 360.0f, Text("360", ruler_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));

	glm::mat4 v_ruler_model(1.0f);
	v_ruler_model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f))*v_ruler_model;
	//model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f))*model;
	v_ruler_model = scale*v_ruler_model;
	v_ruler_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.05f, 1.4f))*v_ruler_model;

	//std::cout << "v_ruler!" << std::endl;
	auto v_ruler_object = v_ruler.LoadObject(this->font_texture);
	v_ruler_object.SetModel(v_ruler_model);

	engine.AddRuler(h_ruler_object);
	engine.AddRuler(v_ruler_object);

	this->doneCurrent();
}
void QLocalSpreadOpenglWidget::SetCurrentWheel(int id)
{
	if (id < 0 || id >= this->engine.GetWheelsSize())
	{
		std::cout << "ERROR::QLocalSpreadOpenglWidget::SETCURRENTWHEEL::ILLEGAL ID!" << std::endl;
	}
	else
	{
		this->currentWheelId = id;

		pickMsg.SetLength(-1.0f);
		pickMsg.SetWidth(70.0f);
		
		//切换轮子时重置shader
		this->makeCurrent();
		wheel_spread_shader_program.use();
		wheel_spread_shader_program.setFloat("pickLength", pickMsg.GetLength());
		wheel_spread_shader_program.setFloat("pickWidth", pickMsg.GetWidth());
		wheel_spread_shader_program.setFloat("unitLength", this->wheelSpreadParm[currentWheelId].GetUnitLength());
		wheel_spread_shader_program.setFloat("unitWidth", this->wheelSpreadParm[currentWheelId].GetUnitWidth());
		wheel_spread_shader_program.setFloat("originHeight", Wheel::WheelFlangeTread::STANDARDTREADRADIUS);
		wheel_spread_shader_program.setFloat("originMaxHeight", this->wheelSpreadParm[currentWheelId].GetSpreadYRange().GetMax());
		wheel_spread_shader_program.setFloat("originMinHeight", this->wheelSpreadParm[currentWheelId].GetSpreadYRange().GetMin());
		wheel_spread_shader_program.setFloat("maxGap", 1.0f);
		wheel_spread_shader_program.setFloat("widthMax", 105.0f);
		int size = this->wheelSpreadParm[currentWheelId].GetStandardTreadMsg().size();
		for (int i = 0; i != size; ++i)
		{
			std::string name = "standardTreadMsg[" + std::to_string(i) + "]";
			wheel_spread_shader_program.setVec2(name, this->wheelSpreadParm[currentWheelId].GetStandardTreadMsg()[i]);
		}
		wheel_spread_shader_program.setInt("standardTreadMsgSize", size);

		this->doneCurrent();
		//this->engine.SetCurrentWheel(id);
	}
}

void QLocalSpreadOpenglWidget::resizeGL(int w, int h)
{
	this->scr_width = w;
	this->scr_height = h;
	glViewport(0, 0, w, h);

}
//void QLocalSpreadOpenglWidget::showEvent(QShowEvent *e)
//{
//	QOpenGLWidget::showEvent(e);
//	//this->InitPickValue();
//}
void QLocalSpreadOpenglWidget::InitShaderValue()
{
	this->makeCurrent();
	wheel_spread_shader_program.use();
	//shaderProgram.setFloat("pickLength", pickMsg.GetLength());
	//shaderProgram.setFloat("pickWidth", pickMsg.GetWidth());
	this->doneCurrent();
}
void QLocalSpreadOpenglWidget::paintGL()
{
	QPainter painter(this);

	painter.beginNativePainting();

	this->DrawOpengl();

	painter.endNativePainting();

	//this->Draw2DText(painter);

}
void QLocalSpreadOpenglWidget::Draw2DText(QPainter& painter)  //only use for paintGL
{
	//QPainter painter(this);
	//painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	//painter.setPen(Qt::white);
	//painter.drawText(0, 0, "Hellow Opengl!");

	painter.setPen(Qt::white);
	painter.setFont(QFont("Arial", 14));
	painter.drawText(rect(), Qt::AlignCenter, "Test");
}
void QLocalSpreadOpenglWidget::DrawOpengl()//only use for paintGL
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//设定清屏所用的颜色
										 //glClear(GL_COLOR_BUFFER_BIT);//接受一个缓冲位(Buffer Bit)来指定要清空的缓冲
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	wheel_spread_shader_program.use();
	wheel_spread_shader_program.setVec3("objectColor", 192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f);
	wheel_spread_shader_program.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	wheel_spread_shader_program.setVec3("lightPos", camera.position);
	wheel_spread_shader_program.setVec3("viewPos", camera.position);

	//view/projection
	view = camera.GetViewMatrix();//3d摄像机
	wheel_spread_shader_program.setMat4("view", view);
	//glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(camera.zoom), ((float)this->scr_width / this->scr_height), 0.1f, 100.0f);
	wheel_spread_shader_program.setMat4("projection", projection);

	if (this->currentWheelId != -1)
	{
		engine.DrawWheel(wheel_spread_shader_program, this->currentWheelId);
	}


	line_shader_program.use();
	line_shader_program.setMat4("view", view);
	line_shader_program.setMat4("projection", projection);

	//engine.DrawLines(line_shader_program);
	//this->coordinate_line.Draw(line_shader_program);
	font_shader_program.use();
	font_shader_program.setMat4("view", view);
	font_shader_program.setMat4("projection", projection);

	engine.UpdateTextDirection(camera.front, camera.up, projection, view);
	//engine.DrawRulers(line_shader_program,font_shader_program);
	
	if (this->ruler_visible)
	{
		engine.DrawRulers(line_shader_program, font_shader_program);
		//std::cout << "?" << std::endl;
		//engine.DrawRulers(line_shader_program, font_shader_program);
	}

	if (this->coordinate_visible)
	{
		engine.DrawCoordinates(line_shader_program, font_shader_program);
	}

	glDisable(GL_DEPTH_TEST);
}
void QLocalSpreadOpenglWidget::paintEvent(QPaintEvent *e)
{
	//QPainter painter(this);
	//painter.drawText(0, 0, "Hellow Opengl!");
	//this->makeCurrent();
	//this->paintGL();
	//QPainter pter(this);
	//pter.setPen(Qt::blue);
	//pter.drawText(20, 50, "This is a Text!");
	//pter.end();
	//update();

	QOpenGLWidget::paintEvent(e);
}
void QLocalSpreadOpenglWidget::SetPickRadian(double radian)
{
	if (currentWheelId != -1)
	{
		if (radian >= 0 && radian <= 2 * Wheel::WheelFlangeTread::PIR)
		{
			Range lengthRange = wheelSpreadParm[currentWheelId].GetLengthRange();
			float pickLength =/* lengthRange.GetStart() - */lengthRange.GetDifference()*radian / (2 * Wheel::WheelFlangeTread::PIR);
			
			if (abs(pickLength - this->pickMsg.GetLength()) > wheelSpreadParm[currentWheelId].GetUnitLength())
			{
				this->pickMsg.SetLength(pickLength);
				this->makeCurrent();
				wheel_spread_shader_program.use();
				wheel_spread_shader_program.setFloat("pickLength", pickLength);  //是-1可以刷新数据
				this->doneCurrent();
			}
		}
		else
		{
			std::cout << "ERROR::QLocalSpreadOpenglWidget::SetPickRadian::ILLEGAL RADIAN" << std::endl;
		}
	}
	else
	{
		std::cout << "ERROR::QLocalSpreadOpenglWidget::SetPickRadian::NO WHEEL TO SET PICK RADIAN!" << std::endl;
	}
}
void QLocalSpreadOpenglWidget::SetPickAngle(double angle)
{
	if (currentWheelId != -1)
	{
		if (angle >= 0 && angle <= 360)
		{
			float radian = (2 * Wheel::WheelFlangeTread::PIR)*angle / 360;
			Range lengthRange = wheelSpreadParm[currentWheelId].GetLengthRange();
			float pickLength = /*lengthRange.GetStart() - */lengthRange.GetDifference()*radian / (2 * Wheel::WheelFlangeTread::PIR);
			if (abs(pickLength - this->pickMsg.GetLength()) > wheelSpreadParm[currentWheelId].GetUnitLength())
			{
				this->pickMsg.SetLength(pickLength);
				this->makeCurrent();
				wheel_spread_shader_program.use();
				wheel_spread_shader_program.setFloat("pickLength", pickLength);  //是-1可以刷新数据
				this->doneCurrent();
			}
		}
		else
		{
			std::cout << "ERROR::QLocalSpreadOpenglWidget::SetPickAngle::ILLEGAL Angle" << std::endl;

		}
	}
	else
	{
		std::cout << "ERROR::QLocalSpreadOpenglWidget::SetPickAngle::NO WHEEL TO SET PICK Angle!" << std::endl;
	}
}
void QLocalSpreadOpenglWidget::mousePressEvent(QMouseEvent *e)
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
			this->Pick(e->x(), e->y());
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
void QLocalSpreadOpenglWidget::mouseReleaseEvent(QMouseEvent *e)  //在非opengl函数中调用opengAPI需要makeCurrent
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



void QLocalSpreadOpenglWidget::mouseMoveEvent(QMouseEvent * e)
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
			this->Pick(e->x(), e->y());

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

void QLocalSpreadOpenglWidget::Pick(int posX, int posY)
{
	if (this->currentWheelId != -1)
	{
		//std::cout << "Pick 1" << std::endl;
		PickMsg locMsg = engine.RayPick_WheelSpread
		(
			posX, posY,
			projection, view,
			camera.position,
			this->scr_width, this->scr_height,
			Range(0, Wheel::WheelFlangeTread::STANDARDWIDTH),
			this->wheelSpreadParm[currentWheelId].GetLengthRange()
		);
		//std::cout <<"Length: "<< msg.GetLength() <<" Width: "<< msg.GetWidth() << std::endl;

		if (this->pickSectionEnable&&locMsg.GetLength() != -1)//防止空
		{
		
			this->makeCurrent();
			wheel_spread_shader_program.use();
			wheel_spread_shader_program.setFloat("pickLength", locMsg.GetLength());  //是-1可以刷新数据
			this->doneCurrent();
			std::cout << "pickLength:" << locMsg.GetLength() << std::endl;

			//this->pickMsg.SetLength(this->wheelSpreadParm[currentWheelId].GetLengthRange().GetStart()- locMsg.GetLength());//真正的length
			this->pickMsg.SetLength(locMsg.GetLength());

			float radian=2* Wheel::WheelFlangeTread::PIR*pickMsg.GetLength() / this->wheelSpreadParm[currentWheelId].GetLengthRange().GetDifference();
			//float angle = 360.0f*radian / (2 * WheelDisplay::Wheel::PIR);
			emit PickSectionRadian(radian);  //只有pick执行时才会发送改变gotoWidget的信号，set的时候不发
		}

		//if (this->pickWidthEnable&&locMsg.GetWidth() != -1)
		//{
		//	Range range = wheelSpreadParm[currentWheelId].GetWidthRange();
		//	if (locMsg.GetWidth() >= range.GetMin() && locMsg.GetWidth() <= range.GetMax())  //再加以限制
		//	{
		//		this->pickMsg.SetWidth(locMsg.GetWidth());
		//		this->makeCurrent();
		//		shaderProgram.setFloat("pickWidth", locMsg.GetWidth());
		//		this->doneCurrent();
		//		std::cout << "pickWidth:" << locMsg.GetWidth() << std::endl;

		//		emit PickWidth(this->pickMsg.GetWidth());
		//	}
		//}
		//std::cout << "Pick 2" << std::endl;
	}
}
//void QLocalSpreadOpenglWidget::SetPickWidth(double width)
//{
//	if (currentWheelId != -1)
//	{
		//Range range = wheelSpreadParm[currentWheelId].GetWidthRange();
		//if (width >= range.GetMax() && width <= range.GetMin())
		//{
		//	if (abs(this->pickMsg.GetWidth() - width)>0.001)//查重
		//	{
		//		this->pickMsg.SetWidth(width);
		//		this->makeCurrent();
		//		shaderProgram.setFloat("pickWidth", width);
		//		this->doneCurrent();
		//		//std::cout << "pickWidth:" << width << std::endl;
		//		emit PickWidth(width);//也要发！
		//	}
		//}
		//else
		//{
		//	std::cout << "ERROR::QLocalSpreadOpenglWidget::SETPICKWIDTH::ILLEGAL WIDTH! " << range.GetStart() << " " << range.GetEnd() << std::endl;
		//}
//	}
//	else
//	{
//		std::cout << "ERROR::QLocalSpreadOpenglWidget::SETPICKWIDTH::NO WHEEL TO SET PICK WIDTH!" << std::endl;
//	}
//}

void QLocalSpreadOpenglWidget::timerEvent(QTimerEvent *e)
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

void QLocalSpreadOpenglWidget::keyPressEvent(QKeyEvent * e)
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
void QLocalSpreadOpenglWidget::keyReleaseEvent(QKeyEvent * e)
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

void QLocalSpreadOpenglWidget::wheelEvent(QWheelEvent * e)
{
	camera.ProcessMouseScroll(e->delta());
	//this->paintGL();
}

void QLocalSpreadOpenglWidget::Bigger()
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

void QLocalSpreadOpenglWidget::Smaller()
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

void QLocalSpreadOpenglWidget::InitCamera()
{
	//camera.InitLocalSpreadCamera();
	glm::vec3 focus;
	if (this->currentWheelId != -1)
	{
		focus.x=(wheelSpreadParm[this->currentWheelId].GetLengthRange().GetMin()+ wheelSpreadParm[this->currentWheelId].GetLengthRange().GetMax())/2.0f - 10;
		focus.z=(0.0f+ Wheel::WheelFlangeTread::STANDARDWIDTH)/2.0f + 10;
		focus.y = (wheelSpreadParm[this->currentWheelId].GetSpreadYRange().GetMin() + wheelSpreadParm[this->currentWheelId].GetSpreadYRange().GetMax()) / 2.0f - Wheel::WheelFlangeTread::STANDARDTREADRADIUS;

		glm::mat4 model(1.0f);
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放
		focus = model*glm::vec4(focus.x,focus.y,focus.z,1.0f);
	}
	else
	{
		focus = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	camera.InitCamera(kYaw, kPitch, focus, kR, kZoom);
	camera.SetMouseSensitivity(kMouseSensitivity);
	camera.SetMiddleMouseSensitivity(kMiddleMouseSensitivity);
	camera.SetWheelSensitivity(kWheelSensitivity);
}

void QLocalSpreadOpenglWidget::Rotate()
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

void QLocalSpreadOpenglWidget::EnableSectionPick()
{
	if (this->pickSectionEnable)
	{
		this->pickSectionEnable = false;
		this->makeCurrent();
		wheel_spread_shader_program.use();
		wheel_spread_shader_program.setFloat("pickRadian", -1);
		this->doneCurrent();
	}
	else
	{
		this->pickSectionEnable = true;
	}
}
void QLocalSpreadOpenglWidget::EnableWidthPick()
{
	if (this->pickWidthEnable)
	{
		this->pickWidthEnable = false;
		this->makeCurrent();
		wheel_spread_shader_program.use();
		wheel_spread_shader_program.setFloat("pickWidth", -1);
		this->doneCurrent();
	}
	else
	{
		this->pickWidthEnable = true;
	}

}
//    if(this->coordinateAxisVisible)
//    {
//        this->coordinateAxisVisible=false;
//    }
//    else
//    {
//        this->coordinateAxisVisible=true;
//    }

void QLocalSpreadOpenglWidget::ColorMarkVisible()
{

}

void QLocalSpreadOpenglWidget::CoordinateVisible()
{
	    if(this->coordinate_visible)
	    {
	        this->coordinate_visible =false;
	    }
	    else
	    {
	        this->coordinate_visible =true;
	    }
}
void QLocalSpreadOpenglWidget::RulerVisible()
{
	//std::cout << "?" << std::endl;
	    if(this->ruler_visible)
	    {
	        this->ruler_visible =false;
	    }
	    else
	    {
	        this->ruler_visible =true;
	    }
}