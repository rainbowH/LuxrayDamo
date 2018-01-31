#include "dlguserparastat.h"
#include "ui_dlguserparastat.h"
#include "dbsqlite.h"
#include <QMessageBox>

DlgUserParaStat::DlgUserParaStat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgUserParaStat)
{
    ui->setupUi(this);
    this->setLayout(ui->mainLayout);

    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);
    ui->gridLayout->setSizeConstraint(QLayout::SetFixedSize);
    initDialog();
    QFile file(":/QSS/dlguserparastat.qss");
    file.open(QFile::ReadOnly);
    this->setStyleSheet(file.readAll());
    file.close();

}

DlgUserParaStat::~DlgUserParaStat()
{
    delete ui;
}

void DlgUserParaStat::initDialog()
{
    DBSqlite *db = new DBSqlite;
    db->connectDB();
    QString sql = "select GroupName from UserParasGroup";
    db->runSql(sql);
    QStringList groupNameList;
    while(db->query->next())
    {
        groupNameList << db->query->value(0).toString();
    }

    for(int i=0; i< groupNameList.length(); i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(QStringList()<<groupNameList.at(i));
        ui->treeWidget->addTopLevelItem(item);

        sql = QString("select Train, DateTime from UserParas where paraGroup = '%1'").arg(groupNameList.at(i));
        db->runSql(sql);
        while(db->query->next())
        {
            QTreeWidgetItem *itemChild = new QTreeWidgetItem(QStringList()<<db->query->value(0).toString()<<db->query->value(1).toString());
            item->addChild(itemChild);
        }
    }

    db->close();
    delete db;
}
//搜索操作
QTreeWidgetItem *DlgUserParaStat::searchItem(QString Train)
{
    for(int i=0; i<ui->treeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *root = ui->treeWidget->topLevelItem(i);
        for(int j=0; j<root->childCount(); j++)
        {
            if(root->child(j)->text(0).trimmed() == Train.trimmed())
                return root->child(j);
        }
    }
    return nullptr;
}
//从数据库获取数据并！设置相应的控件
void DlgUserParaStat::getInfoDB(QString Train)
{
    QString paraGroup;
    DBSqlite *db = new DBSqlite;
    db->connectDB();
    //获取该列车的参数组名称
    QString sql = QString("select ParaGroup from UserParas where Train = '%1'")
            .arg(Train);
    db->runSql(sql);
    db->query->next();
    //获取参数组的参数值
    paraGroup = db->query->value(0).toString();
    QStringList paraList = db->getAllValues("UserParasGroup", "GroupName", paraGroup, 17);
    //对控件设置值
    ui->txtTrain->setText(Train);
    ui->cbxRadials->setText(paraList.at(1));
    ui->txtDMax->setText(paraList.at(2));
    ui->txtDMin->setText(paraList.at(3));
    ui->txtRJump->setText(paraList.at(4));
    ui->txtShMin->setText(paraList.at(5));
    ui->txtShMax->setText(paraList.at(6));
    ui->txtSdMin->setText(paraList.at(7));
    ui->txtSdMax->setText(paraList.at(8));
    ui->txtQrMin->setText(paraList.at(9));
    ui->txtQrMax->setText(paraList.at(10));
    ui->cbxSdStart->setText(QString("%1mm").arg(paraList.at(11)));
    ui->txtInLengthMin->setText(paraList.at(12));
    ui->txtInLengthMax->setText(paraList.at(13));
    ui->txtD1->setText(paraList.at(14));
    ui->txtD2->setText(paraList.at(15));
    ui->txtD3->setText(paraList.at(16));

    db->close();
    delete db;
}



