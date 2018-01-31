#ifndef EXPORTTHREAD_H
#define EXPORTTHREAD_H

#include <QObject>
#include <QThread>
#include <QAxObject>
#include "dbsqlite.h"

#include "objects/wheels.h"

class ExportThread : public QThread
{
    Q_OBJECT
public:
    ExportThread(int dataFlag, Train *train1, QString path = "");

    void exportBaseDatas();
    void exportEddyDatas(QString excelName, Wheels *wheel);
    void exportEddyDatas(QString excelName);
    void exportLaserDatas(QString laser7080Name, QString laser7200Name, Wheels *wheel);
    void exportDBBase();
    void mergeCells(QString mergestring, QAxObject *work_sheet);
signals:
    void progressChanged(int);

private:
    int dataFlag;
    QString pubPath;
    Train *train;
    DBSqlite *db;

    QString eddyName;
    QString laser7080Name, laser7200Name;
    QString excelName;
protected:
    void run();
};

#endif // EXPORTTHREAD_H
