#include "ticks.h"
#include <QPainter>
#include <QPen>
#include <QMessageBox>
#include <QDebug>
#include "dialogs/dlgpalorparasetting.h"

Ticks::Ticks(QWidget *parent, int tickCount, double rangemin, double rangemax) :
    QWidget(parent),
    tickCounts(tickCount*2-1),
    rangeMin(rangemin),
    rangeMax(rangemax)
{
    pressFlag = false;
    for(int i=0; i<tickCounts; i++)
    {
        QLabel *label = new QLabel(this);
        label->setAlignment(Qt::AlignRight);
        labelList.append(label);
    }
}

Ticks::~Ticks()
{
    removeLabels();
}

void Ticks::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawLine(QPointF(this->width()-5, 60), QPointF(this->width()-5, this->height()-60));

    int length = this->height() -120;
    int step = length / (tickCounts-1);
//画刻度
    for(int i=0; i< tickCounts; i++)
    {
        painter.drawLine(QPointF(this->width()-5, i*step+ 60), QPointF(this->width()-15, i*step+60));
    }

//写字
    //刻度值
    QList<double> valueList;
    double distance = (rangeMax - rangeMin)/(tickCounts/2);
    for(int i=0; i< tickCounts/2+1; i++)
        valueList<< rangeMax-i*distance;

    //写刻度
    QPen pen;
    pen.setWidth(1);
    painter.setPen(pen);
    int fontsize;
    for(int i=0; i<tickCounts/2+1; i++)
    {
//        QLabel *label = new QLabel(this);
//        labelList.append(label);
        labelList.at(i)->setText(QString::number(valueList.at(i), 'f', 2));
        int strcount = labelList.at(i)->text().length();
        QFont font;
        fontsize= step<labelList.at(i)->width()/4?step/3:labelList.at(i)->width()/4;
        font.setPixelSize(fontsize/**2/6*/);
        labelList.at(i)->setFont(font);
        labelList.at(i)->setGeometry(0, i*step+50, this->width()-20, step/4*3);
        labelList.at(i)->setAlignment(Qt::AlignRight);
        labelList.at(i)->show();
//        painter.drawText(QRectF(0, i*step, this->width()-20, 10), "111");
    }

    for(int i= tickCounts/2+1; i<tickCounts; i++)
    {
        labelList.at(i)->setText(QString::number(valueList.at(tickCounts-1-i), 'f', 2));
        QFont font;
        font.setPixelSize(fontsize);
        labelList.at(i)->setFont(font);
        labelList.at(i)->setGeometry(0, i*step+50, this->width()-20, step/4*3);
        labelList.at(i)->setAlignment(Qt::AlignRight);
        labelList.at(i)->show();
    }
    painter.end();
}

void Ticks::mouseDoubleClickEvent(QMouseEvent *event)
{
    onSettingParas();
}

void Ticks::mousePressEvent(QMouseEvent *event)
{
//    pressFlag = !pressFlag;
}

void Ticks::mouseReleaseEvent(QMouseEvent *event)
{
//    if(pressFlag)
//    {
//        for(int i=0; i<labelList.length(); i++)
//            labelList.at(i)->setFrameShape(QFrame::Box);
//    }
//    else {
//        for(int i=0; i<labelList.length(); i++)
//            labelList.at(i)->setFrameShape(QFrame::NoFrame);
//    }

}

void Ticks::onSetTicks(int count)
{
    removeLabels();
    this->tickCounts = count*2-1;

    for(int i=0; i<tickCounts; i++)
    {
        QLabel *label = new QLabel(this);
        labelList.append(label);
    }
    emit signalPolarTickCounts(count);
}

void Ticks::onSetRange(double rangemin, double rangemax)
{
    this->rangeMin = rangemin;
    this->rangeMax = rangemax;
    emit signalPolarRange(rangemin, rangemax);
    update();
}

void Ticks::onSetAngelRange(double min, double max)
{
    this->AngleMin = min;
    this->AngleMax = max;
}

void Ticks::onSettingParas()
{
    DlgPalorParaSetting *dlg = new DlgPalorParaSetting;
    connect(dlg, SIGNAL(signalRange(double,double)), this, SLOT(onSetRange(double,double)));
    connect(dlg, SIGNAL(signalTickCounts(int)), this, SLOT(onSetTicks(int)));
    connect(dlg, SIGNAL(signalAngleRange(double,double)), this, SIGNAL(signalPolarAngelRange(double,double)));
    connect(this, SIGNAL(signalRange(double,double)), dlg, SLOT(onSetRange(double,double)));
    connect(this, SIGNAL(signalAngleRange(double,double)), dlg, SLOT(onSetAngleRange(double,double)));
    connect(this, SIGNAL(signalTickCounts(int)), dlg, SLOT(onSetTickCounts(int)));
    emit signalRange(this->rangeMin, this->rangeMax);
    emit signalTickCounts((this->tickCounts+1)/2);
    emit signalAngleRange(this->AngleMin, this->AngleMax);
    dlg->show();
}
//移除所有label
void Ticks::removeLabels()
{

    qDebug() <<labelList.length();
    for(int i=0; i<labelList.length(); i++)
    {
        labelList.at(0)->hide();
        delete labelList.at(0);
        labelList.removeFirst();
    }
}

