#ifndef DBSQLITE_H
#define DBSQLITE_H

#include <QObject>
#include <QWidget>
#include <QtSql>
#include <QStringList>

class DBSqlite
{
public:
    DBSqlite(QString sqlName = "wheelsInfo.db");

    bool connectDB();
    bool runSql(QString sql);
    bool createTable(QString TableName);
    bool isExistValue(QString key, QString columnName, QString dbTable);
    void close();
    //获取相应表格行数
    int getTableRowCount(QString tableName);
    //获取表Table中符合titile字段等于key的所有字段
    QStringList getAllValues(QString Table, QString title, QString Key, int count);
    QStringList getAllValues(QString Table, QStringList titleList, QStringList KeyList, int count);
    QSqlDatabase database;
    QSqlQuery *query;
    QString databaseName;

    QVariant getValue(QString keytitle, QString key, QString title);

private:

};

#endif // DBSQLITE_H
