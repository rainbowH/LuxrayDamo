#include "wheelsinfothread.h"
#include <QMetaType>
#include <windows.h>
#include <QDebug>
#include <QAxObject>
#include <QList>

WheelsInfoThread::WheelsInfoThread(QString name)
{
    fileName = name;
    qRegisterMetaType<QList<QList<QVariant>>>("QList<QList<QVariant>>");

}

void WheelsInfoThread::importFromExcel()
{
    CoInitialize(NULL);
    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", false);
    QAxObject *work_books = excel.querySubObject("WorkBooks");

    work_books->dynamicCall("Open(const QString &)", fileName);
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

//    model = new QStandardItemModel(row_count, column_count);

    //获取单元格数据
    QVariant var = used_range->dynamicCall("Value");
    delete used_range;

    QVariantList list1 =  var.toList();
    //行列不互换
    for(int i=0; i< list1.length(); i++)
    {
       QVariantList list2 = list1.at(i).toList();

       datasTable.append(list2);
    }
    //行列互换
//    for(int j=0; j<column_count; j++)
//    {
//        QVariantList tmp;
//        for(int i=0; i<row_count; i++)
//        {
//            QVariantList list2 = list1.at(i).toList();
//            tmp << list2.at(j);
//        }
//        datasTable.append(tmp);
//    }

    emit sendDatas(datasTable);

    work_books->dynamicCall("Close()");

    excel.dynamicCall("Close()");
    excel.dynamicCall("Quit(void)");

    CoUninitialize();
    //emit endLoading(row_count, column_count, sheet_title, valueList);
}

void WheelsInfoThread::run()
{
    importFromExcel();
}
