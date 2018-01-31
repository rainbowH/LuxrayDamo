#include "dlgwheelshow.h"
#include "ui_dlgwheelshow.h"
#include <QTime>

DlgWheelShow::DlgWheelShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgWheelShow)
{
    ui->setupUi(this);
    this->setLayout(ui->mainLayout);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);
    ui->widget_5->setVisible(false);
    resize(QApplication::desktop()->availableGeometry().size());
    connect(ui->wheel, SIGNAL(PickSectionMsg(QVector<QVector2D>)), ui->plot, SLOT(drawPlot(QVector<QVector2D>)));
    connect(ui->spread, SIGNAL(PickSectionRadian(float)), ui->wheel, SLOT(SetPickRadian(float)));

}

DlgWheelShow::~DlgWheelShow()
{
    delete ui;
}
