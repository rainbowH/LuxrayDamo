#include "dlgmideddydatas.h"
#include "ui_dlgmideddydatas.h"

DlgMidEddyDatas::DlgMidEddyDatas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMidEddyDatas)
{
    ui->setupUi(this);
}

DlgMidEddyDatas::~DlgMidEddyDatas()
{
    delete ui;
}
