#ifndef DLGLOGIN_H
#define DLGLOGIN_H

#include <QDialog>

namespace Ui {
class DlgLogin;
}

class DlgLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DlgLogin(QWidget *parent = 0);
    ~DlgLogin();

private slots:
    void on_Btn_Login_clicked();

    void on_Btn_Cancel_clicked();


private:
    Ui::DlgLogin *ui;
};

#endif // DLGLOGIN_H
