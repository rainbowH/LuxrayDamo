#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>
#include "dbsqlite.h"
#include "dialogs/dlgtrainsearchshow.h"

namespace Ui {
class TableWidget;
}

class TableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TableWidget(QWidget *parent = 0,int rowmax = 10);
    ~TableWidget();

//    void searchTrain(QString sql);
//    void searchWheel(QString sql);
    int rowMax;
public slots:
    void onGetDatas(QString sql, int searchType);
    void onClickedDtl();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_btnLast_clicked();

    void on_btnNext_clicked();

private:
    Ui::TableWidget *ui;
    DBSqlite db;
    int index;
    int count;
    DlgTrainSearchShow *dlg;
    bool isPressed;
    int searchType;

    void deleteAllItem();
};

#endif // TABLEWIDGET_H
