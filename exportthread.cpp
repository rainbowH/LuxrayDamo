#include "exportthread.h"
#include "dbsqlite.h"
#include "arithmetic.h"
#include <QFile>
#include <windows.h>
#include "objects/wheels.h"

ExportThread::ExportThread(int dataFlag, Train *train1, QString path)
{
    this->dataFlag = dataFlag;
    this->pubPath =path;
    this->train = train1;

    db = new DBSqlite();
    qDebug() <<"db has newed";
    QFile file(path);
    if(file.exists())
        file.remove();
}

void ExportThread::run()
{
    exportDBBase();
/*
    //存储基本信息
    exportBaseDatas();
    //存储2D激光数据
        //得到2D激光路径
    QString laserPath = pubPath +"/"+train->RFID+"2D激光数据";
        //创建文件夹并记录数据
    if(Arithmetic::isDirExist(laserPath))
    {
        for(int i=0; i<train->axleList.length(); i++)
        {
            for(int j=0; j< train->axleList.at(i)->wheelsList.length(); j++)
            {
                Wheels *tmpWheel = train->axleList.at(i)->wheelsList.at(j);
                exportLaserDatas(laserPath, tmpWheel);
            }
        }
    }
*/
}

void ExportThread::exportBaseDatas()
{
    QString excelName = pubPath +"/"+ train->RFID+".xls";
    CoInitialize(NULL);
    emit progressChanged(10);
    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", false);
    QAxObject *work_books = excel.querySubObject("WorkBooks");
    work_books->dynamicCall("Add");
//    work_books->dynamicCall("Open(const QString&)", "E:\\test.xlsx");
    excel.setProperty("Caption", "Qt Excel");
    QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
    QAxObject *work_sheets = work_book->querySubObject("Sheets");  //Sheets也可换用WorkSheets
    QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 1);
//    //删除工作表（删除第一个）
//    QAxObject *first_sheet = work_sheets->querySubObject("Item(int)", 1);
//    first_sheet->dynamicCall("delete");

//    //插入工作表（插入至最后一行）
//    int sheet_count = work_sheets->property("Count").toInt();  //获取工作表数目
//    QAxObject *last_sheet = work_sheets->querySubObject("Item(int)", sheet_count);
//    QAxObject *work_sheet = work_sheets->querySubObject("Add(QVariant)", last_sheet->asVariant());
//    last_sheet->dynamicCall("Move(QVariant)", work_sheet->asVariant());
    emit progressChanged(20);
    work_sheet->setProperty("Name", "车轮信息");  //设置工作表名称
    QList<QString> titles;

    //=========1A=======2B========3C======4D==========5E===========6F========7G=======8H====
    titles <<"计轴"<<"车轮编号"<<"轮径"<<"径向跳动"<<"轮缘高度Sh"<<"轮缘厚度Sd"<<"Gr值"<<"内侧距";
    //=========1A=======2B========3C======4D==========5E===========6F========7G=======8H====

    //添加标题
    for(int i=1; i<= titles.length(); i++)
    {
        QAxObject *cell = work_sheet->querySubObject("Cells(int, int)", 1, i);
        cell->setProperty("Value", titles.at(i-1));
        cell->setProperty("WrapText", true);
    }
    //填写内容
        //合并单元格项
    QAxObject *merge_range;
    int axleStart = 2, axleEnd =2;
    int wheelStart = 2, wheelEnd = 2;
    for(int i= 0; i<train->axleList.length(); i++)
    {
        Axle *tmpAxle = train->axleList.at(i);
        axleEnd = axleStart+1;
        QString mergeStr = QString("A%1:A%2").arg(axleStart).arg(axleEnd);
        merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
        merge_range->setProperty("MergeCells", true);
        mergeStr = QString("H%1:H%2").arg(axleStart).arg(axleEnd);
        merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
        merge_range->setProperty("MergeCells", true);
        //写入计轴号
        QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", axleStart, 1);
        cell->setProperty("Value", QString("计轴%1").arg(tmpAxle->No));
        //内侧距
        cell = work_sheet->querySubObject("Cells(int, int)", axleStart, 8);
        cell->setProperty("Value", tmpAxle->Length);

        axleStart = axleEnd + 1;


        //填写轮子信息
        for(int i=0; i< tmpAxle->wheelsList.length(); i++)
        {
            Wheels *tmpWheel = tmpAxle->wheelsList.at(i);
            qDebug() << "开始填写轮子数据";
            //车轮编号
            cell = work_sheet->querySubObject("Cells(int, int)", wheelStart, 2);
            cell->setProperty("Value", tmpWheel->RFID);
            qDebug() <<"车轮编号填完了";
            //轮径
            cell = work_sheet->querySubObject("Cells(int, int)", wheelStart, 3);
            cell->setProperty("Value", tmpWheel->mean_r*2);
            qDebug() <<"车轮轮径填完了";
            //径跳
            cell = work_sheet->querySubObject("Cells(int, int)", wheelStart, 4);
            cell->setProperty("Value", tmpWheel->radiuRunOut);
            qDebug() <<"车轮径跳填完了";
//            //Sh
//            qDebug() << "Sh = " << tmpWheel->laser->Sh;
//            cell = work_sheet->querySubObject("Cells(int, int)", wheelStart, 5);
//            cell->setProperty("Value", QString("%1").arg(tmpWheel->laser->Sh));
//            qDebug() <<"Sh填完了";
//            //Sd
//            cell = work_sheet->querySubObject("Cells(int, int)", wheelStart, 6);
//            cell->setProperty("Value", QString("%1;%2").arg(tmpWheel->laser->Sd1).arg(tmpWheel->laser->Sd2));
//            qDebug() <<"Sd填完了";
//            //Gr
//            cell = work_sheet->querySubObject("Cells(int, int)", wheelStart, 7);
//            cell->setProperty("Value", QString("%1;%2").arg(tmpWheel->laser->Gr1).arg(tmpWheel->laser->Gr2));
//            qDebug() <<"Gr填完了";

            wheelStart = wheelEnd +1;
            wheelEnd = wheelStart;
        }
        qDebug() << "轮子数据填完了";
    }