//进行删除
void DlgUserParaStat::on_btnCancel_clicked()
{
    //删除列车信息
    if((ui->treeWidget->currentItem()->childCount() <=0)&&(ui->treeWidget->currentItem()->parent())
            &&(QMessageBox::Ok == QMessageBox::question(this, "删除", "是否删除当前列车项？",
                                                        QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Cancel)))
    {
        QString train, dateTime;
        train = ui->treeWidget->currentItem()->text(0);
        dateTime = ui->treeWidget->currentItem()->text(1);
        DBSqlite db;
        db.connectDB();
        //清空文本框
        clearAllTxt();
        //删除树节点
        QTreeWidgetItem *item = ui->treeWidget->currentItem();
        delete item;
        //删除数据库记录
        QString sql = QString("delete from UserParas where Train = '%1' and DateTime = '%2'")
                .arg(train).arg(dateTime);
        db.runSql(sql);
        db.close();
    }
    //删除无列车参数组信息
    else if((ui->treeWidget->currentItem()->childCount() <=0) && (!ui->treeWidget->currentItem()->parent())
            && (QMessageBox::Ok == QMessageBox::question(this, "删除", "是否删除当前参数组数据？",
                                                         QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Cancel)))
    {
        QString groupName = ui->treeWidget->currentItem()->text(0);
        DBSqlite db;
        db.connectDB();
        //删除树节点
        QTreeWidgetItem *item = ui->treeWidget->currentItem();
        delete item;
        //删除数据库记录
        QString sql = QString("delete from UserParasGroup where GroupName = '%1'").arg(groupName);
        db.runSql(sql);
        db.close();
    }
    //删除有列车参数组信息
    else if(ui->treeWidget->currentItem()->childCount() > 0)
    {
        QMessageBox::warning(this, "提示！", "有列车使用该参数组参数设置，请将相关列车删除后再删除该参数组。");
        return;
    }
}

void DlgUserParaStat::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    DBSqlite *db = new DBSqlite;
    QString paraGroup;
    db->connectDB();

    //点击到列车
    if(item->childCount() <=0 && item->parent() != 0)
    {
        //获取该列车的参数组名称
        QString sql = QString("select ParaGroup from UserParas where Train = '%1' and DateTime = '%2'")
                .arg(item->text(0).trimmed()).arg(item->text(1).trimmed());
        db->runSql(sql);
        db->query->next();
        ui->txtTrain->setText(item->text(0));
        //获取参数组的参数值
        paraGroup = db->query->value(0).toString();

    }
    //点击到参数组
    else if(item->parent() == 0)
    {
        paraGroup = item->text(0);
        ui->txtTrain->clear();
    }

    QStringList paraList = db->getAllValues("UserParasGroup", "GroupName", paraGroup, 17);
    //对控件设置值
    ui->cbxRadials->setText(paraList.at(1));
    ui->txtDMax->setText(paraList.at(2));
    ui->txtDMin->setText(paraList.at(3));
    ui->txtRJump->setText(paraList.at(4));
    ui->txtShMin->setText(paraList.at(5));
    ui->txtShMax->setText(paraList.at(6));
    ui->txtSdMin->setText(paraList.at(7));
    ui->txtSdMax->setText(paraList.at(8));
    ui->txtQrMin->setText(paraList.at(9));
    ui->txtQrMax->setText(paraList.at(10));
    ui->cbxSdStart->setText(QString("%1mm").arg(paraList.at(11)));
    ui->txtInLengthMin->setText(paraList.at(12));
    ui->txtInLengthMax->setText(paraList.at(13));
    ui->txtD1->setText(paraList.at(14));
    ui->txtD2->setText(paraList.at(15));
    ui->txtD3->setText(paraList.at(16));

    db->close();
    delete db;

}

void DlgUserParaStat::clearAllTxt()
{
    for(int i=0; i<this->children().length(); i++)
    {
        QString objectname = this->children().at(i)->objectName();
        if(objectname.contains("txt"))
        {
            QLineEdit *edit = qobject_cast<QLineEdit*>(this->children().at(i));
            edit->clear();
        }
        else if(objectname.contains("cbx"))
        {
            QComboBox *combobox = qobject_cast<QComboBox*>(this->children().at(i));
            combobox->setCurrentText("");
        }
    }
}
//搜索
void DlgUserParaStat::on_btnSearch_clicked()
{
    QTreeWidgetItem *item;
    //将之前选中的项的选中状态给取消掉
    if(ui->txtTrain->text().trimmed() != "")
    {
        item = searchItem(ui->txtTrain->text().trimmed());
        if(item != nullptr)
            item->setSelected(false);
    }
    //开始搜索新的列车
    if(ui->txtSearchTrain->text() == "")
    {
        QMessageBox::warning(this, "提示", "列车号不能为空！");
        return;
    }
    item = searchItem(ui->txtSearchTrain->text().trimmed());
    if(item == nullptr)
        return;
    qDebug() << "item is not nullptr";
    ui->treeWidget->expandItem(item->parent());
    qDebug() <<"parent has expanded";
    if(ui->treeWidget->currentItem() !=0)
        ui->treeWidget->currentItem()->setSelected(false);
    item->setSelected(true);

    getInfoDB(ui->txtSearchTrain->text().trimmed());
}
