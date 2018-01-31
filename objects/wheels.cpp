#include "wheels.h"
#include "objects/point.h"
#include "arithmetic.h"
#include <QMessageBox>
#include <qmath.h>


Graze::Graze(){}

//车轮类函数
Wheels::Wheels(QString type, QString num, QString texture)
{
    eddy = new EddyDatas();
//    lasers = new LaserDatas();
}

void Wheels::getR()
{
//    QList<QList<Point>> laserdatas;
//    for(int i=0; i< lasers.length();i ++)
//    {
//        QList<Point> data = lasers.at(i)->datasJointed;
//        laserdatas.append(data);
//    }
//    R = Arithmetic::calcWheelEdgeR(eddy->Data1, eddy->Data2, 733, laserdatas);
}

void Wheels::getr()
{
     r = R - lasers->Sh;
}

void Wheels::getMean_r()
{
    double step = Arithmetic::getStep(733, eddy->Data1, eddy->Data2);
    double X2 = Arithmetic::getX2(r, 1080, 445, 20.5, 40.8, step, eddy->DataJointed);
    int c1 = Arithmetic::getOriIterStartPoint(X2, step, eddy->DataJointed);
    mean_r = Arithmetic::calcMeanRadius(eddy->DataJointed, &rList, c1, r, step, 0.05);
}


void Wheels::getAreaRList()
{
    double startX = lasers->datasJointed.at(0).x();
    double y_70 = lasers->datasJointed.at(Arithmetic::getNearValueIndex(lasers->datasJointed, startX + 70)).y();
    for(double i=startX +50; i<lasers->datasJointed.at(lasers->datasJointed.length()-1).x(); i+=0.1)
    {
        double H = lasers->datasJointed.at(Arithmetic::getNearValueIndex(lasers->datasJointed, i)).y()
                - y_70;

        QVector<float> tmp;
        for(int j= 0; j< rList.length(); j++)
        {
            tmp.append(rList.at(j) + H);
        }
        map_rList.insert(i, tmp);
    }
}

void Wheels::getEffectLaserLine()
{
    double startX = lasers->datasJointed.at(0).x();
    double y_70 = lasers->datasJointed.at(Arithmetic::getNearValueIndex(lasers->datasJointed,
                                                                        lasers->datasJointed.at(0).x()+70)).y();
    double maxValueIndex = Arithmetic::getMaxIndex(lasers->datasJointed);
    //7080数据部分
    for(double i = startX, j=0.0; i< lasers->datasJointed.at(maxValueIndex).x(); i+= 0.05, j++)
    {
//            double H = laser->datasJointed.at(Arithmetic::getNearValueIndex(laser->datasJointed, i)).y()
//                    - y_70;
        double H = lasers->datasJointed.at(j).y()
                - y_70;
        effectLaserLine.append(QVector2D(i, r+H));
    }
    //7200数据部分
    for(double i=lasers->datasJointed.at(maxValueIndex).x(), j= maxValueIndex;
        i<lasers->datasJointed.at(lasers->datasJointed.length()-1).x();
        i+=0.1, j++)
    {
        double H = lasers->datasJointed.at(j).y()
                - y_70;
        effectLaserLine.append(QVector2D(i, r+H));
    }
}

void Wheels::getRadiuRunOut()
{
    if(rList.length() > 0)
    {
        radiuRunOut = rList.at(Arithmetic::getMaxIndex(rList)) - rList.at(Arithmetic::getMinIndex(rList));
    }
}

void Wheels::calcAllDatas()
{
    if(lasers->effectLaser7080.length() <=0 || lasers->effectLaser7200.length() <=0)
    {
        QMessageBox::warning(NULL, "暂无获取到激光数据", "车轮的激光数据暂未获取完整！");
        return;
    }

    if(eddy->DataJointed.length() <=0)
    {
        QMessageBox::warning(NULL, "暂无电涡流数据", "车轮的电涡流数据暂未获取完整！");
        return;
    }

    if(lasers->datasJointed.length() <= 0)
    {
        QMessageBox::warning(NULL, "激光计算错误", "有效激光数据暂未获取！");
        return;
    }
    getR();

    getr();
    getMean_r();
    this->Sh = Arithmetic::calcSh(lasers->Sh, r, mean_r);
    if(displacement == 10)
    {
        Sd = Arithmetic::calcSd(lasers->Sd1, r, mean_r);
    }
    else
    {
        Sd = Arithmetic::calcSd(lasers->Sd2, r, mean_r);
    }
    this->Qr = Arithmetic::calcQr(lasers->datasJointed, Sd);
    getAreaRList();
    getEffectLaserLine();

    getRadiuRunOut();
}