//    emit progressChanged(30);
//    for(int i=0; i< train->axleList.length(); i++)
//    {
//        Axle *tmpAxle = train->axleList.at(i);
//        for(int j=0; j<tmpAxle->wheelsList.length(); j++)
//        {
//            emit progressChanged((i+1)*(j+1)/(train->axleList.length()*tmpAxle->wheelsList.length())*60);

//            Wheels *tmpWheel = tmpAxle->wheelsList.at(j);
//            QList<QList<double>> eddydatas;
//            eddydatas << tmpWheel->eddy->Data1 << tmpWheel->eddy->Data2 << tmpWheel->eddy->Data3
//                      << tmpWheel->eddy->Data4 << tmpWheel->eddy->DataJointed;
//            int column = eddydatas.length(); int row = tmpWheel->eddy->DataJointed.length();
//            wheelend = wheelstart + row;
//            QList<QList<QVariant>> tables = Arithmetic::TableDoubleToVariant(eddydatas);
//            //写入数据
//            QString datasRangeStr = QString("D%1:H%2").arg(wheelstart).arg(wheelend);
//            QAxObject *range = work_sheet->querySubObject("Range(const QString&)", datasRangeStr);
//            range->setProperty("Value", Arithmetic::TableToVariant(Arithmetic::reverseTable(tables)));
//            //合并RFID单元格
//            QString mergeStr = QString("C%1:C%2").arg(wheelstart).arg(wheelend);
//            QAxObject *merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
//            merge_range->setProperty("MergeCells", true);
//            QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", wheelstart, 3);
//            cell->setProperty("Value", tmpAxle->wheelsList.at(j)->RFID);
//            wheelstart = wheelend+1;

//        }
//        //合并 Axle的单元格
//        axleend = wheelend;
//        QString mergeStr = QString("B%1:B%2").arg(axlestart).arg(axleend);
//        QAxObject *merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
//        merge_range->setProperty("MergeCells", true);
//        QAxObject *cell = work_sheet->querySubObject("Cells(int, int)", axlestart, 2);
//        QString axlename = QString("计轴%1").arg(tmpAxle->No);
//        cell->setProperty("Value", axlename);
//        axlestart = axleend+1;

//    }

    //合并单元格
