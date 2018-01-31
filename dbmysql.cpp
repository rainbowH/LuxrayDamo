#include "dbmysql.h"
#include <QDebug>
#include <QMessageBox>

DBMySql::DBMySql(QString name)
{
    this->databaseName = name;
}

bool DBMySql::connectDB()
{
    database = QSqlDatabase::addDatabase("QMYSQL");
//    database.setHostName("DESKTOP-VTBGOA0");
    database.setDatabaseName(databaseName);
//    database.setPort(3306);
    database.setUserName("skyage");
    database.setPassword("tiannian");

    if(!database.open())
    {
        qDebug() << "connect failed!";
        QMessageBox::warning(NULL, "警告", "连接数据库"+databaseName+"失败");
        qDebug() <<database.lastError();
        return false;
    }
    else
    {
        query = new QSqlQuery(database);
        qDebug() << "connect successfully!";
        return true;
    }
}

bool DBMySql::runSql(QString sql)
{
    if(!query->exec(sql))
    {
        qDebug() <<"exec failed!";
        qDebug() << query->lastError();
        return false;
    }
    else
    {
        return true;
    }
}

void DBMySql::close()
{
    database.close();
}
