#include "arithmetic.h"

#include <math.h>
#include <QtMath>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <qcomplexnumber.h>
#include "fftw3.h"
#define PI 3.1415926

using namespace std;

Arithmetic::Arithmetic()
{
//    QFourierCalculator *fourier = new QFourierCalculator();
}

QList<double> Arithmetic::compensation(QList<double> standard, QList<double> testing, double length)
{
    double step_standard = length / standard.count();
    double step_testing = length / testing.count();
    double plus_standard = step_standard,  plus_testing = step_testing;

    for(int i=0; i< testing.length(); i++)
    {
        double tmp_dec = 0;
        plus_standard = step_standard;
        for(int j=0; j<standard.length(); j++)
        {
            float a = plus_testing - plus_standard;


            if(plus_testing > plus_standard && plus_testing > (plus_standard+step_standard))
            {
                tmp_dec = qAbs(plus_standard - plus_testing);
                plus_standard+= step_standard;
                continue;
            }
            else if(a<=0.00001)
            {
                if(tmp_dec > qAbs(plus_standard - plus_testing))
                {
                    double tmp_test = testing.at(i) - standard.at(j);
                    testing.replace(i, tmp_test);
                }
                else
                {
                    double tmp_test = testing.at(i)- standard.at(j - 1);
                    testing.replace(i, tmp_test);
                }
                plus_standard += step_standard;
                break;
            }

            tmp_dec = qAbs(plus_standard - plus_testing);
            plus_standard += step_standard;
        }
        plus_testing += step_testing;
    }
    return testing;
}

QList<double> Arithmetic::standardCompensation(QList<double> standard, QList<double> testing, double length)
{
    double step_S = length / standard.length();
    double step_T = length / testing.length();
    double sum_S = 0, sum_T = 0;
    QList<double> compensationTesting;
    for(int i=0, j=0; i<testing.length(); i++)
    {
        qDebug() << i;
        //相等时，直接补偿
        if(sum_S == sum_T)
        {
            compensationTesting.append(testing.at(i) - standard.at(j));
        }

        //标准点比测试点小
        else if(sum_S < sum_T)
        {
            //测试用
            if(i == 2637)
                qDebug() << "到了！";
            //下一个标准点仍小于测试点，自加后继续循环
            if(sum_S+step_S < sum_T)
            {
                sum_S += step_S;
                j++;
                i--;
                continue;
            }
            //下一个标准点比补偿点大或者相等
            else
            {
                //此标准点与补偿点的差值 < 下一个标准点与补偿点的差值，以此标准点作为补偿点
                if(qAbs(sum_S - sum_T)< qAbs(sum_S+step_S - sum_T))
                {
                    compensationTesting.append(testing.at(i) - standard.at(j));
                }
                //此标准点与补偿点的差值 >= 下一个标准点与补偿点的差值，以下一个标准点作为补偿点
                else
                {
                    compensationTesting.append(testing.at(i) - standard.at(j+1));
                }
            }
        }
        sum_S += step_S;
        sum_T += step_T;
        j++;
    }
    return compensationTesting;
}

QList<double> Arithmetic::laserCompensateStandard(QList<double> standard, QList<double> H, QList<double> H1, double length)
{
    QList<double> cha;
    for(int i=0; i< H.length(); i++)
    {
        cha.append(H1.at(i) - H.at(i));
    }
    return standardCompensation(cha, standard, length);

}

QList<double> Arithmetic::eddyValueTransf(QList<double> datas)
{
    QList<double> values;
    for(int i=0; i< datas.length(); i++)
    {
        values.append(datas.at(i)*0.865/1000);
    }
    return values;
}

