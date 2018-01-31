#include "log.h"
#include "dbsqlite.h"

Log::Log()
{

}

void Log::commitToDB()
{
    DBSqlite *db = new DBSqlite;
    db->connectDB();
    QString sql = QString("insert into Log(Time,Event,User) values('%1','%2','%3')")
            .arg(this->dateTime.toString("yyyy-MM-dd hh:mm::ss")).arg(this->eventOperation)
            .arg(this->user);
    db->runSql(sql);

    db->close();
    delete db;
}
