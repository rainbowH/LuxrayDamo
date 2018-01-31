#include "dlgwelcome.h"
#include "ui_dlgwelcome.h"
#include <QPainter>

DlgWelcome::DlgWelcome(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgWelcome)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint);
    mw = new MainWindow();
//    ui->btnDevCom->setStyleSheet("background-color: rgba(122, 255, 111, 0.3);");
    connect(this, SIGNAL(openDevCommunicate()), mw, SLOT(on_actionDevCommu_triggered()));
    connect(this, SIGNAL(openHistoryDatas()), mw, SLOT(on_actionHistoryDatas_triggered()));
    connect(this, SIGNAL(openMoniteRealTime()), mw, SLOT(on_actionMoniteRealTime_triggered()));
    connect(this, SIGNAL(openParaSetting()), mw, SLOT(on_actionUserParaSetting_triggered()));
    connect(mw, SIGNAL(loginShowed()), this, SLOT(show()));

    ui->btnDevCom->installEventFilter(this);
    ui->btnExit->installEventFilter(this);
    ui->btnHistDatas->installEventFilter(this);
    ui->btnMoniteRealTime->installEventFilter(this);
    ui->btnMoniteRealTime->setVisible(false);
    ui->btnParaSetting->installEventFilter(this);

    isPressed = false;
//    this->setLayout(ui->gridLayout);
}

DlgWelcome::~DlgWelcome()
{
    delete ui;
    delete mw;
}

bool DlgWelcome::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::HoverEnter)
    {
        setCursor(Qt::PointingHandCursor);
    }
    else if(event->type() == QEvent::HoverLeave)
    {
        setCursor(Qt::ArrowCursor);
    }
    else if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->button() == Qt::RightButton)
        {
            isPressed = true;
            startPoint = mouseEvent->pos();
        }
    }
    else if(event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->button() == Qt::RightButton)
        {
            isPressed = false;
        }
    }
    else if(event->type() == QEvent::MouseMove)
    {
        if(isPressed)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            endPoint = mouseEvent->pos();
            int deltaX, deltaY;
            deltaX = endPoint.x() - startPoint.x();
            deltaY = endPoint.y() - startPoint.y();
            this->setGeometry(this->x()+deltaX, this->y()+deltaY, this->width(), this->height());
        }
    }
}

void DlgWelcome::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    QPixmap pixmap(":/images/welcomebackground.jpg");
    painter.drawPixmap(-20, -15, this->width()+40, this->height()+30, pixmap);

//    ui->btnDevCom->setFixedSize(QSize(width/2, height/2));
//    ui->btnDevCom->setFixedSize(QSize(this->width()/2, this->height()/2));
    ui->btnDevCom->setFont(QFont("楷体", (this->width()>this->height()?this->width():this->height())/20));

//    ui->btnExit->setFixedSize(QSize(this->width()/2, this->height()/2));
    ui->btnExit->setFont(QFont("楷体", (this->width()>this->height()?this->width():this->height())/20));

//    ui->btnHistDatas->setFixedSize(QSize(this->width()/2, this->height()/2));
    ui->btnHistDatas->setFont(QFont("楷体", (this->width()>this->height()?this->width():this->height())/20));

//    ui->btnParaSetting->setFixedSize(QSize(this->width()/2, this->height()/2));
    ui->btnParaSetting->setFont(QFont("楷体", (this->width()>this->height()?this->width():this->height())/20));
}

void DlgWelcome::on_btnExit_clicked()
{
   // close();
    exit(0);
}

void DlgWelcome::on_btnDevCom_clicked()
{
    this->hide();
    emit openDevCommunicate();
    mw->showMaximized();
}

void DlgWelcome::on_btnMoniteRealTime_clicked()
{
    this->hide();
    mw->showMaximized();
    emit openMoniteRealTime();
}

void DlgWelcome::on_btnHistDatas_clicked()
{
    this->hide();
    mw->showMaximized();
    emit openHistoryDatas();
}

void DlgWelcome::on_btnParaSetting_clicked()
{
    this->hide();
    mw->showMaximized();
    emit openParaSetting();
}
