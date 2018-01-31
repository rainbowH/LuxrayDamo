#ifndef POINT_H
#define POINT_H

#include <QVector2D>

class Point
{
public:
    Point(double x = 0, double y = 0);
    void setX(double x);
    void setY(double y);
    double x() const;
    double y() const;
    QVector2D toVector2D() const;

private:
    double X, Y;
};

#endif // POINT_H
