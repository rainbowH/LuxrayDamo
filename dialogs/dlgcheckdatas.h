#ifndef DLGCHECKDATAS_H
#define DLGCHECKDATAS_H

#include <QDialog>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QCalendarWidget>
#include "dbmysql.h"


namespace Ui {
class DlgCheckDatas;
}

class DlgCheckDatas : public QDialog
{
    Q_OBJECT

public:
    explicit DlgCheckDatas(QWidget *parent = 0);
    ~DlgCheckDatas();

private slots:
    void on_Combox_Condition_currentIndexChanged(const QString &arg1);

    void on_Btn_Search_clicked();

private:
    Ui::DlgCheckDatas *ui;
    QComboBox *timeBox;
    QDateTimeEdit *datetime;
    DBMySql *database;
    QSqlTableModel *model;

};

#endif // DLGCHECKDATAS_H
