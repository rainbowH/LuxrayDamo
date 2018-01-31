#ifndef WHEELSDATATHREAD_H
#define WHEELSDATATHREAD_H

#include <QObject>
#include <QWidget>
#include <QThread>

class WheelsDataThread : public QThread
{
    Q_OBJECT
public:
    WheelsDataThread(QString ExcelName);
    void importFromExcel();

    QString excelName;


protected:
    void run();


};

#endif // WHEELSDATATHREAD_H
