#include "dlglogin.h"
#include "ui_dlglogin.h"
#include "dbsqlite.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QLibrary>

DlgLogin::DlgLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgLogin)
{
    ui->setupUi(this);
    ui->Txt_ID->setText("lisi");
    ui->Txt_PWD->setText("lisi");
    ui->Radio_NotMng->setChecked(true);
}

DlgLogin::~DlgLogin()
{
    delete ui;
}

void DlgLogin::on_Btn_Login_clicked()
{
    if(ui->Txt_ID->text() == "")
    {
        QMessageBox::warning(this, "警告", "用户名不能为空");
        return ;
    }

    if(ui->Txt_PWD->text() == "")
    {
        QMessageBox::warning(this, "警告", "密码不能为空");
        return ;
    }

    DBSqlite *dbsqlite = new DBSqlite("wheelsInfo.db");
    dbsqlite->connectDB();
    QString sql = "select * from UsersInfo where 用户名 = '" + ui->Txt_ID->text()+ "'";
    if(!dbsqlite->runSql(sql))
        qDebug() << "failed to run sql sentence";

    if(!dbsqlite->query->next())
    {
        QMessageBox::about(this, "警告", "查无此人");
        return;
    }

    QString passward = dbsqlite->getValue("用户名", ui->Txt_ID->text(), "密码").toString();
    bool isManager = dbsqlite->getValue("用户名", ui->Txt_ID->text(), "管理员").toBool();

    qDebug() << passward <<" "<<isManager;

    dbsqlite->close();

    if(passward == ui->Txt_PWD->text())
    {
        if((ui->Radio_Mng->isChecked() && isManager) || (ui->Radio_NotMng->isChecked() && !isManager))
        {
            MainWindow *w = new MainWindow(ui->Txt_ID->text(), isManager);
            connect(w, SIGNAL(loginShowed()),this, SLOT(show()));
            this->hide();
            w->show();
        }
        else
        {
            QMessageBox::warning(this, "警告", "登录身份错误");
            return;
        }
    }
    else
    {
         QMessageBox::warning(this, "警告", "密码错误");
         return;
    }
}

void DlgLogin::on_Btn_Cancel_clicked()
{
    this->close();
}

