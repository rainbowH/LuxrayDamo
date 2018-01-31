#ifndef DLGDAMAGEANALYZE_H
#define DLGDAMAGEANALYZE_H

#include <QDialog>

namespace Ui {
class DlgDamageAnalyze;
}

class DlgDamageAnalyze : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDamageAnalyze(int type = -1, QWidget *parent = 0);
    void judgeDamageType();
    ~DlgDamageAnalyze();

private:
    Ui::DlgDamageAnalyze *ui;
    int type;
};

#endif // DLGDAMAGEANALYZE_H
