#include "dlgmidedgeparadatas.h"
#include "ui_dlgmidedgeparadatas.h"

DlgMidEdgeParaDatas::DlgMidEdgeParaDatas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMidEdgeParaDatas)
{
    ui->setupUi(this);
    this->setLayout(ui->mainLayout);
}

DlgMidEdgeParaDatas::~DlgMidEdgeParaDatas()
{
    delete ui;
}
