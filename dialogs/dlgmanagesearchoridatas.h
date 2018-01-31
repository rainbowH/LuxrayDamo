#ifndef DLGMANAGESEARCHORIDATAS_H
#define DLGMANAGESEARCHORIDATAS_H

#include <QDialog>

namespace Ui {
class DlgManageSearchOriDatas;
}

class DlgManageSearchOriDatas : public QDialog
{
    Q_OBJECT

public:
    explicit DlgManageSearchOriDatas(QWidget *parent = 0);
    ~DlgManageSearchOriDatas();

private:
    Ui::DlgManageSearchOriDatas *ui;
};

#endif // DLGMANAGESEARCHORIDATAS_H
