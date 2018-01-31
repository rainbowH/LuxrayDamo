#ifndef DLGLASERCPS_H
#define DLGLASERCPS_H

#include <QDialog>

namespace Ui {
class DlgLaserCPS;
}

class DlgLaserCPS : public QDialog
{
    Q_OBJECT

public:
    explicit DlgLaserCPS(QWidget *parent = 0);
    ~DlgLaserCPS();

private:
    Ui::DlgLaserCPS *ui;
};

#endif // DLGLASERCPS_H
