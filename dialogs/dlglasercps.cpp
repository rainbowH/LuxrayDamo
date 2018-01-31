#include "dlglasercps.h"
#include "ui_dlglasercps.h"

DlgLaserCPS::DlgLaserCPS(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgLaserCPS)
{
    ui->setupUi(this);
    this->setLayout(ui->mainLayout);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

DlgLaserCPS::~DlgLaserCPS()
{
    delete ui;
}
