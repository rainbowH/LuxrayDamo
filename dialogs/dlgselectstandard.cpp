#include "dlgselectstandard.h"
#include "ui_dlgselectstandard.h"
#include <QDir>
#include <QFileInfo>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QIcon>
#include <QSettings>
#include <QDebug>

DlgSelectStandard::DlgSelectStandard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSelectStandard)
{
    ui->setupUi(this);
    this->setWindowTitle("标准轮文件");

    QSettings settings(qApp->applicationDirPath()+"/hubtesting.ini", QSettings::IniFormat);
    currentStandardPath = settings.value("Group1/StandardWheelPath").toString();

    initListView();

}

DlgSelectStandard::~DlgSelectStandard()
{
    delete ui;
}

void DlgSelectStandard::initListView()
{
    QStandardItemModel *model = new QStandardItemModel;
    QDir dir(qApp->applicationDirPath() +"/标准轮数据");
    path = dir.path();
    foreach (QFileInfo fileinfo, dir.entryInfoList()) {
        if(fileinfo.isFile() && fileinfo.baseName().contains("标准轮"))
        {
            QStandardItem *item;
            if(currentStandardPath == fileinfo.filePath())
                item = new QStandardItem(QIcon(":/csv.png"), "√\t"+fileinfo.baseName());
            else
                item = new QStandardItem(QIcon(":/csv.png"), " \t"+fileinfo.baseName());

            model->appendRow(item);
        }
    }
    ui->listView->setModel(model);
}

void DlgSelectStandard::on_buttonBox_accepted()
{
    QStandardItemModel *model = (QStandardItemModel*)ui->listView->model();
    QString name = path + "/" + model->item(ui->listView->currentIndex().row(), ui->listView->currentIndex().column())->text().trimmed()+".csv";
    this->close();
    if(!name.contains("√"))
        emit standardSelected(name);
}

void DlgSelectStandard::on_buttonBox_rejected()
{
    this->close();
}
