#ifndef DLGHISTORYDATAS_H
#define DLGHISTORYDATAS_H

#include <QDialog>
#include <QTableWidgetItem>
#include "dlgtrainsearchshow.h"


namespace Ui {
class DlgHistoryDatas;
}

class DlgHistoryDatas : public QDialog
{
    Q_OBJECT

public:
    explicit DlgHistoryDatas(QWidget *parent = 0);
    ~DlgHistoryDatas();
    DlgTrainSearchShow *dlg;

    void searchTrain(QString TrainNum);
    void searchWheel(QString wheelNum);

signals:
    void sqlSentence(QString, int);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
private slots:
//    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_btnSearch_clicked();
    void onClickedDtl();


//    void on_tableWidget_itemClicked(QTableWidgetItem *item);

private:
    Ui::DlgHistoryDatas *ui;
    const QString btnQSS;
};

#endif // DLGHISTORYDATAS_H
