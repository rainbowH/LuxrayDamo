#include "dlgchecktrain.h"
#include "ui_dlgchecktrain.h"

DlgCheckTrain::DlgCheckTrain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCheckTrain)
{
    ui->setupUi(this);
    this->setLayout(ui->mainLayout);
}

DlgCheckTrain::~DlgCheckTrain()
{
    delete ui;
}
