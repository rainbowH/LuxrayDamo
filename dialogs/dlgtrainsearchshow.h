#ifndef DLGTRAINSEARCHSHOW_H
#define DLGTRAINSEARCHSHOW_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
class DlgTrainSearchShow;
}

class DlgTrainSearchShow : public QDialog
{
    Q_OBJECT

public:
    explicit DlgTrainSearchShow(QString train = "",
                                QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"),
                                QWidget *parent = 0);
    ~DlgTrainSearchShow();

    QString Train, DateTime;

private slots:
    void on_toolButton_clicked();
    void onDatas();
    void onNoteWheel();

private:
    Ui::DlgTrainSearchShow *ui;
};

#endif // DLGTRAINSEARCHSHOW_H
