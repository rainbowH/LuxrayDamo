#ifndef DLGPASSWARDINPUT_H
#define DLGPASSWARDINPUT_H

#include <QDialog>
#include "dbsqlite.h"

namespace Ui {
class DlgPasswardInput;
}

class DlgPasswardInput : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPasswardInput(int passwardType, QWidget *parent = 0);
    ~DlgPasswardInput();

    DBSqlite *db;
    int passwardType;

signals:
    void openDevCom();
    void openManage();

private slots:
    void on_btnOK_clicked();

private:
    Ui::DlgPasswardInput *ui;
};

#endif // DLGPASSWARDINPUT_H
