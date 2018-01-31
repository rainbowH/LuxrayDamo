#include "dlgwheeldtl.h"
#include "ui_dlgwheeldtl.h"
#include <QDebug>

DlgWheelDtl::DlgWheelDtl(DBSqlite *db, QString RFID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgWheelDtl)
{
    ui->setupUi(this);
    QSqlQuery *query;
    if(db->runSql("select * from WheelModel where 车轮编号 ='"+RFID+"'"))
        query = db->query;
    query->next();
    initDatas(query);

}

DlgWheelDtl::~DlgWheelDtl()
{
    delete ui;
}

void DlgWheelDtl::setAllLineEnable(bool enable)
{
    ui->lineAxle->setEnabled(enable);
    ui->lineGr1->setEnabled(enable);
    ui->lineGr2->setEnabled(enable);
    ui->lineInLength->setEnabled(enable);
    ui->lineRadiuJump->setEnabled(enable);
    ui->lineRFID->setEnabled(enable);
    ui->lineSd1->setEnabled(enable);
    ui->lineSd2->setEnabled(enable);
    ui->lineSh->setEnabled(enable);
    ui->lineTrain->setEnabled(enable);
    ui->lineWheelLoc->setEnabled(enable);
}

void DlgWheelDtl::on_btnEdit_clicked()
{
    setAllLineEnable(true);
    ui->lineTrain->setFocus();
}

void DlgWheelDtl::initDatas(QSqlQuery *query)
{
    ui->lineTrain->setText(query->value("车号").toString());
    ui->lineAxle->setText(query->value("计轴").toString());
    ui->lineWheelLoc->setText(query->value("车轮位置").toString());
    ui->lineRFID->setText(query->value("车轮编号").toString());
    ui->lineRadiuJump->setText(query->value("径跳").toString());
    ui->lineSh->setText(query->value("Sh").toString());
    ui->lineSd1->setText(query->value("Sd1").toString());
    ui->lineSd2->setText(query->value("Sd2").toString());
    ui->lineGr1->setText(query->value("Gr1").toString());
    ui->lineGr2->setText(query->value("Gr2").toString());
    ui->lineInLength->setText(query->value("内测距").toString());
}
