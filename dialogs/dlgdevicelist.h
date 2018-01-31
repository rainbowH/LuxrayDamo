#ifndef DLGDEVICELIST_H
#define DLGDEVICELIST_H

#include <QDialog>
#include <QStandardItemModel>
#include <QMenu>

namespace Ui {
class DlgDeviceList;
}

class DlgDeviceList : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDeviceList(QWidget *parent = 0);
    ~DlgDeviceList();

    QStandardItemModel *model;
    QMenu *customMenu;
    void initMenu();

protected:

private slots:
    void on_tableWidget_customContextMenuRequested(const QPoint &pos);
    void onInsertRow();
    void onDeleteRow();

private:
    Ui::DlgDeviceList *ui;

    int eddyEnd, elemchEnd;

    void initTableWidget();
    void resetNo();

};

#endif // DLGDEVICELIST_H
