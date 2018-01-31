#include "dlgmanagesearchstdwheel.h"
#include "ui_dlgmanagesearchstdwheel.h"
#include "QPushButton"
#include <QDebug>

DlgManageSearchStdWheel::DlgManageSearchStdWheel(QWidget *parent) :
    QDialog(parent),
    btnQSS(":hover{color: rgb(255, 0,0);"
           "text-decoration:underline;}"
           ":!hover{background: transparent}"
           ":pressed{color: rgb(255, 200, 200);}"),
    ui(new Ui::DlgManageSearchStdWheel)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);
    addBtnExport();
    addBtnDelete();
    this->setLayout(ui->mainLayout);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

//    btnQSS =":hover{color: rgb(255, 0,0);"
//            "text-decoration:underline;"
//            "cursor: pointer;}"
//            ":!hover{background: transparent};";
}

DlgManageSearchStdWheel::~DlgManageSearchStdWheel()
{
    delete ui;
}
//点击导出
void DlgManageSearchStdWheel::onExport()
{
    QToolButton *senderBtn =qobject_cast<QToolButton*>(sender());
    if(senderBtn == nullptr)
        return;
    int row =  ui->tableWidget->indexAt(QPoint(senderBtn->frameGeometry().x(),
                                               senderBtn->frameGeometry().y())).row();
    qDebug() <<"导出标准轮数据"<< row;

}
//点击删除
void DlgManageSearchStdWheel::onDelete()
{
    QToolButton *senderBtn =qobject_cast<QToolButton*>(sender());
    if(senderBtn == nullptr)
        return;
    int row =  ui->tableWidget->indexAt(QPoint(senderBtn->frameGeometry().x(),
                                               senderBtn->frameGeometry().y())).row();

    qDebug() << "删除标准轮数据"<< row;
}
//往表格中添加导出按钮
void DlgManageSearchStdWheel::addBtnExport()
{
    for(int i=0; i< ui->tableWidget->rowCount(); i++)
    {
        QToolButton *btnExport = new QToolButton(this);
//        btnExport->setStyleSheet(":hover{color: rgb(255, 0,0);"
//                                 "text-decoration:underline;}"
//                                 ":!hover{background: transparent};");
        btnExport->setStyleSheet(btnQSS);
        connect(btnExport, SIGNAL(clicked(bool)), this, SLOT(onExport()));
        btnExport->setText("导出");
        ui->tableWidget->setCellWidget(i, 3, btnExport);
    }
}
//往表格中添加删除按钮
void DlgManageSearchStdWheel::addBtnDelete()
{
    for(int i=0; i< ui->tableWidget->rowCount(); i++)
    {
        QToolButton *btnDelete = new QToolButton(this);
        btnDelete->setStyleSheet(btnQSS);
        connect(btnDelete, SIGNAL(clicked(bool)), this, SLOT(onDelete()));
        btnDelete->setText("删除");
        ui->tableWidget->setCellWidget(i, 4, btnDelete);
    }
}
