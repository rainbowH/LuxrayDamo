#include "dlgdevicelist.h"
#include "ui_dlgdevicelist.h"
#include <QDebug>

DlgDeviceList::DlgDeviceList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDeviceList)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);
    initTableWidget();
    initMenu();
    resetNo();
    this->setLayout(ui->horizontalLayout);
}

DlgDeviceList::~DlgDeviceList()
{
    delete ui;
}

void DlgDeviceList::initMenu()
{
    customMenu = new QMenu();
    QAction *actionInsertRow = new QAction("插入行");
    connect(actionInsertRow, SIGNAL(triggered(bool)), this, SLOT(onInsertRow()));
    QAction *actionDeleteRow = new QAction("删除行");
    connect(actionDeleteRow, SIGNAL(triggered(bool)), this, SLOT(onDeleteRow()));
    customMenu->addAction(actionInsertRow);
    customMenu->addAction(actionDeleteRow);
}


void DlgDeviceList::initTableWidget()
{
    ui->tableWidget->setColumnCount(5);
    eddyEnd = 10;
    elemchEnd = 10;
    ui->tableWidget->setSpan(0, 0, 1, 5);
    ui->tableWidget->item(0, 0)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->setSpan(12, 0, 1, 5);
    ui->tableWidget->setItem(12, 0, new QTableWidgetItem("电机"));
    ui->tableWidget->item(12, 0)->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setFont(QFont("楷体", 12));
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}

void DlgDeviceList::resetNo()
{
    for(int i=2; i< eddyEnd+2; i++)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i-1)));
    }
    for(int i= eddyEnd +4, elemNo =1; i<=eddyEnd+elemchEnd+2; i++, elemNo++)
    {
        qDebug() <<QString("i=%1, eddyEnd=%2, elemchEnd=%3").arg(i).arg(eddyEnd).arg(elemchEnd);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(elemNo)));
    }
}


void DlgDeviceList::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    customMenu->exec(QCursor::pos());
}
//插入列
void DlgDeviceList::onInsertRow()
{
    int row = ui->tableWidget->currentRow();

    if(row <= eddyEnd+2)
        eddyEnd ++;
    else if(row <= elemchEnd+eddyEnd+4)
        elemchEnd ++;

    ui->tableWidget->insertRow(row);
    resetNo();
}
//删除列
void DlgDeviceList::onDeleteRow()
{
    int row = ui->tableWidget->currentRow();

    if(row <= eddyEnd+2)
        eddyEnd --;
    else if(row <= elemchEnd+eddyEnd+4)
        elemchEnd --;

    ui->tableWidget->removeRow(row);
    resetNo();
}
