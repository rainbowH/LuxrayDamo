#include "dlgtrainsearchshow.h"
#include "ui_dlgtrainsearchshow.h"
#include "dialogs/dlgwheelshow.h"
#include "dialogs/dlgtraintable.h"

DlgTrainSearchShow::DlgTrainSearchShow(QString train, QString dateTime, QWidget *parent) :
    QDialog(parent),
    Train(train),
    DateTime(dateTime),
    ui(new Ui::DlgTrainSearchShow)
{
    ui->setupUi(this);
    this->setLayout(ui->mainLayout);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);

    QAction *actionDatas = new QAction(QIcon(":/images/3Doutput.png"), "查看数据");
    ui->widget->appendAction(actionDatas);
    connect(actionDatas, SIGNAL(triggered(bool)), this, SLOT(onDatas()));
    connect(ui->wheel1, SIGNAL(clicked()),this, SLOT(onNoteWheel()));
    connect(ui->wheel2, SIGNAL(clicked()),this, SLOT(onNoteWheel()));
    connect(ui->wheel3, SIGNAL(clicked()),this, SLOT(onNoteWheel()));
    connect(ui->wheel4, SIGNAL(clicked()),this, SLOT(onNoteWheel()));


//    ui->toolButton->setAutoFillBackground(true);
//    QPalette palette = ui->toolButton->palette();
//    palette.setBrush(QPalette::Background,
//                     QBrush(QPixmap(":/images/wheel3D.png").scaled(ui->toolButton->size(),
//                                                                   Qt::IgnoreAspectRatio,
//                                                                   Qt::SmoothTransformation)));
//    ui->toolButton->setStyleSheet("border-image: url(:/images/wheel3D.png)");
//    ui->toolButton_2->setStyleSheet("border-image: url(:/images/wheel3D.png)");
//    ui->toolButton_3->setStyleSheet("border-image: url(:/images/wheel3D.png)");
//    ui->toolButton_4->setStyleSheet("border-image: url(:/images/wheel3D.png)");

//    ui->frame->setLayout(ui->wheelLayout1);
//    ui->frame->setFrameShape(QFrame::Box);
//    ui->frame->setLineWidth(5);
//    ui->frame_2->setLayout(ui->wheelLayout2);
//    ui->frame_3->setLayout(ui->wheelLayout3);
//    ui->frame_4->setLayout(ui->wheelLayout4);
//    ui->toolButton->setPalette(palette);

}

DlgTrainSearchShow::~DlgTrainSearchShow()
{
    delete ui;
}

void DlgTrainSearchShow::on_toolButton_clicked()
{
//    qDebug() << "clicked the wheel";
    DlgWheelShow *dlg = new DlgWheelShow();
    dlg->showMaximized();
}

void DlgTrainSearchShow::onDatas()
{
    DlgTrainTable *dlg = new DlgTrainTable(Train, DateTime);
    dlg->showMaximized();
}

void DlgTrainSearchShow::onNoteWheel()
{
    QFrame *frame = (QFrame *)sender();
    setStyleSheet(QString("QFrame#%1{border: 2px solid red}").arg(frame->objectName()));
}
