#ifndef DLGMANAGESEARCHMIDDATAS_H
#define DLGMANAGESEARCHMIDDATAS_H

#include <QDialog>

namespace Ui {
class DlgManageSearchMidDatas;
}

class DlgManageSearchMidDatas : public QDialog
{
    Q_OBJECT

public:
    explicit DlgManageSearchMidDatas(QWidget *parent = 0);
    ~DlgManageSearchMidDatas();
    void addOpenBtn();

private:
    Ui::DlgManageSearchMidDatas *ui;

private slots:
    void onOpen();
};

#endif // DLGMANAGESEARCHMIDDATAS_H
