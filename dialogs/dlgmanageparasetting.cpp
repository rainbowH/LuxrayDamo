#include "dlgmanageparasetting.h"
#include "ui_dlgmanageparasetting.h"

DlgManageParaSetting::DlgManageParaSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgManageParaSetting)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);
    ui->groupEddy->setLayout(ui->LayoutEddy);
    ui->groupLaser7200A->setLayout(ui->gridLayout);
    ui->groupLaser7200B->setLayout(ui->gridLayout_2);
    ui->group7200->setLayout(ui->Layout7200);

    this->setLayout(ui->mainLayout);
}

DlgManageParaSetting::~DlgManageParaSetting()
{
    delete ui;
}
