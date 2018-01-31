#include "wheelicon.h"
#include "ui_wheelicon.h"
#include "dialogs/dlgwheelshow.h"
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QDebug>

WheelIcon::WheelIcon(QWidget *parent) :
    QFrame(parent),
    isPressed(false),
    ui(new Ui::WheelIcon)
{
    ui->setupUi(this);
    this->setLayout(ui->mainLayout);

}

WheelIcon::~WheelIcon()
{
    delete ui;
}

void WheelIcon::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/images/wheel3D.png");
    painter.begin(ui->iconWidget);
    painter.drawPixmap(5, 5, ui->iconWidget->width(), ui->iconWidget->height(), pixmap);
    painter.end();
}

void WheelIcon::mousePressEvent(QMouseEvent *event)
{
    isPressed = true;
}

void WheelIcon::mouseReleaseEvent(QMouseEvent *event)
{
    if(isPressed)
    {
        isPressed = false;
//        this->setStyleSheet("QFrame{border: 1px solid red;}");
        emit clicked();
    }
}

void WheelIcon::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug() << "double clicked";
    DlgWheelShow *dlg = new DlgWheelShow;
    dlg->showMaximized();
}
