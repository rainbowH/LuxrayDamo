#include "dlglathe.h"
#include "ui_dlglathe.h"
#include <QFile>
DlgLathe::DlgLathe(QList<QString> list) :
    comboBoxlist(list),
    ui(new Ui::DlgLathe)
{
    ui->setupUi(this);
    initComboBox();
    loadQSS();

}

DlgLathe::~DlgLathe()
{
    delete ui;
}

void DlgLathe::initComboBox()
{
    for(int i=0;i<comboBoxlist.length();i++){
        ui->comboBox_select->addItem(comboBoxlist.at(i));
    }
}

void DlgLathe::loadQSS()
{
    QFile file(":/QSS/dlglathe.qss");
    file.open(QFile::ReadOnly);
    this->setStyleSheet(file.readAll());
    file.close();
}

void DlgLathe::closeEvent(QCloseEvent *event)
{
    emit isDestroyed(this);
}

void DlgLathe::on_pushButton_Cancle_clicked()
{
    emit destroyed();
    this->destroy();
}

void DlgLathe::on_pushButton_OK_clicked()
{
    emit comboBoxSelected(ui->comboBox_select->currentText());
    emit destroyed();
    this->destroy();
}
