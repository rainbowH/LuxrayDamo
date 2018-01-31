#ifndef WHEELSINFOTHREAD_H
#define WHEELSINFOTHREAD_H

#include <QObject>
#include <QWidget>
#include <QThread>
#include <QVariant>

class WheelsInfoThread : public QThread
{
    Q_OBJECT
public:
    WheelsInfoThread(QString name);
    void importFromExcel();
    QList<QList<QVariant>> datasTable;

signals:
    void sendDatas(QList<QList<QVariant>> datas);

protected:
    void run();

private:
    QString fileName;
};

#endif // WHEELSINFOTHREAD_H
