#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QObject>
#include <QTableView>
#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include "dbsqlite.h"


class TableView : public QTableView
{
    Q_OBJECT
public:
    TableView(QWidget *parent = 0);
    ~TableView();

    QMenu *menu;
    QAction *actionDelete;
    QAction *actionDtl;
    QAction *actionUndo;

    int curRow;
    QString trainNo, wheelNo;
    QDateTime dateTime;

signals:
    trainDoubleClicked(QString, QDateTime);
    wheelDoubleClicked(QString, QDateTime);

public slots:
    void onRequestMenu(const QPoint&point);
    void onDeleteRow();
    void onShowDtl();
    void onUndo();
    void onSearch(QString train, QString wheel, QDateTime time);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
    QSqlTableModel *model;
    DBSqlite *db;


};

#endif // TABLEVIEW_H
