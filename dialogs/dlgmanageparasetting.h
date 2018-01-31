#ifndef DLGMANAGEPARASETTING_H
#define DLGMANAGEPARASETTING_H

#include <QDialog>

namespace Ui {
class DlgManageParaSetting;
}

class DlgManageParaSetting : public QDialog
{
    Q_OBJECT

public:
    explicit DlgManageParaSetting(QWidget *parent = 0);
    ~DlgManageParaSetting();

private:
    Ui::DlgManageParaSetting *ui;
};

#endif // DLGMANAGEPARASETTING_H
