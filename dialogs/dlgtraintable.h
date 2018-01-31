#ifndef DLGTRAINTABLE_H
#define DLGTRAINTABLE_H

#include <QDialog>
#include <QStandardItemModel>
#include <QDateTime>


namespace Ui {
class DlgTrainTable;
}

class DlgTrainTable : public QDialog
{
    Q_OBJECT

public:
    explicit DlgTrainTable(QString train = "", QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"),
                           QWidget *parent = 0);
    ~DlgTrainTable();

    QString Train, datetTime;

private slots:
    void on_btnDatasExport_clicked();

private:
    Ui::DlgTrainTable *ui;

    void initTable();
    void setTableValues();
    void exportToExcel(QString filePath, QStandardItemModel *model);
};

#endif // DLGTRAINTABLE_H