//    QString merge_cell;
//    merge_cell.append(QChar('B'));
//    merge_cell.append(QString::number(2));
//    merge_cell.append(":");
//    merge_cell.append(QChar('C'));
//    merge_cell.append(QString::number(8));
//    QAxObject *merge_range = work_sheet->querySubObject("Range(const QString&)", merge_cell);
//            merge_range->setProperty("MergeCells", true);

    emit progressChanged(95);
    excelName.replace('/', '\\');
    qDebug() << excelName;
    work_book->dynamicCall("SaveAs (const QString&,int,const QString&,const QString&,bool,bool)",
                             excelName,56,QString(""),QString(""), false, false);
    work_book->dynamicCall("Close(Boolean)", false);
    excel.dynamicCall("Quit(void)");
    emit progressChanged(100);

    CoUninitialize();
}

void ExportThread::exportEddyDatas(QString excelName, Wheels *wheel)
{
    //    CoInitialize(NULL);
        emit progressChanged(10);
        QAxObject excel("Excel.Application");
        excel.setProperty("Visible", false);
        QAxObject *work_books = excel.querySubObject("WorkBooks");
        work_books->dynamicCall("Add");
    //    work_books->dynamicCall("Open(const QString&)", "E:\\test.xlsx");
        excel.setProperty("Caption", "Qt Excel");
        QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
        QAxObject *work_sheets = work_book->querySubObject("Sheets");  //Sheets也可换用WorkSheets
        QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 1);
    //    //删除工作表（删除第一个）
    //    QAxObject *first_sheet = work_sheets->querySubObject("Item(int)", 1);
    //    first_sheet->dynamicCall("delete");

    //    //插入工作表（插入至最后一行）
    //    int sheet_count = work_sheets->property("Count").toInt();  //获取工作表数目
    //    QAxObject *last_sheet = work_sheets->querySubObject("Item(int)", sheet_count);
    //    QAxObject *work_sheet = work_sheets->querySubObject("Add(QVariant)", last_sheet->asVariant());
    //    last_sheet->dynamicCall("Move(QVariant)", work_sheet->asVariant());
        emit progressChanged(20);
        work_sheet->setProperty("Name", "车轮信息");  //设置工作表名称
        QList<QString> titles;
        //========1A=======2B========3C=======4D=======5E=======6F=======7G=========8H========9I===
        titles <<"计轴"<<"车轮编号"<<"Data1"<<"Data2"<<"Data3"<<"Data4"<<"拼接数据"<<"检测时间"<<"检测人";

        //添加标题
        for(int i=1; i<= 8; i++)
        {
            QAxObject *cell = work_sheet->querySubObject("Cells(int, int)", 1, i);
            cell->setProperty("Value", titles.at(i-1));
            cell->setProperty("WrapText", true);
        }
        //填写内容
        int wheelstart = 2, wheelend;
        int axlestart = 2, axleend;

        emit progressChanged(30);


        QList<QList<double>> eddydatas;
        eddydatas << wheel->eddy->Data1 << wheel->eddy->Data2 << wheel->eddy->Data3
                  << wheel->eddy->Data4 << wheel->eddy->DataJointed;
        int column = eddydatas.length(); int row = wheel->eddy->DataJointed.length();
        wheelend = wheelstart + row;
        QList<QList<QVariant>> tables = Arithmetic::TableDoubleToVariant(eddydatas);
//写入数据
        QString datasRangeStr = QString("C%1:G%2").arg(wheelstart).arg(wheelend);
        QAxObject *range = work_sheet->querySubObject("Range(const QString&)", datasRangeStr);
        range->setProperty("Value", Arithmetic::TableToVariant(Arithmetic::reverseTable(tables)));
//合并RFID单元格
        QString mergeStr = QString("B%1:B%2").arg(wheelstart).arg(wheelend);
        QAxObject *merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
        merge_range->setProperty("MergeCells", true);
        QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", wheelstart, 3);
        cell->setProperty("Value", wheel->RFID);
        wheelstart = wheelend+1;
//合并 Axle的单元格
        axleend = wheelend;
        mergeStr = QString("A%1:A%2").arg(axlestart).arg(axleend);
        merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
        merge_range->setProperty("MergeCells", true);
        cell = work_sheet->querySubObject("Cells(int, int)", axlestart, 2);
        QString axlename = QString("计轴%1").arg(wheel->axle);
        cell->setProperty("Value", axlename);
        axlestart = axleend+1;

        emit progressChanged(95);
        excelName.replace('/', '\\');
        qDebug() << excelName;
        work_book->dynamicCall("SaveAs (const QString&,int,const QString&,const QString&,bool,bool)",
                                 excelName,56,QString(""),QString(""), false, false);
        work_book->dynamicCall("Close(Boolean)", false);
        excel.dynamicCall("Quit(void)");
        emit progressChanged(100);

    //    CoUninitialize();

}