//电涡流数据类函数区
EddyDatas::EddyDatas(QList<double> data1, QList<double>data2, QList<double>data3, QList<double>data4)
{
    Data1 = Arithmetic::eddyValueTransf(data1);
    Data2 = Arithmetic::eddyValueTransf(data2);
    Data3 = Arithmetic::eddyValueTransf(data3);
    Data4 = Arithmetic::eddyValueTransf(data4);
    QList<QList<double>> tmp;
    tmp << Data1 << Data2 << Data3 << Data4;
    DataJointed=Arithmetic::jointEddyDatas(&tmp);
}

EddyDatas::EddyDatas(QList<QList<double>> dataTable)
{
//    Data1 = Arithmetic::eddyValueTransf(dataTable.at(0));
//    Data2 = Arithmetic::eddyValueTransf(dataTable.at(1));
//    Data3 = Arithmetic::eddyValueTransf(dataTable.at(2));
//    Data4 = Arithmetic::eddyValueTransf(dataTable.at(3));
    DataJointed=Arithmetic::jointEddyDatas(&dataTable);
    Data1 = dataTable.at(0);
    Data2 = dataTable.at(1);
    Data3 = dataTable.at(2);
    Data4 = dataTable.at(3);


}


//2d激光数据类函数区
LaserDatas::LaserDatas(QList<QList<Point> > origi7080, QList<QList<Point> > origi7200, double angle)
    :ori7080(origi7080)
    ,ori7200(origi7200)
    ,alpha(qDegreesToRadians(-angle))
{
    initDatas();
}

LaserDatas::LaserDatas(QList<QList<Point> > origi7080, QList<QList<Point> > origi7200)
    :ori7080(origi7080)
    ,ori7200(origi7200)
{
    get7080Data();
    get7200Data();
}

LaserDatas::LaserDatas(QList<Point> laser7080Line, QList<Point> laser7200Line, double angle)
    :effectLaser7080(laser7080Line)
    ,effectLaser7200(laser7200Line)
    ,alpha(angle)
{
    initDatas();
}

void LaserDatas::get7080Data()
{
//    for(int i=0; i<ori7080.length(); i++)
//        laser7080.append(Arithmetic::reverseList(ori7080.at(i)));
    effectLaser7080 = Arithmetic::reverseList(effectLaser7080);
}

void LaserDatas::get7200Data()
{
//    for(int i=0; i< ori7200.length(); i++)
//        laser7200.append(Arithmetic::rotatePoints(ori7200.at(i), alpha));
    effectLaser7200 = Arithmetic::rotatePoints(effectLaser7200, alpha);
}

void LaserDatas::calcSh()
{
    double y1 = datasJointed.at(Arithmetic::getNearValueIndex(datasJointed, datasJointed.at(0).x()+70)).y();
    double y2 = datasJointed.at(Arithmetic::getMaxIndex(datasJointed)).y();

    Sh = y2 - y1;

}

void LaserDatas::calcSd()
{
    double y = datasJointed.at(Arithmetic::getNearValueIndex(datasJointed, datasJointed.at(0).x()+70)).y();
    double y1 = y + 10;
    double y2 = y + 12;

    QList<int> index = Arithmetic::getNearValueIndexs(datasJointed, y1);
//            Sd1 = qAbs(datasJointed.at(index.at(0)).x() - datasJointed.at(index.at(1)).x());
//            Gr1 = datasJointed.at(index.at(1)).x();
    Sd1 = index.length() == 1 ? datasJointed.at(index.at(0)).x() : datasJointed.at(index.at(1)).x();
    Gr1 = Sd1;
    Sd1 -= datasJointed.at(0).x();

    index = Arithmetic::getNearValueIndexs(datasJointed, y2);
    Sd2 = index.length() == 1 ? datasJointed.at(index.at(0)).x() : datasJointed.at(index.at(1)).x();
    Gr2 = Sd2;
    Sd2 -= datasJointed.at(0).x();
}

void LaserDatas::calcGr()
{
    int y = datasJointed.at(Arithmetic::getMaxIndex(datasJointed)).y();
    int y1 = y - 2;
    QList<int> index = Arithmetic::getNearValueIndexs(datasJointed, y1);

    Gr1 = qAbs(Gr1 - datasJointed.at(index.at(1)).x());
    Gr2 = qAbs(Gr2 - datasJointed.at(index.at(1)).x());
}

