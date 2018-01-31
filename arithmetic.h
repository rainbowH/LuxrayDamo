#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include <QVector>
#include <QList>
#include <QMetaType>
#include "objects/point.h"
#include "objects/wheels.h"

class Arithmetic
{

public:
    Arithmetic();

    //标准轮补偿算法
    static QList<double> compensation(QList<double> standard, QList<double> testing, double length);
    static QList<double> standardCompensation(QList<double> standard, QList<double> testing, double length);
    //7200激光补偿标准轮补偿算法
    static QList<double> laserCompensateStandard(QList<double> standard, QList<double> H, QList<double> H1, double length);

    //电涡流位移值转换
    static QList<double> eddyValueTransf(QList<double> datas);
    //电涡流拼接
    static QList<double> jointEddyDatas(QList<QList<double> > *datas);

//====================迭代法相关函数集合==============================
    //1. 获取间隔距离算法（length: 电涡流板长，Data1：第一组电涡流数据，Data2：第二组电涡流数据）
    static double getStep(double length, QList<double>Data1, QList<double>Data2);
    //2. 计算从7200打在法线的表面一点，到这一点压到轨道上的行近距离(r位踏面法线点半径，h位7200同轨道的高度差，L位接近开关距离7200距离，l为车轮接触点与7200距离，belta为7200角度，
    //                  step为电涡流中每两点之间的距离长度，jointedData为拼接后的电涡流数据)
    static double getX2(double r, double L, double l, double h, double belta, double step, QList<double> jointedDatas);
    //3. 寻找迭代法的起点位置
    static int getOriIterStartPoint(double x2, double step, QList<double> jointedData);
    //4. 寻找迭代法偏移的起始位置
    static int getIterStartPoint(int c1, double r, QList<double> jointedDatas, double step);
    //5. 获取相应位置的半径
    static double getPositionR(QList<double> jointedData, int curIndex, int rIndex, double r);
    //6. 迭代计算平均半径算法
    static double calcMeanRadius(QList<double> x, QList<double> *lastr, int startindex, double r, double stepX, double precision);
//====================================================================


    //将激光点数据进行旋转
    static QList<Point> rotatePoints(QList<Point> pointList, double alpha);
    //两个激光数据的拼接
    static QList<Point> jointLaserDatas(QList<Point> laserDatas7080, QList<Point> laserDatas7200);
    //根据电涡流数据和激光数据计算轮缘半径
    static double calcWheelEdgeR(QList<double> eddyDatas1, QList<double> eddyDatas2, QString laserFileName);
    static double calcWheelEdgeR(QList<double> eddyDatas1, QList<double> eddyDatas2, double length, QList<QList<Point> > laserDatas);

    //计算准确的Sh
    static double calcSh(double Sh0, double r0, double meanr);
    //计算准确的Sd
    static double calcSd(double Sd0, double r0, double meanr);
    //计算Qr
    static double calcQr(QList<Point> laserLine, double Sd);


    //擦伤分析
    static QList<Graze *> GrazeAnalyze(QList<double> rList, double meanR);

    //傅里叶级数转换（将时域转换成频域，进而判断出相应的损伤信号）
    static int DamageAnalyze(QList<double> valuesList, int size, QList<double> *resultList);

    static bool isDirExist(QString fullPath);
//    static QFile isFileExist(QString fullPath);
    static int getMaxIndex(QList<Point> datas);
    static int getMaxIndex(QList<double> datas);
    static int getMaxIndex(QList<float> datas);
    static int getMinIndex(QList<double> datas);
    static int getMinIndex(QList<float> datas);
    static QList<int> getNearValueIndexs(QList<Point> datas, double value);
    static int getNearValueIndex(QList<Point> datas, double x);
    static int getNearValueIndex(QList<double> datas, double length, double distance);
    //翻转表格数据（将列变成行，行变成列）
    static QList<QList<double>> reverseTable(QList<QList<double>> datasTable);
    static QList<QList<QVariant>> reverseTable(QList<QList<QVariant>> datasTable);

    //表格的variant与double互转
    static QList<QList<QVariant>> TableDoubleToVariant(QList<QList<double>> table);
    static QList<QList<double>> TableVariantToDouble(QList<QList<QVariant>> table);
    static QVariant TableToVariant(QList<QList<QVariant>> table);


    //获取两个列表中交叉点的邻近点的索引
    static int getNearCrossIndex(QList<double> data1, QList<double> data2);

    //QList倒转
     static QList<double> reverseList(QList<double> data);
     static QList<QVariant> reverseList(QList<QVariant> data);
     static QList<Point> reverseList(QList<Point> data);



     //截取电涡流数据
     static QList<double> spliteEddyDatas(QList<double> eddyDatas1, QList<double> eddyDatas2);

};

#endif // ARITHMETIC_H
