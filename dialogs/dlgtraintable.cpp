#include "dlgtraintable.h"
#include "arithmetic.h"
#include "ui_dlgtraintable.h"
#include <QAbstractItemModel>
#include <QDebug>
#include <QAbstractTableModel>
#include <QFileDialog>
#include <QAxObject>

DlgTrainTable::DlgTrainTable(QString train, QString datetime, QWidget *parent) :
    QDialog(parent),
    Train(train),
    datetTime(datetime),
    ui(new Ui::DlgTrainTable)
{
    ui->setupUi(this);
    this->setLayout(ui->mainLayout);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);

    ui->lblTrain->setText(Train);
    ui->lblDateTime->setText(datetTime);
    initTable();
    setTableValues();
}

DlgTrainTable::~DlgTrainTable()

{
    delete ui;
}

void DlgTrainTable::initTable()
{
    ui->tableWidget->setSpan(0, 0, 2, 1);
    ui->tableWidget->setSpan(0, 2, 2, 1);
    ui->tableWidget->setSpan(0, 4, 2, 1);
    ui->tableWidget->setSpan(0, 5, 2, 1);
    ui->tableWidget->setSpan(0, 7, 2, 1);
    ui->tableWidget->setSpan(0, 8, 2, 1);
    ui->tableWidget->setSpan(0, 9, 2, 1);
    ui->tableWidget->setSpan(0, 10, 2, 1);
    ui->tableWidget->setSpan(0, 11, 2, 1);
    ui->tableWidget->setSpan(0, 12, 2, 1);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(10, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(11, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(12, QHeaderView::Stretch);
}

void DlgTrainTable::setTableValues()
{
    for(int i=0; i< 12; i++)
    {
        ui->tableWidget->setSpan(2*(i+1), 0, 2, 1);
        ui->tableWidget->setSpan(2*(i+1), 1, 2, 1);
        ui->tableWidget->setSpan(2*(i+1), 10, 2, 1);
        ui->tableWidget->setSpan(2*(i+1), 11, 4, 1);
        ui->tableWidget->setSpan(2*(i+1), 12, 24, 1);
    }
}

void DlgTrainTable::exportToExcel(QString filePath, QStandardItemModel *model)
{
    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", false);
    QAxObject *work_books = excel.querySubObject("WorkBooks");
    work_books->dynamicCall("Add");
//    work_books->dynamicCall("Open(const QString&)", "E:\\test.xlsx");
    excel.setProperty("Caption", "Qt Excel");
    QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
    QAxObject *work_sheets = work_book->querySubObject("Sheets");  //Sheets也可换用WorkSheets
    QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 1);
    work_sheet->setProperty("Name", "列车信息");  //设置工作表名称

    //合并标题单元格
        QString mergeStr = QString("A1:A2");
        QAxObject *merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
        merge_range->setProperty("MergeCells", true);

        mergeStr = "C1:C2";
        merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
        merge_range->setProperty("MergeCells", true);

        mergeStr = "E1:E2";
        merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
        merge_range->setProperty("MergeCells", true);

        mergeStr = "F1:F2";
        merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
        merge_range->setProperty("MergeCells", true);

        for(char c = 'H'; c<'N'; c++)
        {
            mergeStr = QString(c)+"1:"+QString(c)+"2";
            merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
            merge_range->setProperty("MergeCells", true);
        }


    //合并内容单元格
        int axleStart = 3, shelfStart = 3, trainStart = 3;
        qDebug() <<"rowCount = "<<ui->tableWidget->rowCount();
        int axleCount = (ui->tableWidget->rowCount() - 2)/2;
        for(int i=0; i< axleCount; i++)
        {
            //计轴列
            mergeStr = QString("A%1:A%2").arg(axleStart).arg(axleStart+1);
            merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
            merge_range->setProperty("MergeCells", true);
            //内测距列
            mergeStr = QString("B%1:B%2").arg(axleStart).arg(axleStart+1);
            merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
            merge_range->setProperty("MergeCells", true);
            //同轴轮径差列
            mergeStr = QString("K%1:K%2").arg(axleStart).arg(axleStart+1);
            merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
            merge_range->setProperty("MergeCells", true);
            //同架轮径差列
            if(i%2 == 1)
            {
                mergeStr = QString("L%1:L%2").arg(shelfStart).arg(shelfStart+3);
                merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
                merge_range->setProperty("MergeCells", true);
                shelfStart += 4;
            }

            axleStart += 2;

        }
        //同车轮径差列
        mergeStr = QString("M%1:M%2").arg(trainStart).arg(ui->tableWidget->rowCount());
        merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
        merge_range->setProperty("MergeCells", true);


//写入数据
    QList<QList<QVariant>> tables;
    for(int i=0; i<model->rowCount(); i++)
    {
        QList<QVariant> list;
        for(int j=0; j<model->columnCount(); j++)
        {
            list.append(model->data(model->index(i, j)));
        }
        tables.append(list);
    }

    QString datasRangeStr = QString("A1:M%1").arg(model->columnCount());
    QAxObject *range = work_sheet->querySubObject("Range(const QString&)", datasRangeStr);
    range->setProperty("Value", Arithmetic::TableToVariant(tables));

    filePath.replace('/', '\\');
    qDebug() << filePath;
    work_book->dynamicCall("SaveAs (const QString&,int,const QString&,const QString&,bool,bool)",
                             filePath,56,QString(""),QString(""), false, false);
    work_book->dynamicCall("Close(Boolean)", false);
    excel.dynamicCall("Quit(void)");
}
//数据导出
void DlgTrainTable::on_btnDatasExport_clicked()
{
    QStandardItemModel *model =(QStandardItemModel *) ui->tableWidget->model();
    qDebug()<<model->rowCount()<<model->data(model->index(1, 1)).toString();

    QString filePath = QFileDialog::getSaveFileName(this, "存储为", QString("./%1-监测数据-%2.xls").arg(Train)
                                                    .arg(datetTime), "表格文件(*.xls)");
    if(filePath.trimmed() == "")
        return;
    exportToExcel(filePath, model);
}
