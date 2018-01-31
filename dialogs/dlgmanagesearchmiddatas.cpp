#include "dlgmanagesearchmiddatas.h"
#include "ui_dlgmanagesearchmiddatas.h"
#include "dialogs/dlgmideddydatas.h"
#include "dialogs/dlgmidedgeparadatas.h"

DlgManageSearchMidDatas::DlgManageSearchMidDatas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgManageSearchMidDatas)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);
    this->setLayout(ui->mainLayout);
    addOpenBtn();
}

DlgManageSearchMidDatas::~DlgManageSearchMidDatas()
{
    delete ui;
}
//为表格添加打开按钮
void DlgManageSearchMidDatas::addOpenBtn()
{
    for(int i=0; i<ui->tableWidget->rowCount(); i++)
    {
        for(int j=5; j<ui->tableWidget->columnCount(); j++)
        {
            QToolButton *btnOpen = new QToolButton(this);
            connect(btnOpen, SIGNAL(clicked(bool)), this, SLOT(onOpen()));
            btnOpen->setText("打开");
            ui->tableWidget->setCellWidget(i, j, btnOpen);
        }
    }
}

void DlgManageSearchMidDatas::onOpen()
{
    QToolButton *senderBtn =qobject_cast<QToolButton*>(sender());
    if(senderBtn == nullptr)
        return;
    int row =  ui->tableWidget->indexAt(QPoint(senderBtn->frameGeometry().x(),
                                               senderBtn->frameGeometry().y())).row();
    int column = ui->tableWidget->indexAt(QPoint(senderBtn->frameGeometry().x(),
                                                 senderBtn->frameGeometry().y())).column();
    if(ui->tableWidget->horizontalHeaderItem(column)->text() == "电涡流")
    {
        DlgMidEddyDatas *dlg = new DlgMidEddyDatas(this);
        dlg->show();
    }
    else if (ui->tableWidget->horizontalHeaderItem(column)->text() == "轮缘参数")
    {
        DlgMidEdgeParaDatas *dlg = new DlgMidEdgeParaDatas(this);
        dlg->show();
    }
}
