#ifndef DLGUSERPARASETTING_H
#define DLGUSERPARASETTING_H

#include <QDialog>
#include <QMetaType>
#include <QSqlQuery>
#include <QListWidgetItem>
#include "objects/wheels.h"

namespace Ui {
class DlgUserParaSetting;
}

class DlgUserParaSetting : public QDialog
{
    Q_OBJECT

public:
    explicit DlgUserParaSetting(QWidget *parent = 0);
    ~DlgUserParaSetting();

    QStringList trainList;

signals:
    void userParasAdded(UserParas *paras);
    void userParasDeleted(QString paraGroupName);
    void userTrainAdded(QString train, QString datetime, QString paragroup);
    void userTrainDeleted(QString train, QString datetime);
    
protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:

    void on_btnOK_clicked();

    void on_btnDtl_clicked();

    void on_txtParaGroup_currentTextChanged(const QString &arg1);

    void on_btnOK_2_clicked();

    void on_btnDtl_2_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::DlgUserParaSetting *ui;
    void initWidget();
    void initParaGroup();
    void initTrain();
    void initParasWidget();

    void initParasWidgetFromDB(QSqlQuery *query);
    void addTwoDecimal(QLineEdit *lineEdit);
    bool isNewParaGroup;
    QString tmpString;
};

#endif // DLGUSERPARASETTING_H
