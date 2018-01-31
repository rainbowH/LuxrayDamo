#include "wheelsdatathread.h"
#include <QVariant>
#include <QAxObject>
#include <QDebug>
#include <windows.h>

WheelsDataThread::WheelsDataThread(QString ExcelName)
{
    this->excelName = ExcelName;
//    OleInitialize(0);
}

void WheelsDataThread::importFromExcel()
{
    CoInitialize(NULL);
    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", false);
    QAxObject *work_books = excel.querySubObject("WorkBooks");

    work_books->dynamicCall("Open(const QString &)", excelName);
    //获取标题
    QVariant title = excel.property("Caption");

    QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
    QAxObject *work_sheets = work_book->querySubObject("Sheets");
    //获取工作表的数目
    int sheet_count = work_sheets->property("Count").toInt();

    QAxObject *sheet1 = work_book->querySubObject("Sheets(int)", 1);// 获取第一个表
    //获取表名
    QString sheet_title = sheet1->property("Name").toString();

    QAxObject *used_range = sheet1->querySubObject("UsedRange");
    QAxObject *rows = used_range->querySubObject("Rows");
    QAxObject *columns = used_range->querySubObject("Columns");

    int row_start = used_range->property("Row").toInt();  //获取起始行
    int column_start = used_range->property("Column").toInt();  //获取起始列
    int row_count = rows->property("Count").toInt();  //获取行数
    int column_count = columns->property("Count").toInt();  //获取列数


    //model = new QStandardItemModel(row_count, column_count);
    //获取单元格数据
    QVariant var = used_range->dynamicCall("Value");
    delete used_range;
    QVariantList list1 =  var.toList();

    //数据处理(进行倒置）




    work_books->dynamicCall("Close()");

    excel.dynamicCall("Close()");
    excel.dynamicCall("Quit(void)");

    CoUninitialize();
}

void WheelsDataThread::run()
{
    importFromExcel();
}