void LaserDatas::initDatas()
{
    get7080Data();
    get7200Data();
    //参数可能会互换
    datasJointed = Arithmetic::jointLaserDatas(effectLaser7080, effectLaser7200);
    calcSh();
    calcSd();
    calcGr();
}
/*原来激光数据类定义
LaserDatas::LaserDatas(QList<Point> origi7080, QList<Point> origi7200, double angle)
    :origi7080(origi7080)
    ,origi7200(origi7200)
    ,alpha(angle)
{
    initDatas();
}

void LaserDatas::get7080Data()
{
    Laser7080 = Arithmetic::reverseList(origi7080);
}

void LaserDatas::calcSh()
{
    double y1 = datasJointed.at(Arithmetic::getNearValueIndex(datasJointed, datasJointed.at(0).x()+70)).y();
    double y2 = datasJointed.at(Arithmetic::getMaxIndex(datasJointed)).y();

    Sh = y2 - y1;
}

void LaserDatas::calcSd()
{
    double y = datasJointed.at(Arithmetic::getNearValueIndex(datasJointed, datasJointed.at(0).x()+70)).y();
    double y1 = y + 10;
    double y2 = y + 12;

    QList<int> index = Arithmetic::getNearValueIndexs(datasJointed, y1);
//            Sd1 = qAbs(datasJointed.at(index.at(0)).x() - datasJointed.at(index.at(1)).x());
//            Gr1 = datasJointed.at(index.at(1)).x();
    Sd1 = index.length() == 1 ? datasJointed.at(index.at(0)).x() : datasJointed.at(index.at(1)).x();
    Gr1 = Sd1;
    Sd1 -= datasJointed.at(0).x();

    index = Arithmetic::getNearValueIndexs(datasJointed, y2);
    Sd2 = index.length() == 1 ? datasJointed.at(index.at(0)).x() : datasJointed.at(index.at(1)).x();
    Gr2 = Sd2;
    Sd2 -= datasJointed.at(0).x();
}

void LaserDatas::calcGr()
{
    int y = datasJointed.at(Arithmetic::getMaxIndex(datasJointed)).y();
    int y1 = y - 2;
    QList<int> index = Arithmetic::getNearValueIndexs(datasJointed, y1);

    Gr1 = qAbs(Gr1 - datasJointed.at(index.at(1)).x());
    Gr2 = qAbs(Gr2 - datasJointed.at(index.at(1)).x());
}

void LaserDatas::initDatas()
{
    get7080Data();
    Laser7200 = Arithmetic::rotatePoints(origi7200, alpha);
    //参数可能会互换
    datasJointed = Arithmetic::jointLaserDatas(Laser7080, Laser7200);
    calcSh();
    calcSd();
    calcGr();
}
*/

//轴类函数
Axle::Axle(int num, double length, Wheels *wheel1, Wheels *wheel2)
    :No(num)
    ,Length(length)
{
    if(wheel1 != NULL)
        wheelsList << wheel1;
    if(wheel2 != NULL)
        wheelsList<< wheel2;
}

void Axle::addWheel(Wheels *wheel)
{
    wheelsList << wheel;
    wheel->axle = this->No;
}

void Axle::removeWheel(QString RFID)
{
    for(int i=0; i< wheelsList.length(); i++)
    {
        if(wheelsList.at(i)->RFID == RFID)
        {
            wheelsList.removeOne(wheelsList.at(i));
            return;
        }
    }
}

Wheels *Axle::wheel(QString RFID)
{
    for(int i=0; i< wheelsList.length(); i++)
    {
        if(RFID == wheelsList.at(i)->RFID)
            return wheelsList.at(i);
    }
    return nullptr;
}

Train::Train(QString rfid, QDateTime date1, int marshalling)
{
    this->RFID = rfid;
    this->datetime = date1;
    this->marshalling = marshalling;
}

void Train::addAxle(Axle *axle)
{
    this->axleList.append(axle);
}

void Train::removeAxle(int axleCount)
{
    for(int i=0; i< axleList.length(); i++)
    {
        if(axleList.at(i)->No == axleCount)
        {
            axleList.removeOne(axleList.at(i));
            return;
        }
    }
}

Axle *Train::axle(int No)
{
    for(int i=0; i< axleList.length(); i++)
    {
        if(No == axleList.at(i)->No)
            return axleList.at(i);
    }
    return nullptr;
}

Wheels* Train::wheel(QString position)
{
    for(int i=0; i< this->axleList.length(); i++)
    {
        Axle *tmpAxle = this->axleList.at(i);
        for(int j=0; j< tmpAxle->wheelsList.length(); j++)
        {
            if(tmpAxle->wheelsList.at(j)->position == position)
                return tmpAxle->wheelsList.at(j);
        }
    }
    return nullptr;
}

void Train::clear()
{
    this->axleList.clear();
    RFID = "";
    datetime = QDateTime::currentDateTime();
    marshalling = 6;
}

bool Train::isExistAxle(Axle* axle)
{
    for(int i=0; i<axleList.length(); i++)
    {
        if(axleList.at(i)->No == axle->No)
            return true;
    }
    return false;
}


