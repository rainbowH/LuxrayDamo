#include "dlgdevcommunic.h"
#include "ui_dlgdevcommunic.h"
#include "SelfWidget/toolbutton.h"
#include "dlgpasswardinput.h"

DlgDevCommunic::DlgDevCommunic(bool isConnect, QWidget *parent) :
    QDialog(parent),
    isConnected(isConnect),
    ui(new Ui::DlgDevCommunic)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);
    ui->lblConnectState->setStyleSheet("color: rgb(145, 145, 145)");
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);
    loadQSS("dlgdevcommunic.qss");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);

    if(isConnected)
        ui->btnConnect->setText("断开连接");
    else
        ui->btnConnect->setText("连接");
}

DlgDevCommunic::~DlgDevCommunic()
{
    delete ui;
}

void DlgDevCommunic::initWidget()
{
    //    ui->btnSelfCheck->setStyleSheet();
}

void DlgDevCommunic::loadQSS(QString qssName)
{
    QString qss = QString(":/QSS/%1").arg(qssName);
    QFile file(qss);
    file.open(QFile::ReadOnly);
    this->setStyleSheet(file.readAll());
    file.close();
}

void DlgDevCommunic::on_btnConnect_clicked()
{
    if(isConnected)
    {
        ui->btnConnect->setText("连接");

        ui->btnConnect->setIcon(QIcon(":/images/connect.png"));
        ui->lblConnectState->setText("未连接");
        ui->lblConnectState->setStyleSheet("color: rgb(145, 145, 145)");

        ui->labLight1->setPixmap(QPixmap(":/images/graylight.png"));
        ui->labLight2->setPixmap(QPixmap(":/images/graylight.png"));
        ui->labLight3->setPixmap(QPixmap(":/images/graylight.png"));
        ui->labLight4->setPixmap(QPixmap(":/images/graylight.png"));
        ui->labLight5->setPixmap(QPixmap(":/images/graylight.png"));
        ui->labLight6->setPixmap(QPixmap(":/images/graylight.png"));
        ui->labLight7->setPixmap(QPixmap(":/images/graylight.png"));
        ui->labLight8->setPixmap(QPixmap(":/images/graylight.png"));
        ui->labLight9->setPixmap(QPixmap(":/images/graylight.png"));
        ui->labLight10->setPixmap(QPixmap(":/images/graylight.png"));
    }
    else{
        DlgPasswardInput *pwdInput = new DlgPasswardInput(0);
        connect(pwdInput, SIGNAL(openDevCom()), this, SLOT(onConnect()));
        pwdInput->exec();
        if(ui->btnConnect->text() == "连接")
            return;
    }
    isConnected = !isConnected;
    emit isConnectedChanged(isConnected);
}

void DlgDevCommunic::onConnect()
{
    ui->btnConnect->setText("断开连接");
    ui->btnConnect->setIcon(QIcon(":/images/disconnect.png"));
    ui->lblConnectState->setText("已连接");
    ui->lblConnectState->setStyleSheet("color: rgb(90, 249, 65);");

    ui->labLight1->setPixmap(QPixmap(":/images/redlight.png"));
    ui->labLight2->setPixmap(QPixmap(":/images/redlight.png"));
    ui->labLight3->setPixmap(QPixmap(":/images/redlight.png"));
    ui->labLight4->setPixmap(QPixmap(":/images/redlight.png"));
    ui->labLight5->setPixmap(QPixmap(":/images/redlight.png"));
    ui->labLight6->setPixmap(QPixmap(":/images/redlight.png"));
    ui->labLight7->setPixmap(QPixmap(":/images/redlight.png"));
    ui->labLight8->setPixmap(QPixmap(":/images/redlight.png"));
    ui->labLight9->setPixmap(QPixmap(":/images/redlight.png"));
    ui->labLight10->setPixmap(QPixmap(":/images/redlight.png"));

}
