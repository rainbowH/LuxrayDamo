#include "dlgmoniterealtime.h"
#include "ui_dlgmoniterealtime.h"
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QFile>

DlgMoniteRealTime::DlgMoniteRealTime(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMoniteRealTime)
{
    ui->setupUi(this);
    this->setLayout(ui->mainLayout);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

    isStartMonite = false;

    QFile file(":/QSS/dlgmoniterealtime.qss");
    file.open(QFile::ReadOnly);
    this->setStyleSheet(file.readAll());
    file.close();
//    rect.setX(ui->line->geometry().x());
//    rect.setY(ui->line->geometry().y());
//    rect.setWidth(10);
//    rect.setHeight(ui->line->geometry().height());
//    timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
//    flag = 0;
}

DlgMoniteRealTime::~DlgMoniteRealTime()
{
    delete ui;
}

void DlgMoniteRealTime::paintEvent(QPaintEvent *event)
{
//    if(flag ==0)
//    {
//        rect = ui->line->geometry();
//        rect.setY(10);
//        rect.setHeight(3);
//        flag = 1;
//    }
//    QPainter painter(this);
//painter.setBrush(QBrush(Qt::white));
////    painter.drawPixmap(rect.x(), rect.y(), QPixmap(":/images/timeline.png"));
//painter.drawRect(rect);
}
//开始/停止监控
void DlgMoniteRealTime::on_btnMonite_clicked()
{
    if(!isStartMonite)
    {
        ui->btnMonite->setText("停止监控");
//        timer->start(10);
    }
    else
    {
        switch(QMessageBox::question(this, "question", "监控尚未结束，是否确认结束",
                                     QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok))
        {
        case QMessageBox::Ok:
            ui->btnMonite->setText("开始监控");
            break;
        case QMessageBox::Cancel:
            return;
        }
    }
    isStartMonite = !isStartMonite;
}

void DlgMoniteRealTime::onTimeOut()
{
//    rect.setX(rect.x() + 5);
//    update();
}
