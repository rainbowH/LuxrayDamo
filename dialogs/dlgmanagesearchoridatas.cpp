#include "dlgmanagesearchoridatas.h"
#include "ui_dlgmanagesearchoridatas.h"

DlgManageSearchOriDatas::DlgManageSearchOriDatas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgManageSearchOriDatas)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);
    ui->tabWidget->setStyleSheet("alignment: center");

    ui->tab_maching->setLayout(ui->LayoutMaching);
    ui->tab_notmaching->setLayout(ui->LayoutNotMaching);
    this->setLayout(ui->mainLayout);
}

DlgManageSearchOriDatas::~DlgManageSearchOriDatas()
{
    delete ui;
}
