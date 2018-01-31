#ifndef DLGPALORPARASETTING_H
#define DLGPALORPARASETTING_H

#include <QDialog>

namespace Ui {
class DlgPalorParaSetting;
}

class DlgPalorParaSetting : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPalorParaSetting(QWidget *parent = 0);
    ~DlgPalorParaSetting();

signals:
    void signalTickCounts(int);
    void signalRange(double, double);
    void signalAngleRange(double, double);

public slots:
    void onSetTickCounts(int counts);
    void onSetRange(double min, double max);
    void onSetAngleRange(double min, double max);

private slots:
    void on_btnOK_clicked();

private:
    Ui::DlgPalorParaSetting *ui;
};

#endif // DLGPALORPARASETTING_H