void ExportThread::exportEddyDatas(QString excelName)
{
//    eddyName = pubPath +"/"+train->RFID+".xls";

    //    CoInitialize(NULL);
        emit progressChanged(10);
        QAxObject excel("Excel.Application");
        excel.setProperty("Visible", false);
        QAxObject *work_books = excel.querySubObject("WorkBooks");
        work_books->dynamicCall("Add");
    //    work_books->dynamicCall("Open(const QString&)", "E:\\test.xlsx");
        excel.setProperty("Caption", "Qt Excel");
        QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
        QAxObject *work_sheets = work_book->querySubObject("Sheets");  //Sheets也可换用WorkSheets
        QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 1);
    //    //删除工作表（删除第一个）
    //    QAxObject *first_sheet = work_sheets->querySubObject("Item(int)", 1);
    //    first_sheet->dynamicCall("delete");

    //    //插入工作表（插入至最后一行）
    //    int sheet_count = work_sheets->property("Count").toInt();  //获取工作表数目
    //    QAxObject *last_sheet = work_sheets->querySubObject("Item(int)", sheet_count);
    //    QAxObject *work_sheet = work_sheets->querySubObject("Add(QVariant)", last_sheet->asVariant());
    //    last_sheet->dynamicCall("Move(QVariant)", work_sheet->asVariant());
        emit progressChanged(20);
        work_sheet->setProperty("Name", "车轮信息");  //设置工作表名称
        QList<QString> titles;
        //========1A=======2B========3C=======4D=======5E=======6F=======7G=========8H========9I===
        titles <<"计轴"<<"车轮编号"<<"Data1"<<"Data2"<<"Data3"<<"Data4"<<"拼接数据"<<"检测时间"<<"检测人";

        //添加标题
        for(int i=1; i<= 8; i++)
        {
            QAxObject *cell = work_sheet->querySubObject("Cells(int, int)", 1, i);
            cell->setProperty("Value", titles.at(i-1));
            cell->setProperty("WrapText", true);
        }
        //填写内容
        int wheelstart = 2, wheelend;
        int axlestart = 2, axleend;

        emit progressChanged(30);
        for(int i=0; i< train->axleList.length(); i++)
        {
            Axle *tmpAxle = train->axleList.at(i);
            for(int j=0; j<tmpAxle->wheelsList.length(); j++)
            {
                emit progressChanged((i+1)*(j+1)/(train->axleList.length()*tmpAxle->wheelsList.length())*60);

                Wheels *tmpWheel = tmpAxle->wheelsList.at(j);
                QList<QList<double>> eddydatas;
                eddydatas << tmpWheel->eddy->Data1 << tmpWheel->eddy->Data2 << tmpWheel->eddy->Data3
                          << tmpWheel->eddy->Data4 << tmpWheel->eddy->DataJointed;
                int column = eddydatas.length(); int row = tmpWheel->eddy->DataJointed.length();
                wheelend = wheelstart + row;
                QList<QList<QVariant>> tables = Arithmetic::TableDoubleToVariant(eddydatas);
                //写入数据
                QString datasRangeStr = QString("C%1:G%2").arg(wheelstart).arg(wheelend);
                QAxObject *range = work_sheet->querySubObject("Range(const QString&)", datasRangeStr);
                range->setProperty("Value", Arithmetic::TableToVariant(Arithmetic::reverseTable(tables)));
                //合并RFID单元格
                QString mergeStr = QString("B%1:B%2").arg(wheelstart).arg(wheelend);
                QAxObject *merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
                merge_range->setProperty("MergeCells", true);
                QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", wheelstart, 3);
                cell->setProperty("Value", tmpAxle->wheelsList.at(j)->RFID);
                wheelstart = wheelend+1;
            }
            //合并 Axle的单元格
            axleend = wheelend;
            QString mergeStr = QString("A%1:A%2").arg(axlestart).arg(axleend);
            QAxObject *merge_range = work_sheet->querySubObject("Range(const QString&)", mergeStr);
            merge_range->setProperty("MergeCells", true);
            QAxObject *cell = work_sheet->querySubObject("Cells(int, int)", axlestart, 2);
            QString axlename = QString("计轴%1").arg(tmpAxle->No);
            cell->setProperty("Value", axlename);
            axlestart = axleend+1;
        }

        emit progressChanged(95);
        excelName.replace('/', '\\');
        qDebug() << excelName;
        work_book->dynamicCall("SaveAs (const QString&,int,const QString&,const QString&,bool,bool)",
                                 excelName,56,QString(""),QString(""), false, false);
        work_book->dynamicCall("Close(Boolean)", false);
        excel.dynamicCall("Quit(void)");
        emit progressChanged(100);

    //    CoUninitialize();
}