QList<double> Arithmetic::jointEddyDatas(QList<QList<double>> *datas)
{
    QList<double> datasJoint;
    int index = 0;
    for(int i=0; i<datas->length(); i++)
    {
        for(int j=index; j<datas->at(i).length()- 1; j++)
        {
            datasJoint.append(datas->at(i).at(j));
            if(i+1<datas->length() && (datas->at(i).at(j) - datas->at(i+1).at(j))>0 &&
                    (datas->at(i).at(j+1)- datas->at(i+1).at(j+1))<0 &&
                    datas->at(i).at(j) > 1000)
            {
                if(abs(datas->at(i).at(j) - datas->at(i+1).at(j)) <
                        (datas->at(i).at(j+1)- datas->at(i+1).at(j+1)))
                    index = j;
                else
                    index = j+1;
                break;
            }
        }
    }

    for(int i=0; i<datasJoint.length(); i++)
    {
        if(datasJoint.at(i) < 1500)
        {
            for(int j=0; j< datas->length(); j++)
            {
                QList<double> tmp = datas->at(j);
                tmp.removeAt(i);
                datas->replace(j, tmp);
                qDebug()<< "datas["<<j<<"].length="<<tmp.length();
            }
            datasJoint.removeAt(i);
             qDebug()<< "datasJoint.length="<<datasJoint.length();

            i --;
        }
    }
    //样本数据最后一行去掉
    for(int j=0; j< datas->length(); j++)
    {
        QList<double> tmp = datas->at(j);
        tmp.removeLast();
        datas->replace(j, tmp);
        qDebug()<< "datas["<<j<<"].length="<<tmp.length();
    }
    return datasJoint;
}

double Arithmetic::getStep(double length, QList<double> Data1, QList<double> Data2)
{
    if(Data1.length() <= 0 || Data2.length() <=0)
        return -1;
    int crossIndex = getNearCrossIndex(Data1, Data2);
    double step = length / crossIndex;
    return step;
}

//r:激光处车轮半径， L：接近开关到7200长度， l:车轮到7200长度，h:7200高度，belta:7200角度，step：x轴步长
double Arithmetic::getX2(double r, double L, double l, double h, double belta, double step, QList<double> jointedDatas)
{
    double l_pre = L- l;
    int xpre_index = getNearValueIndex(jointedDatas, l_pre, step);
    double x_pre = step * xpre_index;
    double y_pre = jointedDatas.at(xpre_index);
    double x1 = x_pre - y_pre*qCos(qDegreesToRadians(60.0))/1000;
    x1 = step*getNearValueIndex(jointedDatas, x1, step);
    double l1 = r*qAcos((r-h)/r);
    double l2 = r*qDegreesToRadians(belta);
    double x2 = x1+l1+l2;

    return x2;
}

int Arithmetic::getOriIterStartPoint(double x2, double step, QList<double> jointedData)
{
    int c1 = x2/step;
    //如果后续距离足够，则直接返回当前点数
//    if(2*PI*r + x2 < jointedData.length()*step)
//        return c2;
//    //后续距离不够，要往前递进
//    double deltaX = 2*PI*r - (jointedData.length()*step - x2);
//    double x_ppre = x2 - deltaX;
//    c2 = getNearValueIndex(jointedData, x_ppre, step);
    return c1;
}

int Arithmetic::getIterStartPoint(int c1, double r, QList<double> jointedDatas, double step)
{
    if(2*PI*r + c1*step*1.0 <= jointedDatas.length()*step)
        return c1;
    else
    {
        double deltaX = 2*PI*r -(jointedDatas.length() *step - c1*step*1.0);
        double x_ppre = c1*step*1.0 - deltaX;
        int c2 = getNearValueIndex(jointedDatas, x_ppre, step);
        return c2;
    }
}

double Arithmetic::getPositionR(QList<double> jointedData, int curIndex, int rIndex, double r)
{
    return r+(jointedData.at(rIndex) - jointedData.at(curIndex))*0.865/1000.0;
}


