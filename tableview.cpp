#include <QPoint>
#include <QDebug>
#include <QSqlError>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QMouseEvent>
#include "tableview.h"
#include "dialogs/dlgwheeldtl.h"

TableView::TableView(QWidget *parent)
    :QTableView(parent)
{
    db = new DBSqlite;
    db->connectDB();
    model = new QSqlTableModel(this, db->database);
    model->setHeaderData(0, Qt::Horizontal, "暂无数据");
    this->setModel(model);

    menu = new QMenu(this);
    actionDelete = new QAction("删除");
    actionDtl = new QAction("详情");
    actionUndo = new QAction("撤销操作");
    menu->addAction(actionDelete);
    menu->addAction(actionDtl);
//    menu->addAction(actionUndo);

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onRequestMenu(QPoint)));
    connect(actionDelete, SIGNAL(triggered(bool)), this, SLOT(onDeleteRow()));
    connect(actionUndo, SIGNAL(triggered(bool)), this, SLOT(onUndo()));
    connect(actionDtl, SIGNAL(triggered(bool)), this, SLOT(onShowDtl()));
}

TableView::~TableView()
{
    qDebug() <<"tableview has closed!";
    db->close();
    delete db;
}

void TableView::onRequestMenu(const QPoint &point)
{
    if(model->rowCount() > 0 && this->rowAt(point.y()) >=0)
    {
        curRow = this->rowAt(point.y());
        qDebug() <<"customContextMenu";
        actionDtl->setEnabled(true);
        actionDelete->setEnabled(true);
        actionUndo->setEnabled(true);
    }
    else
    {
        actionDtl->setEnabled(false);
        actionDelete->setEnabled(false);
    }
    menu->exec(QCursor::pos());


}

void TableView::onDeleteRow()
{
    model->database().transaction();
    model->removeRow(curRow);
    int ok = QMessageBox::warning(this, "确认信息", "是否将该数据彻底删除？", QMessageBox::Ok, QMessageBox::No);
    qDebug() << ok;
    if(ok == QMessageBox::Ok)
    {
        qDebug() << "Yes";
        model->database().commit();
    }
    else
    {
        qDebug() <<"no";
        model->revertAll();
        model->database().rollback();
    }
    model->select();
    this->setModel(model);
}

void TableView::onShowDtl()
{
    QString RFID = model->data(model->index(curRow, 3)).toString();
    qDebug() << RFID;
    DlgWheelDtl *dlg = new DlgWheelDtl(db, RFID);
    dlg->exec();
}

void TableView::onUndo()
{
    model->revertAll();
    model->database().rollback();
}

void TableView::onSearch(QString train, QString wheel, QDateTime time)
{
    this->trainNo = train;
    this->wheelNo = wheel;
    this->dateTime = time;
    QString timeStr = time.date().toString("yyyy-MM-dd")+" "+time.time().toString("HH:mm");
  //  model->removeColumns(4, 9);
    QString sql;
    if(train == "" && wheel == "")
    {
        model->setTable("WheelModel");
        model->select();
        this->setModel(model);
        return;
    }
    if(wheel != "")
    {
        model->setTable("WheelModel");
        model->setFilter(QString("车轮编号 = '%1' and 检测时间 = '%2'").arg(wheel).arg(timeStr));
        model->removeColumns(0, 3);
        model->removeColumns(12, 2);
    }
    else
    {
        model->setTable("TrainInfo");
        model->setFilter(QString("车号 ='%1' and 检测时间 = '%2'").arg(train).arg(timeStr));
//        if(model->rowCount()>1)
//            model->removeRows(1, model->rowCount()-1);
    }
    model->select();
    this->setModel(model);
}

void TableView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(model->rowCount() > 0 && this->rowAt(event->pos().y())>=0)
    {
        if(wheelNo != "")
            emit wheelDoubleClicked(wheelNo, dateTime);
        else
            emit trainDoubleClicked(trainNo, dateTime);
//        QString RFID = model->data(model->index(currentIndex().row(), 3)).toString();
//        qDebug() << RFID;
//        DlgWheelDtl *dlg = new DlgWheelDtl(db, RFID);
//        dlg->exec();
    }
}
