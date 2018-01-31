#include "dlgpathsetting.h"
#include <QFileDialog>
#include "ui_dlgpathsetting.h"

DlgPathSetting::DlgPathSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPathSetting)
{
    ui->setupUi(this);
}

DlgPathSetting::~DlgPathSetting()
{
    delete ui;
}

void DlgPathSetting::setPath(QString path)
{
    this->path = path;
    ui->linePath->setText(path);
}

QString DlgPathSetting::getPath()
{
    return this->path;
}

void DlgPathSetting::on_btnSelect_clicked()
{
    path = QFileDialog::getExistingDirectory(this, "选择默认文件夹", "./");
    ui->linePath->setText(path);
}
