#include "dlghistorydatas.h"
#include "ui_dlghistorydatas.h"
#include "dialogs/dlgtrainsearchshow.h"
#include "dialogs/dlgwheelshow.h"
#include <QThread>
#include <QDebug>
#include "dbsqlite.h"

DlgHistoryDatas::DlgHistoryDatas(QWidget *parent) :
    QDialog(parent),
    btnQSS(":hover{color: rgb(255, 0,0);"
           "text-decoration:underline;}"
           ":!hover{background: transparent}"
           ":pressed{color: rgb(255, 200, 200);}"),
    ui(new Ui::DlgHistoryDatas)
{
    ui->setupUi(this);
    this->setLayout(ui->mainLayout);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);

    ui->dateStart->setDate(QDate::fromString("2015-03-14"));
    ui->dateEnd->setDate(QDate::currentDate());
    ui->lineEdit->setText("D9870");
    ui->tableWidget->rowMax = 11;
    ui->dateStart->setMaximumDate(QDate::currentDate());
    ui->dateEnd->setMaximumDate(QDate::currentDate());

    connect(this, SIGNAL(sqlSentence(QString,int)), ui->tableWidget, SLOT(onGetDatas(QString,int)));
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

//    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{font: 8pt}");
//    ui->tableWidget->setFont(QFont("楷体", 13));

    dlg = nullptr;
    QFile file(":/QSS/dlghistorydatas.qss");
    file.open(QFile::ReadOnly);
    this->setStyleSheet(file.readAll());
    file.close();
//    ui->tableWidget->verticalHeader()->hide();
}

DlgHistoryDatas::~DlgHistoryDatas()
{
    delete ui;
}
//通过车号进行搜索
void DlgHistoryDatas::searchTrain(QString TrainNum)
{
//    DBSqlite db;
//    db.connectDB();
    QString sql = QString("select 车号, 速度, 检测时间 from TrainInfo where 车号='%1' and "
                          "检测时间>datetime('%2') and 检测时间<datetime('%3')").arg(TrainNum)
            .arg(ui->dateStart->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
            .arg(ui->dateEnd->dateTime().toString("yyyy-MM-dd hh:mm:ss"));
    emit sqlSentence(sql, 0);
//    db.runSql(sql);
//    ui->tableWidget->setColumnCount(5);
//    QStringList headers;
//    headers <<"序号"<<"车号"<<"速度km/s"<<"日期"<<"详情";
//    ui->tableWidget->setHorizontalHeaderLabels(headers);

//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

//    int rowIndex = 0;
//    while(db.query->next())
//    {
//        ui->tableWidget->insertRow(rowIndex);
//        //编号
//        ui->tableWidget->setItem(rowIndex, 0, new QTableWidgetItem(QString::number(rowIndex+1)));
//        //信息
//        for(int i=1; i< 4; i++)
//        {
//            ui->tableWidget->setItem(rowIndex, i, new QTableWidgetItem(db.query->value(i-1).toString()));
//        }
//        //详情按钮
//        QToolButton *btnDtl = new QToolButton(this);
//        connect(btnDtl, SIGNAL(clicked(bool)), this, SLOT(onClickedDtl()));
//        btnDtl->installEventFilter(this);
//        btnDtl->setText("查看");
//        btnDtl->setStyleSheet(btnQSS);
//        ui->tableWidget->setCellWidget(rowIndex, 4, btnDtl);
//        rowIndex++;
//    }

//    db.close();
}
//通过车轮号进行搜索
void DlgHistoryDatas::searchWheel(QString wheelNum)
{
//    DBSqlite db;
//    db.connectDB();
    QString sql = QString("select 车号, 车轮编号, 速度, 检测时间 from WheelModel where 车轮编号='%1' and "
                          "检测时间>=datetime('%2') and 检测时间<=datetime('%3')").arg(wheelNum)
            .arg(ui->dateStart->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
            .arg(ui->dateEnd->dateTime().toString("yyyy-MM-dd hh:mm:ss"));

    emit sqlSentence(sql, 1);
//    qDebug() <<sql;
//    db.runSql(sql);
//    ui->tableWidget->setColumnCount(6);
//    QStringList headers;
//    headers <<"序号"<<"车号"<<"车轮编号"<<"速度km/s"<<"日期"<<"详情";
//    ui->tableWidget->setHorizontalHeaderLabels(headers);

//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);

//    int rowIndex = 0;
//    while(db.query->next())
//    {
//        ui->tableWidget->insertRow(rowIndex);
//        //编号
//        ui->tableWidget->setItem(rowIndex, 0, new QTableWidgetItem(QString::number(rowIndex+1)));
//        //信息
//        for(int i=1; i< 5; i++)
//        {
//            ui->tableWidget->setItem(rowIndex, i, new QTableWidgetItem(db.query->value(i-1).toString()));
//        }
//        //详情按钮
//        QToolButton *btnDtl = new QToolButton(this);
//        connect(btnDtl, SIGNAL(clicked(bool)), this, SLOT(onClickedDtl()));
//        btnDtl->installEventFilter(this);
//        btnDtl->setText("查看");
//        btnDtl->setStyleSheet(btnQSS);
//        ui->tableWidget->setCellWidget(rowIndex, 5, btnDtl);
//        rowIndex++;
//    }

//    db.close();
}
//事件处理机制
bool DlgHistoryDatas::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::HoverEnter)
    {
        setCursor(Qt::PointingHandCursor);
    }
    else if(event->type() == QEvent::HoverLeave)
    {
        setCursor(Qt::ArrowCursor);
    }
}


//void DlgHistoryDatas::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
//{
//    if(item->column() == 4 && item->text() =="查看")
//    {
//        DlgTrainSearchShow *dlg = new DlgTrainSearchShow;
//        dlg->show();
//    }
//    else if(item->column() == 5 && item->text() == "查看")
//    {
//        DlgTrainSearchShow *dlg = new DlgTrainSearchShow;

//        dlg->show();
//    }
//}
//进行搜索
void DlgHistoryDatas::on_btnSearch_clicked()
{
    if(ui->lineEdit_2->text().trimmed()!="")
    {
        searchWheel(ui->lineEdit_2->text().trimmed());
    }
    else if(ui->lineEdit->text().trimmed()!="")
    {
        searchTrain(ui->lineEdit->text().trimmed());
    }

}

void DlgHistoryDatas::onClickedDtl()
{
    if(dlg == nullptr)
        dlg = new DlgTrainSearchShow;
    dlg->showFullScreen();
}

//void DlgHistoryDatas::on_tableWidget_itemClicked(QTableWidgetItem *item)
//{
//    if(item->column() == 4 && item->text() =="查看")
//    {
//        DlgTrainSearchShow *dlg = new DlgTrainSearchShow;
//        dlg->show();
//    }
//    else if(item->column() == 5 && item->text() == "查看")
//    {
//        DlgTrainSearchShow *dlg = new DlgTrainSearchShow;

//        dlg->show();
//    }
//}
