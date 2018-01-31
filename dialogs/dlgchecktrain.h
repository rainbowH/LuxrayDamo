#ifndef DLGCHECKTRAIN_H
#define DLGCHECKTRAIN_H

#include <QDialog>

namespace Ui {
class DlgCheckTrain;
}

class DlgCheckTrain : public QDialog
{
    Q_OBJECT

public:
    explicit DlgCheckTrain(QWidget *parent = 0);
    ~DlgCheckTrain();

private:
    Ui::DlgCheckTrain *ui;
};

#endif // DLGCHECKTRAIN_H
