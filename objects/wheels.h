#ifndef WHEELS_H
#define WHEELS_H

#include <QString>
#include <QList>
#include <QDateTime>
#include <QDate>
#include "point.h"

//擦伤数据类
class Graze
{
public:
    Graze();
    double positionStart, positionEnd, grazeDeep, grazeWide;

};

//电涡流数据类
class EddyDatas{
public:
    QList<double> Data1, Data2, Data3, Data4;
    QList<double> DataJointed;
//    QList<QDateTime> datetime;
    EddyDatas(QList<double> data1, QList<double>data2, QList<double>data3, QList<double>data4);


    EddyDatas(QList<QList<double>> dataTable);
    EddyDatas(){}


};
//电激光数据类
class LaserDatas{
public:
    QList<QList<Point>> laser7080, laser7200, ori7080, ori7200;
    QList<Point> effectLaser7080, effectLaser7200;
    QList<Point> datasJointed;
    double alpha;
    double Sh, Sd1, Sd2, Gr1, Gr2;

    LaserDatas(QList<QList<Point>> origi7080, QList<QList<Point>> origi7200, double angle);

    LaserDatas(QList<QList<Point>> origi7080, QList<QList<Point>> origi7200);

    LaserDatas(QList<Point> laser7080Line, QList<Point> laser7200Line, double angle);

    //转换7080激光数据
    void get7080Data();
    //转换7200激光数据
    void get7200Data();
    //计算Sh
    void calcSh();
    //计算Sd
    void calcSd();
    //计算Gr
    void calcGr();
    void initDatas();
};
/*原激光数据类
class LaserDatas{
public:
    QList<Point> Laser7080, Laser7200;
    QList<Point> origi7080, origi7200;
    QList<Point> datasJointed;
    Point rotatePoint;
    double alpha;
    double Sh, Sd1, Sd2, Gr1, Gr2;          //需要求出的三个参数

    LaserDatas(QList<Point> origi7080, QList<Point> origi7200, double angle);

    //转换7080激光数据
    void get7080Data();
//计算Sh
    void calcSh();
//计算Sd
    void calcSd();

//计算Gr
    void calcGr();
    void initDatas();
};
*/
//车轮类
class Wheels
{
public:
    Wheels(QString type = "", QString num= "", QString texture = "");
    QList<QList<double>> datas;
    EddyDatas *eddy;
    LaserDatas *lasers;
//    QList<LaserDatas *>lasers;
//    LaserDatas *laser;          //打在法线上时那条激光线的数据
    QDateTime dateTime;
    char pathway;
    QString floatplanttype;     //辐板类型

    double r, R;                //车轮半径和轮缘半径
    double Sh, Sd, Qr;
    double mean_r;
    QList<double> rList;        //踏面一圈每一点的半径
    double radiuRunOut;         //径向跳动
    QMap<float, QVector<float>> map_rList;
     QVector<QVector2D> effectLaserLine;
    QString texture, RFID;      //材质和编号,型号
    int axle;                   //计轴
    double displacement;        //计算轮缘厚时上移的位移
    QString position;           //位置

    void getR();                //计算轮缘半径
    void getr();                //计算踏面半径
    void getMean_r();           //计算踏面平均半径
    void getAreaRList();        //获取区域的半径（给王岳的数据）
    void getEffectLaserLine();  //得到有效激光线上的数据（给王岳的数据）
    void getRadiuRunOut();      //获取径跳

    void calcAllDatas();

};
//轴类
class Axle{
public:
    Axle(int num, double length, Wheels *wheel1 = NULL, Wheels *wheel2 = NULL);
    void addWheel(Wheels *wheel);
    void removeWheel(QString RFID);
    Wheels *wheel(QString RFID);
    int No;
    double Length;
    QList<Wheels *> wheelsList;
    QDateTime datetime;

};
//列车类
class Train{
public:
    Train(QString rfid = "", QDateTime date1 = QDateTime::currentDateTime(), int marshalling = 6);
    QString RFID;               //列车号
    QDateTime datetime;         //列车检测时间
    int marshalling;            //编组数
    QString checher;
    QList<Axle *> axleList;

    void addAxle(Axle *axle);
    void removeAxle(int axleCount);
    Axle * axle(int No);
    Wheels *wheel(QString position);

    void clear();
    bool isExistAxle(Axle *axle);

};
//用户设置参数类
class UserParas{
public:
    UserParas(QString paragroup)
    {
        this->paraGroup = paragroup;
    }

    QString paraGroup;
    QString radialsType;
    QDateTime dateTime;
    double DMin, DMax;
    double deltaRMax;
    double shMin, shMax, sdMin, sdMax, qrMin, qrMax;
    int sdStart;
    double inLengthMin, inLengthMax;
    double deltaD1, deltaD2, deltaD3;
};
#endif // WHEELS_H