void ExportThread::exportLaserDatas(QString laser7080Name, QString laser7200Name, Wheels *wheel)
{
//    laser7080Name = QString("%1/%2%3.txt").arg(path).arg(wheel->RFID).arg(7080);
//    laser7200Name = QString("%1/%2%3.txt").arg(path).arg(wheel->RFID).arg(7200);
    QFile file(laser7080Name), file1(laser7200Name);
    file.open(QFile::WriteOnly);
    file1.open(QFile::WriteOnly);

   //7080数据
    for(int i=0; i<wheel->lasers->laser7080.length(); i++)
    {
        QList<Point> tmpLaserLine = wheel->lasers->laser7080.at(i);
        for(int j=0; j<tmpLaserLine.length(); j++)
        {
            Point point = tmpLaserLine.at(j);
            QString text = QString("%1\t%2\t").arg(point.x()).arg(point.y());
            file.write(text.toLatin1());
        }
        QString text = "\b\n";
        file.write(text.toLatin1());
    }
    //7200数据
    for(int i=0; i<wheel->lasers->laser7200.length(); i++)
    {
        QList<Point> tmpLaserLine = wheel->lasers->laser7200.at(i);
        for(int j=0; j<tmpLaserLine.length(); j++)
        {
            Point point = tmpLaserLine.at(j);
            QString text = QString("%1\t%2\t").arg(point.x()).arg(point.y());
            file1.write(text.toLatin1());
        }
        QString text = "\b\n";
        file.write(text.toLatin1());
    }

    file.close();
    file1.close();
}

void ExportThread::exportDBBase()
{
    qDebug() << "db is connecting";
    db->connectDB();
    qDebug() << "db has connected";
    for(int i=0; i<train->axleList.length(); i++)
    {
        Axle *axle = train->axleList.at(i);
        for(int j=0; j<axle->wheelsList.length(); j++)
        {
            Wheels *tmpWheel = axle->wheelsList.at(j);
            eddyName = QString("%1/%2.xls").arg(pubPath).arg(tmpWheel->RFID);
            laser7080Name = QString("%1/%2%3.txt").arg(pubPath).arg(tmpWheel->RFID).arg(7080);
            laser7200Name = QString("%1/%2%3.txt").arg(pubPath).arg(tmpWheel->RFID).arg(7200);
            QString laserName = laser7080Name+";"+laser7200Name;
            QString sql = QString("insert into WheelModel(车号,计轴,车轮位置,车轮编号,径跳,Sh,"
                                  "Sd1,Sd2,Gr1,Gr2,内测距,电涡流数据,激光数据,检测时间,检测人)"
                                  "values('%1',%2,%3,'%4',%5,%6,%7,%8,%9,%10,%11,'%12','%13',"
                                  "'%14','%15')").arg(train->RFID).arg(tmpWheel->axle)
                    .arg(tmpWheel->position).arg(tmpWheel->RFID).arg(tmpWheel->radiuRunOut)
                    .arg(tmpWheel->lasers->Sh).arg(tmpWheel->lasers->Sd1).arg(tmpWheel->lasers->Sd2)
                    .arg(tmpWheel->lasers->Gr1).arg(tmpWheel->lasers->Gr2).arg(axle->Length).arg(eddyName)
                    .arg(laserName).arg(QTime::currentTime().toString()).arg("张三");
            qDebug() <<sql;
//            db->runSql(sql);
            qDebug() << "has inserted into DB";

            exportEddyDatas(eddyName, tmpWheel);
            qDebug() << "has inserted into eddy.xls";
            exportLaserDatas(laser7080Name, laser7200Name, tmpWheel);
             qDebug() << "has inserted into laser.txt";
        }
    }
    db->close();
}

void ExportThread::mergeCells(QString mergestring, QAxObject *work_sheet)
{
    QAxObject *merge_range = work_sheet->querySubObject("Range(const QString&)", mergestring);
            merge_range->setProperty("MergeCells", true);

}


