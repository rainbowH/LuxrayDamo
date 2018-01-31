#ifndef DLGWHEELSHOW_H
#define DLGWHEELSHOW_H

#include <QDialog>
#include <QObject>

namespace Ui {
class DlgWheelShow;
}

class DlgWheelShow :public QDialog
{
    Q_OBJECT

public:
    explicit DlgWheelShow(QWidget *parent = 0);
    ~DlgWheelShow();

private:
    Ui::DlgWheelShow *ui;
};

#endif // DLGWHEELSHOW_H
