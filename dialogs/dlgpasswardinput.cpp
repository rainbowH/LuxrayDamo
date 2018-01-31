#include "dlgpasswardinput.h"
#include "ui_dlgpasswardinput.h"
#include "QMessageBox"
#include <QDebug>

DlgPasswardInput::DlgPasswardInput(int passwardType, QWidget *parent) :
    QDialog(parent),
    passwardType(passwardType),
    ui(new Ui::DlgPasswardInput)
{
    ui->setupUi(this);
    db = new DBSqlite();

    QFile file(":/QSS/dlgpasswardinput.qss");
    file.open(QFile::ReadOnly);
    this->setStyleSheet(file.readAll());
    file.close();
}

DlgPasswardInput::~DlgPasswardInput()
{
    delete ui;
    delete db;
}

void DlgPasswardInput::on_btnOK_clicked()
{
    db->connectDB();
    if(passwardType == 0)
    {
        QString sql = "select DevComPWD from PWDManage";
        db->runSql(sql);
        db->query->next();
        if(ui->txtPassward->text() == db->query->value(0).toString())
        {
            qDebug() <<"连接成功";
            emit openDevCom();
            db->close();
            this->close();
        }
        else
        {
            QMessageBox::warning(this, "提示", "密码错误!");
            ui->txtPassward->selectAll();
            db->close();
        }
    }
    else if(passwardType == 1)
    {
        QString sql = "select ManagePWD from PWDManage";
        db->runSql(sql);
        db->query->next();
        if(ui->txtPassward->text() == db->query->value(0).toString())
        {
            emit openManage();
            db->close();
            this->close();
        }
        else
        {
            QMessageBox::warning(this, "提示", "密码错误!");
            ui->txtPassward->selectAll();
            db->close();
        }

    }
//    db->close();
//    this->close();
}
