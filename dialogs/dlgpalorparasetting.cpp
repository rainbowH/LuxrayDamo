#include "dlgpalorparasetting.h"
#include "ui_dlgpalorparasetting.h"
#include <QDebug>

DlgPalorParaSetting::DlgPalorParaSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPalorParaSetting)
{
    ui->setupUi(this);
}

DlgPalorParaSetting::~DlgPalorParaSetting()
{
    delete ui;
}

void DlgPalorParaSetting::onSetTickCounts(int counts)
{
    ui->txtTickCounts->setText(QString::number(counts));
}

void DlgPalorParaSetting::onSetRange(double min, double max)
{
    ui->txtMinValue->setText(QString::number(min));
    ui->txtMaxValue->setText(QString::number(max));
}

void DlgPalorParaSetting::onSetAngleRange(double min, double max)
{
    ui->txtAngleMin->setText(QString::number(min));
    ui->txtAngleMax->setText(QString::number(max));
}

void DlgPalorParaSetting::on_btnOK_clicked()
{
    int count = ui->txtTickCounts->text().toInt();
    double min = ui->txtMinValue->text().toDouble();
    double max = ui->txtMaxValue->text().toDouble();

    if(count >0)
        emit signalTickCounts(count);

    if(min > max)
    {
        double tmp = min;
        min = max;
        max = tmp;
    }
    emit signalRange(min, max);

    min =ui->txtAngleMin->text().toDouble();
    max = ui->txtAngleMax->text().toDouble();
    if(min > max)
    {
        double tmp = min;
        min = max;
        max = tmp;
    }
    emit signalAngleRange(min, max);

    QDialog::close();
}
