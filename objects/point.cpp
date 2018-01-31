#include "point.h"

Point::Point(double x, double y)
{
    setX(x);
    setY(y);
}

void Point::setX(double x)
{
    this->X = x;
}

void Point::setY(double y)
{
    this->Y = y;
}

double Point::x() const
{
    return X;
}

double Point::y() const
{
    return Y;
}

QVector2D Point::toVector2D() const
{
    QVector2D vector2D(X, Y);
    return vector2D;
}
