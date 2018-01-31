#include "dlgdamageanalyze.h"
#include "ui_dlgdamageanalyze.h"

DlgDamageAnalyze::DlgDamageAnalyze(int type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDamageAnalyze)
{
    ui->setupUi(this);
    this->type = type;

    judgeDamageType();

}

void DlgDamageAnalyze::judgeDamageType()
{
    switch (this->type) {
    case 0:
        ui->txtType->setText("正常轮");
        break;
    case 1:
        ui->txtType->setText("偏心");
        break;
    case 2:
        ui->txtType->setText("椭圆");
        break;
    default:
        ui->txtType->setText("多边形");
        break;
    }
}

DlgDamageAnalyze::~DlgDamageAnalyze()
{
    delete ui;
}
