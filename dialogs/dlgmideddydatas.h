#ifndef DLGMIDEDDYDATAS_H
#define DLGMIDEDDYDATAS_H

#include <QDialog>

namespace Ui {
class DlgMidEddyDatas;
}

class DlgMidEddyDatas : public QDialog
{
    Q_OBJECT

public:
    explicit DlgMidEddyDatas(QWidget *parent = 0);
    ~DlgMidEddyDatas();

private:
    Ui::DlgMidEddyDatas *ui;
};

#endif // DLGMIDEDDYDATAS_H
