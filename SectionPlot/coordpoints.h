#ifndef COORDPOINTS_H
#define COORDPOINTS_H

#include <QObject>
#include "objects/point.h"
class CoordPoints
{
public:
    CoordPoints();
    Point firstPoint;
    Point secondPoint;
    bool isFirstPoint;
    bool isSecondPoint;
};

#endif // COORDPOINTS_H
