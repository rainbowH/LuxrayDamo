#ifndef DBMYSQL_H
#define DBMYSQL_H

#include <QtSql>

class DBMySql
{
public:
    DBMySql(QString name);

    bool connectDB();
    bool runSql(QString sql);
    void close();
    QSqlDatabase database;
    QSqlQuery *query;

private:
    QString databaseName;
};

#endif // DBMYSQL_H