double Arithmetic::calcMeanRadius(QList<double> x, QList<double> *lastr, int startindex, double r, double step, double precision)
{
    static int i =0;
    i++;
    qDebug() <<"迭代次数"<< i;
    double c = 2*PI*r;
    int count = 1;
    double plusX = step;
    double meanRadius = r;
    lastr->clear();
    int c2;
    double R;

    c2 = Arithmetic::getIterStartPoint(startindex, meanRadius, x, step)-1;

    R = Arithmetic::getPositionR(x, c2, startindex, meanRadius);

    lastr->append(R);

    while(plusX < c)
    {
        double tmpR = R-(x.at(c2) - x.at(c2 + count))*0.865/1000;
        lastr->append(tmpR);
        plusX += step;
        meanRadius += tmpR;
        count ++;
    }
    meanRadius /= count;

    if(qAbs(meanRadius - r) < precision)
        return meanRadius;
    else
    {
        return calcMeanRadius(x,lastr, startindex, meanRadius, step, precision);
    }
//        return calcMeanRadius(x,lastr, startindex, meanRadius, step, precision);
/*============一开始的写法=================
    double c = 2 * PI * r;
    int count =1;
    double plusX = 0.0;
    double meanRadius = r;
    lastX->clear();
    lastX->append(r);
    while(plusX <= c)
    {
        plusX += stepX;
        meanRadius += (r+(x.at(0)- x.at(count)/* *0.865/1000));       //x中已经做过转换了
        lastX->append(r+(x.at(0) - x.at(count)));
        count ++;
    }
    meanRadius /= count;

    if(qAbs(meanRadius - r) < precision)
        return meanRadius;
    else
        return calcMeanRadius(x, lastX, meanRadius, stepX, precision);
===================================================================*/
}


QList<Point> Arithmetic::rotatePoints(QList<Point> pointList, double alpha)
{
    QList<Point> pointList2;
    Point point;
    //旋转补正
    for(int i=0; i<pointList.length(); i++)
    {
        double x =pointList.at(i).x();
        double y =pointList.at(i).y();
        point.setX(x*qCos(alpha)-y*qSin(alpha));
        point.setY(x*qSin(alpha)+y*qCos(alpha));
        pointList2.append(point);
    }
    //偏移补正
//    int index = getMaxIndex(pointList2);
//    double deltaX = standardPoint.x() - pointList2.at(index).x();
//    double deltaY = standardPoint.y() - pointList2.at(index).y();

//    for(int i=0; i< pointList2.length(); i++)
//    {
//        point.setX(pointList2.at(i).x() + deltaX);
//        point.setY(pointList2.at(i).y() + deltaY);
//        pointList2.replace(i, point);
//    }
    return pointList2;
}

double Arithmetic::calcWheelEdgeR(QList<double> eddyDatas1, QList<double> eddyDatas2, QString laserFileName)
{
    //截取
       QList<double> eddyDatas = spliteEddyDatas(eddyDatas1, eddyDatas2);

        int count_805 = eddyDatas.length();
    //计算
        //计算长度230处是第几个点
        int count_230 = 230*count_805/805;
        //计算激光是第几条
        int count_laser = count_230 / 2;
    //找线
        QFile file(laserFileName);
        file.open(QFile::ReadOnly);
        QString text;
       //略过前面没用的数据
       for(int i=0; i< 800*(count_laser - 1)+(count_laser-1); i++)
           text = file.readLine();
       //获得这800个点
       QList<Point> laserList;
       for(int i=0; i<800; i++)
       {
           Point point;
           QString tmp = file.readLine();
           if(tmp == "")
               continue;
           tmp = tmp.trimmed();
           QString tmpNumber = tmp.left(tmp.indexOf('\t')).trimmed();
           point.setX(tmpNumber.toDouble()/100000);
           tmpNumber = tmp.right(tmp.length() - tmpNumber.length()).trimmed();
           point.setY(tmpNumber.toDouble()/100000);

           laserList.append(point);
       }
       //寻找最大的那个点
       int index = getMaxIndex(laserList);
       double yMax = laserList.at(index).y();

    //计算轮缘半径
       double a = (80 - yMax)*qSin(qDegreesToRadians(62.82));
       double b = 229.4- (80 - yMax)*qCos(qDegreesToRadians(62.82));

       double r = ((a-19.95)*(a-19.95)+b*b)/(2*(a-19.95));
   return r;
}

