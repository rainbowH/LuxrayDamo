#ifndef DLGLATHE_H
#define DLGLATHE_H

#include <QDialog>
#include <QList>
#include <QString>

namespace Ui {
class DlgLathe;
}

class DlgLathe : public QDialog
{
    Q_OBJECT

public:
    //explicit DlgLathe(QWidget *parent = 0);
    explicit DlgLathe(QList<QString> list);

    ~DlgLathe();
    void initComboBox();
    void loadQSS();

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void comboBoxSelected(QString list);
    void isDestroyed(QObject *);
private slots:
    void on_pushButton_Cancle_clicked();
    void on_pushButton_OK_clicked();

private:
    Ui::DlgLathe *ui;
    QList<QString> comboBoxlist;
};

#endif // DLGLATHE_H
