#ifndef DLGUSERPARASTAT_H
#define DLGUSERPARASTAT_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class DlgUserParaStat;
}

class DlgUserParaStat : public QDialog
{
    Q_OBJECT

public:
    explicit DlgUserParaStat(QWidget *parent = 0);
    ~DlgUserParaStat();

    void initDialog();
    QTreeWidgetItem *searchItem(QString Train);
    void getInfoDB(QString Train);

private slots:
    void on_btnCancel_clicked();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_btnSearch_clicked();

private:
    Ui::DlgUserParaStat *ui;
    void clearAllTxt();
};

#endif // DLGUSERPARASTAT_H
