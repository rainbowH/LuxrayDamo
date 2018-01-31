#include "importthread.h"
#include <QAxObject>
#include <windows.h>
#include <QTableWidgetItem>
#include <QModelIndex>
#include <QList>
#include <QMetaType>
#include <QDebug>
#include <QMutexLocker>


ImportThread::ImportThread(QString ExcelName)
{
    this->ExcelName = ExcelName;
    //将QList<double>和QList<QList<QString>>注册为可发送的信息
    qRegisterMetaType<QList<double>>("QList<double>");
    qRegisterMetaType<QList<QList<QVariant>>>("QList<QList<QVariant>>");
}

ImportThread::~ImportThread()
{
    this->exit();
}

void ImportThread::importFromExcel()
{
    // ui->progressBar->setValue(10);
    CoInitialize(NULL);


      QAxObject excel("Excel.Application");
      excel.setProperty("Visible", false);
      QAxObject *work_books = excel.querySubObject("WorkBooks");

      emit labelText("正在打开文件");
      emit progress(10);
      work_books->dynamicCall("Open(const QString &)", QString(ExcelName));
      emit progress(20);
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

      model = new QStandardItemModel(row_count, column_count);

      emit progress(50);
      //获取单元格数据
      QVariant var = used_range->dynamicCall("Value");
      delete used_range;

      QVariantList list1 =  var.toList();
      for(int j=0; j<column_count; j++)
      {
          QVariantList tmp;
          for(int i=0; i<row_count; i++)
          {
              QVariantList list2 = list1.at(i).toList();
              tmp << list2.at(j);
          }
          datasTable.append(tmp);
      }

       emit progress(70);
       emit sendDatas(datasTable);

      work_books->dynamicCall("Close()");

      excel.dynamicCall("Close()");
      excel.dynamicCall("Quit(void)");

      //emit endLoading(row_count, column_count, sheet_title, valueList);

      CoUninitialize();
}

void ImportThread::run()
{
    QMutexLocker locker(&mutex);
    importFromExcel();
}
