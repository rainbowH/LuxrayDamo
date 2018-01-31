#ifndef DLGWHEELDTL_H
#define DLGWHEELDTL_H

#include <QDialog>
#include <dbsqlite.h>

namespace Ui {
class DlgWheelDtl;
}

class DlgWheelDtl : public QDialog
{
    Q_OBJECT

public:
    explicit DlgWheelDtl(DBSqlite *db, QString RFID, QWidget *parent = 0);
    ~DlgWheelDtl();
    void setAllLineEnable(bool enable);

private slots:
    void on_btnEdit_clicked();

private:
    Ui::DlgWheelDtl *ui;
    void initDatas(QSqlQuery *query);
};

#endif // DLGWHEELDTL_H
