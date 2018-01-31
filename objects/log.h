#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QWidget>
#include <QDateTime>

class Log
{
public:
    Log();
    QDateTime dateTime;
    QString eventOperation;
    QString user;

    void commitToDB();
};

#endif // LOG_H
