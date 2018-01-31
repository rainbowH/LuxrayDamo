#ifndef IMPORTTHREAD_H
#define IMPORTTHREAD_H

#include <QObject>
#include <QThread>
#include <QStandardItemModel>
#include <QMutex>

class ImportThread : public QThread
{
    Q_OBJECT
public:
    ImportThread(QString ExcelName);
    ~ImportThread();
    void importFromExcel();
    QList<QList<QVariant>> datasTable;
    QList<double> doubleList;
    QString ExcelName;
    QStandardItemModel *model;

signals:
    void progress(int);
    void progressRange(int);
    void labelText(QString);
    void initTable(int, int);
    void setTabelItem(int, int, QString);
    void endLoading(int, int, QString, QStringList);
    void updateTable(QStandardItemModel *);

    void sendDatas(QList<QList<QVariant>> datas);

protected:
    void run();

private:
    QMutex mutex;
};

#endif // IMPORTTHREAD_H