double Arithmetic::calcWheelEdgeR(QList<double> eddyDatas1, QList<double> eddyDatas2, double length, QList<QList<Point> > laserDatas)
{
    //截取
       QList<double> eddyDatas = spliteEddyDatas(eddyDatas1, eddyDatas2);

        int count_805 = eddyDatas.length();
    //计算
        //计算长度230处是第几个点
        int count_230 = 230*count_805/length;
        //计算激光是第几条
        int count_laser = count_230 / 2;
    //找线
        QList<Point> laserList = laserDatas.at(count_laser-1);
       //获得这800个点
//       for(int i=0; i<800; i++)
//       {
//           Point point;
//           point.setX(lasers.at(count_laser)->datasJointed.at(i).x());
//           point.setY(lasers.at(count_laser)->datasJointed.at(i).y());

//           laserList.append(point);
//       }
       //寻找最大的那个点
       int index = getMaxIndex(laserList);
       double yMax = laserList.at(index).y();

    //计算轮缘半径
       double a = (80 - yMax)*qSin(qDegreesToRadians(62.82));
       double b = 229.4 -(80 - yMax)*qCos(qDegreesToRadians(62.82));

       double r = ((a-19.95)*(a-19.95)+b*b)/(2*(a-19.95));
       qDebug() << "count_805="<<count_805<<" count_230="<<count_230<<" count_laser="<<count_laser;
       qDebug() << "yMax="<<yMax<<" a="<<a<<" b="<<b<<" r="<<r;
       return r;
}


double Arithmetic::calcSh(double Sh0, double r0, double meanr)
{
    return Sh0 +(r0 - meanr);
}


double Arithmetic::calcSd(double Sd0, double r0, double meanr)
{
    double Sd = Sd0 + qAbs(r0 - meanr);
    return Sd;
}

double Arithmetic::calcQr(QList<Point> laserLine, double Sd)
{
    int y = laserLine.at(getMaxIndex(laserLine)).y();
    int y1 = y-2;
    QList<int> index = getNearValueIndexs(laserLine, y1);

    return qAbs(Sd += laserLine.at(0).x() - laserLine.at(index.at(1)).x());
}

QList<Graze *> Arithmetic::GrazeAnalyze(QList<double> rList, double meanR)
{
    QList<Graze *> GrazeList;
//    int count =0;
//    double angle =0.0;
    double step_angle = 360.0/rList.length();
    QList<double>tmpRList;
    for(int i=0; i<rList.length(); i++)
    {
        //检测到第一个擦伤点后，开始擦伤分析
        if(rList.at(i) + 0.05 < meanR)
        {
            Graze *graze = new Graze();
            graze->positionStart = i*step_angle;
            int j= i;
            //将接下来所有的擦伤点都存到临时数组
            while(rList.at(i)+0.05 < meanR && i< rList.length())
            {
                tmpRList.append(rList.at(i));
                i++;
            }
            graze->positionEnd = i*step_angle;
            //根据本区间中最高值和最低值算出深度
            //根据本区间中最低值可得角度位置
            //根据本区间长度可以得出擦伤宽度
            int jMin = Arithmetic::getMinIndex(tmpRList);
            int jMax = Arithmetic::getMaxIndex(tmpRList);
            double min = tmpRList.at(jMin);
            double max = tmpRList.at(jMax);
            double deep = max - min;
            graze->grazeDeep = deep;
//            graze->angle = (jMin + i)*step_angle;
            graze->grazeWide = qDegreesToRadians(tmpRList.length()*step_angle)*meanR;

            GrazeList.append(graze);
            tmpRList.clear();
//            i+=j; i--;
        }
    }

    //起点处擦伤与终点处擦伤合并
    if(GrazeList.at(GrazeList.length() -1)->positionEnd == 360.00 && GrazeList.at(0)->positionStart == 0.00)
    {
        Graze *grazeEnd = GrazeList.at(GrazeList.length() -1);
        Graze *grazeStart = GrazeList.at(0);
        Graze *graze = new Graze();
        graze->positionStart = grazeEnd->positionStart;
        graze->positionEnd = grazeStart->positionEnd;
        graze->grazeWide = grazeEnd->grazeWide + grazeStart->grazeWide;
        graze->grazeDeep = grazeEnd->grazeDeep > grazeStart->grazeDeep ? grazeEnd->grazeDeep : grazeStart->grazeDeep;
        GrazeList.removeLast();
        GrazeList.replace(0, graze);
        delete grazeEnd;
        delete grazeStart;
    }

    return GrazeList;
}

