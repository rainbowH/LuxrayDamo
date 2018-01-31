#include "progressbarui.h"
#include "ui_progressbarui.h"
#include <QPalette>
#include <QTimer>

ProgressBarUi::ProgressBarUi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressBarUi)
{
    ui->setupUi(this);
//    setAttribute(Qt::WA_TranslucentBackground, true);
    setAutoFillBackground(false);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);

    value = 0;
    ui->progressBar->setValue(value);
}

ProgressBarUi::~ProgressBarUi()
{
    delete ui;
}

void ProgressBarUi::setProgressValue(int value)
{
    ui->progressBar->setValue(value);
}

void ProgressBarUi::on_progressBar_valueChanged(int value)
{
    if(value == ui->progressBar->maximum())
    {
        emit finished();
        close();
    }
}
