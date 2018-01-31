#include "dlgcheckdatas.h"
#include "ui_dlgcheckdatas.h"
#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QDebug>


DlgCheckDatas::DlgCheckDatas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCheckDatas)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);

    ui->SearchWidget->setLayout(ui->searchLayout);
    this->setLayout(ui->mainLayout);

//    timeBox = new QComboBox(this);
    datetime = new QDateTimeEdit(this);
    datetime->setCalendarPopup(true);
    datetime->setDateTime(QDateTime::currentDateTime());
    datetime->setDisplayFormat("yyyy-MM-dd HH:mm:ss");

//    timeBox->setVisible(false);
    datetime->setVisible(false);

//    QStandardItemModel *model = new QStandardItemModel();
    QStringList titles;
    titles << "时间" << "检测人" <<"车轮型号"<<"车轮材质属性"<<"检测情况";
//    model->setColumnCount(5);
//    model->setHorizontalHeaderLabels(titles);
//    model->insertRows(0, 10);

//    qDebug() << model->rowCount() << " " << model->columnCount();

    database = new DBMySql("hubtesting");
    database->connectDB();
//    QSqlTableModel *model = new QSqlTableModel();
//    model->setTable("wheels");
//    model->select();

//    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    设置所有单元格不可编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->Combox_Condition->setCurrentIndex(1);

}

DlgCheckDatas::~DlgCheckDatas()
{
    database->close();
    delete ui;
}
//选择条件发生改变
void DlgCheckDatas::on_Combox_Condition_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "按检测时间查找")
    {
        ui->searchLayout->replaceWidget(ui->Txt_SearchContent, datetime);
        ui->Txt_SearchContent->clear();
        ui->Txt_SearchContent->setVisible(false);
        datetime->setVisible(true);
    }

    else
    {
        if(datetime->isVisible())
        {
            ui->searchLayout->replaceWidget(datetime, ui->Txt_SearchContent);
            ui->Txt_SearchContent->setVisible(true);
            datetime->setVisible(false);
        }
    }
}
//开始搜索
void DlgCheckDatas::on_Btn_Search_clicked()
{
    QString sql;
    model = new QSqlTableModel(this, database->database);
    model->setTable("origdatas");
    model->select();
    if(ui->Combox_Condition->currentText() == "按检测时间查找" && datetime->text() != "")
    {
        QString condition = datetime->text();
        sql = "检测时间 = '"+condition+"'";
        model->setFilter(sql);
        model->select();
    }
    else if(ui->Combox_Condition->currentText() == "按车号查找" && ui->Txt_SearchContent->text() != "")
    {
        QString condition = ui->Txt_SearchContent->text();
        sql = "车号 = '"+condition+"'";
        model->setFilter(sql);
        model->select();
    }
    else if(ui->Combox_Condition->currentText() == "按检测人查找"&& ui->Txt_SearchContent->text() != "")
        {
        QString condition = ui->Txt_SearchContent->text();
        sql = "检测人员 = '"+condition+"'";
        model->setFilter(sql);
        model->select();
    }
    else if(ui->Combox_Condition->currentText() == "按车轮RFID查找"&& ui->Txt_SearchContent->text() != "")
        {
        QString condition = ui->Txt_SearchContent->text();
        sql = "车轮编号 = '"+condition+"'";
        model->setFilter(sql);
        model->select();
    }

    ui->tableView->setModel(model);
}
