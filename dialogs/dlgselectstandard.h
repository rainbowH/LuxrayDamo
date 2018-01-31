#ifndef DLGSELECTSTANDARD_H
#define DLGSELECTSTANDARD_H

#include <QDialog>

namespace Ui {
class DlgSelectStandard;
}

class DlgSelectStandard : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSelectStandard(QWidget *parent = 0);
    ~DlgSelectStandard();

    void initListView();
    QString path;

signals:
    void standardSelected(QString);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DlgSelectStandard *ui;
    QString currentStandardPath;
};

#endif // DLGSELECTSTANDARD_H
