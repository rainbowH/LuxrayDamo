#include "dbsqlite.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

DBSqlite::DBSqlite(QString sqlName)
{
    this->databaseName = sqlName;
}

bool DBSqlite::connectDB()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseName);
    if(!database.open())
    {
        qDebug() <<"connect failed!";
        return false;
    }
    else {
        query = new QSqlQuery(database);
        qDebug() << "connect successfully!";
        return true;
    }
}

bool DBSqlite::runSql(QString sql)
{
    if(!query->exec(sql))
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool DBSqlite::createTable(QString TableName)
{
    QString sql = "CREATE TABLE WheelModel (TrainID TEXT, Axle INT, WheelLocation INT, "
                  "RFID TEXT, RadiuJump DOUBLE, Sh DOUBLE, Sd1 DOUBLE, Sd2 DOUBLE, Gr1 DOUBLE, "
                  "Gr2 DOUBLE, InsideLength DOUBLE, EddyPath TEXT, Laser TEXT);";
    if(!query->exec(sql))
        return false;
    else
        return true;
}

bool DBSqlite::isExistValue(QString key, QString columnName, QString dbTable)
{
    QSqlQuery *query2 = new QSqlQuery(database);
    QString sql = QString("select %1 from %2").arg(columnName).arg(dbTable);
    query2->exec(sql);
    while(query2->next())
    {
        if(query2->value(0).toString() == key)
        {
            delete query2;
            return true;
        }
    }
    delete query2;
    return false;

}

QVariant DBSqlite::getValue(QString keytitle, QString key , QString title)
{
    QSqlQuery *query2 = query;
    query2->previous();
    while(query2->next())
    {
        if(query2->value(keytitle).toString() == key)
        {
            return query2->value(title);
        }
    }
    return QVariant();
}

void DBSqlite::close()
{
    database.close();
}

int DBSqlite::getTableRowCount(QString tableName)
{
    if(query->exec(QString("select count(*) from %1").arg(tableName)))
    {
        query->next();
        return query->value(0).toInt();
    }
    return -1;
}

QStringList DBSqlite::getAllValues(QString Table, QString title, QString Key, int count)
{
    QStringList tmpList;

    if(query->exec(QString("select * from %1 where %2 = '%3'").arg(Table).arg(title).arg(Key)))
    {
        if(query->next())
            for(int i=0; i< count; i++)
                tmpList << query->value(i).toString();
    }
    return tmpList;
}

QStringList DBSqlite::getAllValues(QString Table, QStringList titleList, QStringList KeyList, int count)
{
    QStringList tmpList;
    QString sql = QString("select * from %1 where ").arg(Table);
    QString condition = titleList.at(0) + " = '"+KeyList.at(0)+"'";
    for(int i=1; i< titleList.length(); i++)
    {
        condition = condition +" and " +titleList.at(i)+" = '"+KeyList.at(i)+"'";
    }
    sql = sql + condition;
    qDebug() <<sql;

    if(query->exec(sql))
    {
        if(query->next())
            for(int i=0; i<count; i++)
                tmpList << query->value(i).toString();

        return tmpList;
    }
}