int Arithmetic::DamageAnalyze(QList<double> valuesList, int size, QList<double> *resultList)
{
    if(valuesList.length() <=0)
        return -1;
    fftw_complex *in, *out;
    fftw_plan p;
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    p = fftw_plan_dft_1d(487, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    for(int i=0; i<size; i++)
    {
        in[i][0] = valuesList.at(i);
        in[i][1] = 0;
    }

    fftw_execute(p);

//    QList<double> resultList;
    for(int i=0; i< size; i++)
    {
        double tmp = qSqrt(qPow(out[i][0], 2) + qPow(out[i][1], 2));
        resultList->append(tmp);
    }
    return getMaxIndex(*resultList);
}

QList<Point> Arithmetic::jointLaserDatas(QList<Point> laserDatas7080, QList<Point> laserDatas7200)
{
    QList<Point> datasJointed;
    int index1 = 0, index2 = 0;

//step.1 旋转和7080的倒置

//step.2 找出最低的值的位置(Y值最大的那个点)
    for(int i=0; i<laserDatas7080.length(); i++)
    {
        if(laserDatas7080.at(index1).y() < laserDatas7080.at(i).y())
        {
            index1 = i;
        }
    }

    for(int i=0; i<laserDatas7200.length(); i++)
    {
        if(laserDatas7200.at(index2).y() < laserDatas7200.at(i).y())
        {
            index2 = i;
        }
    }

//step.3 7200数据移动
    //偏移
    double deltaY = laserDatas7080.at(index1).y() - laserDatas7200.at(index2).y();
    double deltaX = laserDatas7080.at(index1).x() - laserDatas7200.at(index2).x();

    for(int i=0; i< laserDatas7200.length(); i++)
    {
        Point point(laserDatas7200.at(i).x() + deltaX, laserDatas7200.at(i).y() + deltaY);
        laserDatas7200.replace(i, point);
    }
//step.4 拼接
    for(int i=0; i<=index1; i++)
    {
        datasJointed.append(laserDatas7080.at(i));
    }

    for(int i= index2+1; i<laserDatas7200.length(); i++)
    {
        datasJointed.append(laserDatas7200.at(i));
    }

    return datasJointed;
}



bool Arithmetic::isDirExist(QString fullPath)
{
    QDir dir(fullPath);
    if(dir.exists())
        return true;
    else
    {
        if(dir.mkdir(fullPath))
            return true;
        else
            return false;
    }
}

//QFile Arithmetic::isFileExist(QString fullPath)
//{
//    QFile file(fullPath);
//    file.open(QFile::WriteOnly);

//    return file;
//}
int Arithmetic::getMaxIndex(QList<double> datas)
{
    int index = 0;
    for(int i=0; i<datas.length(); i++)
    {
        if(datas.at(i) - datas.at(index) > 0.000000001)
        {
            index = i;
        }
    }
    return index;
}

int Arithmetic::getMaxIndex(QList<float> datas)
{
    int index = 0;
    for(int i=0; i<datas.length(); i++)
    {
        if(datas.at(i) > datas.at(index))
        {
            index = i;
        }
    }
    return index;
}

int Arithmetic::getMinIndex(QList<double> datas)
{
    int index = 0;
    for(int i=1; i<datas.length(); i++)
    {
        if(datas.at(i) < datas.at(index))
            index = i;
    }
    return index;
}

int Arithmetic::getMinIndex(QList<float> datas)
{
    int index = 0;
    for(int i=1; i<datas.length(); i++)
    {
        if(datas.at(i) < datas.at(index))
            index = i;
    }
    return index;
}


int Arithmetic::getMaxIndex(QList<Point> datas)
{
    int index = 0;
    for(int i=0; i<datas.length(); i++)
    {
        if(datas.at(i).y() > datas.at(index).y())
            index = i;
    }
    return index;
}

QList<int> Arithmetic::getNearValueIndexs(QList<Point> datas, double value)
{
    QList<int> indexList;
    int index = 0;
    int index_max = getMaxIndex(datas);
    double deltaY = qAbs(datas.at(0).y() - value);
    //前半段
    for(int i=1; i<index_max; i++)
    {
        if(qAbs(datas.at(i).y() - value) < deltaY)
        {
            deltaY = qAbs(datas.at(i).y() - value);
            index = i;
        }
    }
    indexList.append(index);
    //后半段
    index = index_max;
    deltaY = qAbs(datas.at(index).y() - value);
    for(int i= index_max+1; i< datas.length(); i++)
    {
        if(qAbs(datas.at(i).y() - value) < deltaY)
        {
            deltaY = qAbs(datas.at(i).y() - value);
            index = i;
        }
    }
    indexList.append(index);
    return indexList;
}

int Arithmetic::getNearValueIndex(QList<Point> datas, double x)
{
    for(int i= 1; i< datas.length()-1; i++)
    {
        if(((datas.at(i).x() - x < 0) && (datas.at(i+1).x() - x > 0)) ||
            ((datas.at(i).x() - x> 0)&& (datas.at(i+1).x() - x< 0)))
        {
            if(qAbs(datas.at(i).x() - x) < qAbs(datas.at(i+1).x() - x))
                return i;
            else
                return i+1;
        }
    }
    return datas.length()-1;
}

int Arithmetic::getNearValueIndex(QList<double> datas, double length, double distance)
{
    for(int i=0; i< datas.length(); i++)
    {
        if(distance *i <= length && distance *(i+1) > length)
        {
            if(qAbs(length - distance*i) < qAbs(length - distance*(i+1)))
                return i;
            else
                return i+1;
        }
    }
    return -1;
}

QList<QList<double>> Arithmetic::reverseTable(QList<QList<double> > datasTable)
{
    QList<QList<double>> reversedTable;
    QList<double> tmp;
    for(int j=0; j<datasTable.at(0).length(); j++)
    {
        for(int i=0; i<datasTable.length(); i++)
        {
            tmp << datasTable.at(i).at(j);
        }
        reversedTable <<tmp;
        tmp.clear();
    }
    return reversedTable;
}

QList<QList<QVariant>> Arithmetic::reverseTable(QList<QList<QVariant> > datasTable)
{
    QList<QList<QVariant>> reversedTable;

    qDebug() << datasTable.at(0).length();
    for(int j=0; j<datasTable.at(0).length(); j++)
    {
        QList<QVariant> tmp;
        for(int i=0; i<datasTable.length(); i++)
        {
            qDebug() << "i= "<<i <<" length1 = " <<datasTable.length();
            qDebug() << "length2="<<datasTable.at(i).length();
            if(datasTable.at(i).at(j).isValid())
            {
                QString x = QString("table[%1][%2]").arg(i).arg(j);
                qDebug() <<x;
                tmp << datasTable.at(i).at(j);
            }
            else
                tmp << QVariant(-9999);

            qDebug()<<"i="<<i<<" j="<<j<<" value="<<datasTable.at(i).at(j);
        }
        reversedTable <<tmp;
        tmp.clear();
    }
    return reversedTable;
}

QList<QList<QVariant> > Arithmetic::TableDoubleToVariant(QList<QList<double> > table)
{
    QList<QList<QVariant>> datas;
    for(int i=0; i< table.length(); i++)
    {
        QList<double> tablerow = table.at(i);
        QList<QVariant> tmp;
        for(int j=0; j<tablerow.length(); j++)
        {
            tmp.append(QVariant(tablerow.at(j)));
        }
        datas.append(tmp);
    }
    return datas;
}

QList<QList<double> > Arithmetic::TableVariantToDouble(QList<QList<QVariant> > table)
{
    QList<QList<double>> datas;
    for(int i=0; i< table.length(); i++)
    {
        QList<QVariant> tablerow = table.at(i);
        QList<double> tmp;
        for(int j=0; j< tablerow.length(); j++)
        {
            tmp.append(tablerow.at(j).toDouble());
        }
        datas.append(tmp);
    }
    return datas;
}

QVariant Arithmetic::TableToVariant(QList<QList<QVariant> > table)
{
    QList<QVariant> tmp;
    for(int i=0; i< table.length(); i++)
    {
        tmp.append(QVariant(table.at(i)));
    }
    QVariant var(tmp);
    return var;
}


QList<double> Arithmetic::reverseList(QList<double> data)
{
    QVector<double> vector = data.toVector();
    std::reverse(vector.begin(), vector.end());
    return vector.toList();
}

QList<QVariant> Arithmetic::reverseList(QList<QVariant> data)
{
    QVector<QVariant> vector = data.toVector();
    std::reverse(vector.begin(), vector.end());
    return vector.toList();
}

QList<Point> Arithmetic::reverseList(QList<Point> data)
{
    QVector<double> x, y;
    for(int i=0; i<data.length(); i++)
    {
        x.append(data.at(i).x());
        y.append(data.at(i).y());
    }
    std::reverse(y.begin(), y.end());
    QList<Point> points;
    Point point;
    for(int i=0; i< data.length(); i++)
    {
        point.setX(x.at(i));
        point.setY(y.at(i));
        points.append(point);
    }
    return points;
}

QList<double> Arithmetic::spliteEddyDatas(QList<double> eddyDatas1, QList<double> eddyDatas2)
{
    //方法二
    int index =0;
    QList<double> eddyDatas;
    for(int i=0; i< eddyDatas1.length(); i++)
    {
        double cha1 = eddyDatas1.at(i)- eddyDatas2.at(i);
        double cha2 = eddyDatas1.at(i+1) - eddyDatas2.at(i+1);
        if(cha1 >=0 && cha2 <= 0)
        {
            index = qAbs(cha1) < qAbs(cha2) ? i : i+1;
            break;
        }
    }

    for(int i=0; i<= index; i++)
        eddyDatas.append(eddyDatas1.at(i));

    return eddyDatas;

/*方法一
    eddyDatas = reverseList(eddyDatas);
    double data = eddyDatas.at(0);
    int index = 0;
    //找点
    for(int i=0; i< eddyDatas.length(); i++)
    {
        if(qAbs(eddyDatas.at(i) - eddyDatas.at(i+50)) < 10)
        {
            index = i;
            break;
        }

    }
    //删除不用的点
    for(int i=0; i< index; i++)
    {
        eddyDatas.removeFirst();
    }
    //倒置回来
    eddyDatas = reverseList(eddyDatas);
    return eddyDatas;
*/
}

int Arithmetic::getNearCrossIndex(QList<double> data1, QList<double> data2)
{
    for(int i=0; i< data1.length(); i++)
    {
        if(data1.at(i) - data2.at(i) >= 0 &&
                data1.at(i+1) - data2.at(i+1) < 0 &&
                data1.at(i) > 1000)
        {
            if(qAbs(data1.at(i) - data2.at(i)) < qAbs(data1.at(i+1) - data2.at(i+1)))
                return i;
            else
                return i+1;
        }
    }
    return -1;
}

