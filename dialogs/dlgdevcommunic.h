#ifndef DLGDEVCOMMUNIC_H
#define DLGDEVCOMMUNIC_H

#include <QDialog>

namespace Ui {
class DlgDevCommunic;
}

class DlgDevCommunic : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDevCommunic(bool isConnect = false, QWidget *parent = 0);
    ~DlgDevCommunic();

signals:
    void isConnectedChanged(bool);
private slots:
    void on_btnConnect_clicked();
    void onConnect();

private:
    Ui::DlgDevCommunic *ui;

    bool isConnected;
    void initWidget();
    void loadQSS(QString qssName);
    bool isPwdRight;
};

#endif // DLGDEVCOMMUNIC_H
