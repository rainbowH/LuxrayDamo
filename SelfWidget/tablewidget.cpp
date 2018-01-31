#include "tablewidget.h"
#include "ui_tablewidget.h"
#include <QStandardItemModel>
#include <QTableWidgetItem>
#include <QToolButton>
#include <QMouseEvent>

TableWidget::TableWidget(QWidget *parent, int rowmax) :
    QWidget(parent),
    rowMax(rowmax),
    index(0),
    count(0),
    isPressed(false),
    ui(new Ui::TableWidget)
{
    ui->setupUi(this);
    this->setLayout(ui->mainLayout);
    dlg = nullptr;
    ui->btnLast->setEnabled(false);
    ui->btnNext->setEnabled(false);
    db.connectDB();
    ui->tableWidget->setFont(QFont("楷体", 13));
    ui->tableWidget->horizontalHeader()->setFont(QFont("楷体", 13));

}

TableWidget::~TableWidget()
{
    delete ui;
    db.close();
    if(dlg != nullptr)
        delete dlg;
}

void TableWidget::onGetDatas(QString sql, int searchType)
{
    this->searchType = searchType;
    deleteAllItem();
    ui->tableWidget->clear();
    ui->btnLast->setEnabled(false);
    //按车号查询
    if(searchType == 0)
    {
        ui->tableWidget->setColumnCount(5);
        QStringList headers;
        headers <<"序号"<<"车号"<<"速度km/s"<<"日期"<<"详情";
        ui->tableWidget->setHorizontalHeaderLabels(headers);

        ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    }
    //按车轮号查询
    else if(searchType == 1)
    {
         ui->tableWidget->setColumnCount(6);
        QStringList headers;
        headers <<"序号"<<"车号"<<"车轮编号"<<"速度km/s"<<"日期"<<"详情";
        ui->tableWidget->setHorizontalHeaderLabels(headers);

        ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    }

    QString sql2 = sql;
    sql2 = sql2.replace(sql2.mid(7,sql2.indexOf("from")- 7), "count(*) ");

    db.runSql(sql2);
    db.query->next();
    count = db.query->value(0).toInt();
    if(count >rowMax)
        ui->btnNext->setEnabled(true);
    else
        ui->btnNext->setEnabled(false);

    db.runSql(sql);
    int rowCount = count>rowMax?rowMax:count;
    ui->tableWidget->setRowCount(rowCount);
    for(int i=0; i<rowCount; i++)
    {
        db.query->next();

        //编号
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
        //内容
        for(int j=1; j<ui->tableWidget->columnCount()-1; j++)
        {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(db.query->value(j-1).toString()));
        }
        //详情按钮
        QToolButton *btnDtl = new QToolButton(ui->tableWidget);
        connect(btnDtl, SIGNAL(clicked(bool)), this, SLOT(onClickedDtl()));
        btnDtl->installEventFilter(this);
        btnDtl->setText("查看");
        btnDtl->setStyleSheet(":hover{color: rgb(255, 0,0);"
                              "text-decoration:underline;}"
                              ":!hover{background: transparent}"
                              ":pressed{color: rgb(255, 200, 200);}");
        ui->tableWidget->setCellWidget(i, ui->tableWidget->columnCount()-1, btnDtl);
    }
    index += rowMax;
}
//点击查看
void TableWidget::onClickedDtl()
{
    int row =  ui->tableWidget->rowAt(mapFromGlobal(QCursor::pos()).y());
    QString train, datetime;
    if(searchType == 0)
    {
        train = ui->tableWidget->item(row, 1)->text();
        datetime = ui->tableWidget->item(row, 3)->text();
    }
    else if(searchType == 1)
    {
        train = ui->tableWidget->item(row, 1)->text();
        datetime = ui->tableWidget->item(row, 4)->text();
    }
    if(dlg == nullptr)
        dlg = new DlgTrainSearchShow(train, datetime);
    else
    {
        if(train != dlg->Train || datetime != dlg->DateTime)
        {
            delete dlg;
            dlg = new DlgTrainSearchShow(train, datetime);
        }
    }
    dlg->showMaximized();
}

bool TableWidget::eventFilter(QObject *watched, QEvent *event)
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
//上一页
void TableWidget::on_btnLast_clicked()
{
    ui->btnNext->setEnabled(true);
    index -= ui->tableWidget->rowCount();
    deleteAllItem();
    ui->tableWidget->setRowCount(rowMax);
    qDebug()<<"index="<<index;
    db.query->seek(index - 11);
    for(int i=0; i<rowMax; i++)
    {
        db.query->next();
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
        //内容
        for(int j=1; j<ui->tableWidget->columnCount()-1; j++)
        {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(db.query->value(j-1).toString()));
        }
        //详情按钮
        QToolButton *btnDtl = new QToolButton(ui->tableWidget);
        connect(btnDtl, SIGNAL(clicked(bool)), this, SLOT(onClickedDtl()));
        btnDtl->installEventFilter(this);
        btnDtl->setText("查看");
        btnDtl->setStyleSheet(":hover{color: rgb(255, 0,0);"
                              "text-decoration:underline;}"
                              ":!hover{background: transparent}"
                              ":pressed{color: rgb(255, 200, 200);}");
        ui->tableWidget->setCellWidget(i, ui->tableWidget->columnCount()-1, btnDtl);
    }
    if(index -rowMax <=0)
        ui->btnLast->setEnabled(false);
}
//下一页
void TableWidget::on_btnNext_clicked()
{
//    if(index +10 >= count)
//        ui->btnNext->setEnabled(false);
    ui->btnLast->setEnabled(true);

    deleteAllItem();
    int rowCount = count - index;
    ui->tableWidget->setRowCount(rowCount);
    for(int i=0; i<rowCount; i++)
    {
        qDebug()<<"start debug";
        db.query->next();
        //编号
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
        //内容
        for(int j=1; j<ui->tableWidget->columnCount()-1; j++)
        {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(db.query->value(j-1).toString()));
        }
        //详情按钮
        QToolButton *btnDtl = new QToolButton(ui->tableWidget);
        connect(btnDtl, SIGNAL(clicked(bool)), this, SLOT(onClickedDtl()));
        btnDtl->installEventFilter(this);
        btnDtl->setText("查看");
        btnDtl->setStyleSheet(":hover{color: rgb(255, 0,0);"
                              "text-decoration:underline;}"
                              ":!hover{background: transparent}"
                              ":pressed{color: rgb(255, 200, 200);}");
        ui->tableWidget->setCellWidget(i, ui->tableWidget->columnCount()-1, btnDtl);
    }
    index += rowCount;
    qDebug()<<"index = "<<index;
    if(index >= count)
        ui->btnNext->setEnabled(false);
    qDebug()<<"Next finished";

}
//释放所有item指针
void TableWidget::deleteAllItem()
{
    for(int i=0; i<ui->tableWidget->rowCount(); i++)
    {
        for(int j=0; j<ui->tableWidget->colorCount()-1; j++)
        {
            QTableWidgetItem *item = ui->tableWidget->item(i, j);
            delete item;
        }

        QToolButton *btn = (QToolButton *)ui->tableWidget->cellWidget(i, ui->tableWidget->columnCount()-1);
        delete btn;
    }
    ui->tableWidget->clearContents();
}
