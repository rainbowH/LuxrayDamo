#ifndef DLGWELCOME_H
#define DLGWELCOME_H

#include <QDialog>
#include <QPoint>
#include "mainwindow.h"

namespace Ui {
class DlgWelcome;
}

class DlgWelcome : public QDialog
{
    Q_OBJECT

public:
    explicit DlgWelcome(QWidget *parent = 0);
    ~DlgWelcome();

signals:
    void openDevCommunicate();
    void openMoniteRealTime();
    void openHistoryDatas();
    void openParaSetting();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void on_btnExit_clicked();

    void on_btnDevCom_clicked();

    void on_btnMoniteRealTime_clicked();

    void on_btnHistDatas_clicked();

    void on_btnParaSetting_clicked();

private:
    Ui::DlgWelcome *ui;
    MainWindow *mw;
    bool isPressed;
    QPoint startPoint;
    QPoint endPoint;
};

#endif // DLGWELCOME_H
