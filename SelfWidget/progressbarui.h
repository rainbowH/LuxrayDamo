#ifndef PROGRESSBARUI_H
#define PROGRESSBARUI_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class ProgressBarUi;
}

class ProgressBarUi : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressBarUi(QWidget *parent = 0);
    ~ProgressBarUi();

    int value;
signals:
    void finished();

public slots:
    void setProgressValue(int value);

private slots:
    void on_progressBar_valueChanged(int value);

private:
    Ui::ProgressBarUi *ui;
};

#endif // PROGRESSBARUI_H
