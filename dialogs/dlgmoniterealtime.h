#ifndef DLGMONITEREALTIME_H
#define DLGMONITEREALTIME_H

#include <QDialog>
#include <QRect>
#include <QTimer>

namespace Ui {
class DlgMoniteRealTime;
}

class DlgMoniteRealTime : public QDialog
{
    Q_OBJECT

public:
    explicit DlgMoniteRealTime(QWidget *parent = 0);
    ~DlgMoniteRealTime();

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void on_btnMonite_clicked();
    void onTimeOut();

private:
    Ui::DlgMoniteRealTime *ui;
    bool isStartMonite;
    QRect rect;
    QTimer *timer;
    int flag;
};

#endif // DLGMONITEREALTIME_H
