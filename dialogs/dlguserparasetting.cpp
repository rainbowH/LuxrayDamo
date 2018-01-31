#include "dlguserparasetting.h"
#include "ui_dlguserparasetting.h"
#include <QMessageBox>
#include <QDateTime>
#include "dbsqlite.h"

DlgUserParaSetting::DlgUserParaSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgUserParaSetting)
{
    ui->setupUi(this);
    qRegisterMetaType<UserParas*>("UserParas *");
    this->setLayout(ui->mainLayout);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);
    ui->gridLayout->setSizeConstraint(QLayout::SetFixedSize);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    initWidget();
    initTrain();

    QFile file(":/QSS/dlguserparasetting.qss");
    file.open(QFile::ReadOnly);
    this->setStyleSheet(file.readAll());
    file.close();
}

DlgUserParaSetting::~DlgUserParaSetting()
{
    delete ui;
}

bool DlgUserParaSetting::eventFilter(QObject *obj, QEvent *event)
{
    QLineEdit *tmpLineEdit = qobject_cast<QLineEdit *>(obj);
    if(tmpLineEdit != 0)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(!tmpLineEdit->text().contains('.'))
            {
                tmpLineEdit->setText(QString("%1.00").arg(tmpLineEdit->text()));
            }
            else if(tmpLineEdit->text().length() - tmpLineEdit->text().indexOf('.')-1<2)
            {
                tmpLineEdit->setText(tmpLineEdit->text()+"0");
            }
        }
    }
    if(!isNewParaGroup)
    {
        if(tmpLineEdit != 0 && event->type() == QEvent::FocusIn)
        {
            tmpString = tmpLineEdit->text();
            qDebug() <<"is QLineEdit";
        }
        if(tmpLineEdit != 0 && event->type() == QEvent::FocusOut)
        {
            if(tmpLineEdit->text() != tmpString)
            {
                isNewParaGroup = true;
                DBSqlite *db = new DBSqlite;
                db->connectDB();
                int count = db->getTableRowCount("UserParasGroup");
                ui->txtParaGroup->setCurrentText(QString("参数组%1").arg(count+1));
                db->close();
                delete db;
            }
        }
    }
}
//初始化控件
void DlgUserParaSetting::initWidget()
{
    QRegExp regx("[0-9]{0,4}\\.{1}[0-9]{2}$");
    QValidator *validator = new QRegExpValidator(regx);
    ui->txtD1->setValidator(validator);
    ui->txtD1->installEventFilter(this);

    ui->txtD2->setValidator(validator);
    ui->txtD2->installEventFilter(this);

    ui->txtD3->setValidator(validator);
    ui->txtD3->installEventFilter(this);

    ui->txtDJump->setValidator(validator);
    ui->txtDJump->installEventFilter(this);

    ui->txtDMax->setValidator(validator);
    ui->txtDMax->installEventFilter(this);

    ui->txtDMin->setValidator(validator);
    ui->txtDMin->installEventFilter(this);

    ui->txtInLengthMax->setValidator(validator);
    ui->txtInLengthMax->installEventFilter(this);

    ui->txtInLengthMin->setValidator(validator);
    ui->txtInLengthMin->installEventFilter(this);

    ui->txtQrMax->setValidator(validator);
    ui->txtQrMax->installEventFilter(this);

    ui->txtQrMin->setValidator(validator);
    ui->txtQrMin->installEventFilter(this);

    ui->txtSdMax->setValidator(validator);
    ui->txtSdMax->installEventFilter(this);

    ui->txtSdMin->setValidator(validator);
    ui->txtSdMin->installEventFilter(this);

    ui->txtShMax->setValidator(validator);
    ui->txtShMax->installEventFilter(this);

    ui->txtShMin->setValidator(validator);
    ui->txtShMin->installEventFilter(this);

    initParaGroup();
}
//初始化参数组控件
void DlgUserParaSetting::initParaGroup()
{
    DBSqlite *db = new DBSqlite;
    db->connectDB();
    db->runSql("select GroupName from UserParasGroup");
    int flag = 0;
    QStringList paraGroupList;
    while(db->query->next())
    {
        paraGroupList<<db->query->value(0).toString();
        flag ++;
    }
    db->close();
    delete db;

    ui->txtParaGroup->addItems(paraGroupList);

    QList<QListWidgetItem *> listItemList;
    for(int i=0; i< paraGroupList.length(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem(QIcon(":/images/paragroup.png"), paraGroupList.at(i), ui->listWidget);
        listItemList << item;
    }

    if(flag >0)
    {
        ui->txtParaGroup->setCurrentIndex(0);
        for(int i=0; i< listItemList.length(); i++)
            ui->listWidget->addItem(listItemList.at(i));
        ui->cbbParaGroup->addItems(paraGroupList);
    }

    isNewParaGroup = false;
}
//初始化所有列车
void DlgUserParaSetting::initTrain()
{
    DBSqlite db;
    db.connectDB();
    QString sql = "select * from UserParas";
    db.runSql(sql);

    while(db.query->next())
    {
        ui->tableWidget->insertRow(0);
        ui->tableWidget->setItem(0, 0, new QTableWidgetItem(db.query->value(0).toString()));
        ui->tableWidget->setItem(0, 1, new QTableWidgetItem(db.query->value(1).toString()));
        ui->tableWidget->setItem(0, 2, new QTableWidgetItem(db.query->value(2).toString()));

        trainList << db.query->value(0).toString();
    }

    db.close();
}
//根据参数组初始化其它数据
void DlgUserParaSetting::initParasWidget()
{
    DBSqlite *db = new DBSqlite;
    db->connectDB();
    db->runSql("select * from UserParasGroup");
    while(db->query->next())
    {
        //找到相符合的参数组数据
        if(db->query->value(0).toString() == ui->txtParaGroup->currentText())
        {
            ui->cbxRadials->setCurrentText(db->query->value(1).toString());
            ui->txtDMax->setText(QString::number(db->query->value(2).toDouble(), 'f', 2));
            ui->txtDMin->setText(QString::number(db->query->value(3).toDouble(), 'f', 2));
            ui->txtDJump->setText(QString::number(db->query->value(4).toDouble(), 'f', 2));
            ui->txtShMin->setText(QString::number(db->query->value(5).toDouble(), 'f', 2));
            ui->txtShMax->setText(QString::number(db->query->value(6).toDouble(), 'f', 2));
            ui->txtSdMin->setText(QString::number(db->query->value(7).toDouble(), 'f', 2));
            ui->txtSdMax->setText(QString::number(db->query->value(8).toDouble(), 'f', 2));
            ui->txtQrMin->setText(QString::number(db->query->value(9).toDouble(), 'f', 2));
            ui->txtQrMax->setText(QString::number(db->query->value(10).toDouble(), 'f', 2));
            ui->cbxSdStart->setCurrentText(QString("%1mm").arg(db->query->value(11).toString()));
            ui->txtInLengthMin->setText(QString::number(db->query->value(12).toDouble(), 'f', 2));
            ui->txtInLengthMax->setText(QString::number(db->query->value(13).toDouble(), 'f', 2));
            ui->txtD1->setText(QString::number(db->query->value(14).toDouble(), 'f', 2));
            ui->txtD2->setText(QString::number(db->query->value(15).toDouble(), 'f', 2));
            ui->txtD3->setText(QString::number(db->query->value(16).toDouble(), 'f', 2));
        }
    }
    db->close();
    delete db;
}

void DlgUserParaSetting::initParasWidgetFromDB(QSqlQuery *query)
{
    ui->txtParaGroup->setCurrentText(query->value(0).toString());
    ui->cbxRadials->setCurrentText(query->value(1).toString());
    ui->txtDMax->setText(QString::number(query->value(2).toDouble(), 'f', 2));
    ui->txtDMin->setText(QString::number(query->value(3).toDouble(), 'f', 2));
    ui->txtDJump->setText(QString::number(query->value(4).toDouble(), 'f', 2));
    ui->txtShMin->setText(QString::number(query->value(5).toDouble(), 'f', 2));
    ui->txtShMax->setText(QString::number(query->value(6).toDouble(), 'f', 2));
    ui->txtSdMin->setText(QString::number(query->value(7).toDouble(), 'f', 2));
    ui->txtSdMax->setText(QString::number(query->value(8).toDouble(), 'f', 2));
    ui->txtQrMin->setText(QString::number(query->value(9).toDouble(), 'f', 2));
    ui->txtQrMax->setText(QString::number(query->value(10).toDouble(), 'f', 2));
    ui->cbxSdStart->setCurrentText(QString("%1mm").arg(query->value(11).toString()));
    ui->txtInLengthMin->setText(QString::number(query->value(12).toDouble(), 'f', 2));
    ui->txtInLengthMax->setText(QString::number(query->value(13).toDouble(), 'f', 2));
    ui->txtD1->setText(QString::number(query->value(14).toDouble(), 'f', 2));
    ui->txtD2->setText(QString::number(query->value(15).toDouble(), 'f', 2));
    ui->txtD3->setText(QString::number(query->value(16).toDouble(), 'f', 2));
}

void DlgUserParaSetting::addTwoDecimal(QLineEdit *lineEdit)
{
    if(!lineEdit->text().contains('.'))
    {
        lineEdit->setText(QString("%1.00").arg(lineEdit->text()));
    }
    else if(lineEdit->text().length() - lineEdit->text().indexOf('.')-1<2)
    {
        lineEdit->setText(lineEdit->text()+"0");
    }
}

//确认录入 参数组
void DlgUserParaSetting::on_btnOK_clicked()
{
    if(ui->txtParaGroup->currentText().trimmed() == "")
    {
        QMessageBox::warning(this, "警告", "参数组不可缺失");
        return;
    }

    if(ui->txtShMax->text().toDouble()< ui->txtShMin->text().toDouble())
    {
        QMessageBox::warning(this, "警告", "参数Sh的最小范围不能高于其最大范围，请核实并纠正。");
        return;
    }
    if(ui->txtSdMax->text().toDouble() < ui->txtSdMin->text().toDouble())
    {
        QMessageBox::warning(this, "警告", "参数Sd的最小范围不能高于其最大范围，请核实并纠正。");
        return;
    }
    if(ui->txtQrMax->text().toDouble() < ui->txtQrMin->text().toDouble())
    {
        QMessageBox::warning(this, "警告", "参数Qr的最小范围不能高于其最大范围，请核实并纠正。");
        return;
    }
    if(ui->txtDMax->text().toDouble() < ui->txtDMin->text().toDouble())
    {
        QMessageBox::warning(this, "警告", "参数轮径的最小范围不能高于其最大范围，请核实并纠正。");
        return;
    }


    UserParas *userParas =  new UserParas(ui->txtParaGroup->currentText().trimmed());
    userParas->deltaD1 = ui->txtD1->text().toDouble();
    userParas->deltaD2 = ui->txtD2->text().toDouble();
    userParas->deltaD3 = ui->txtD3->text().toDouble();
    userParas->deltaRMax = ui->txtDJump->text().toDouble();
    userParas->DMax = ui->txtDMax->text().toDouble();
    userParas->DMin = ui->txtDMin->text().toDouble();
    userParas->inLengthMax = ui->txtInLengthMax->text().toDouble();
    userParas->inLengthMin = ui->txtInLengthMin->text().toDouble();
    userParas->qrMax = ui->txtQrMax->text().toDouble();
    userParas->qrMin = ui->txtQrMin->text().toDouble();
    userParas->radialsType = ui->cbxRadials->currentText();
    userParas->sdMax = ui->txtSdMax->text().toDouble();
    userParas->sdMin = ui->txtSdMin->text().toDouble();
    userParas->sdStart = ui->cbxSdStart->currentText().trimmed().left(ui->cbxSdStart->currentText().length()-2).toInt();
    userParas->shMax = ui->txtShMax->text().toDouble();
    userParas->shMin = ui->txtShMin->text().toDouble();
    userParas->dateTime = QDateTime::currentDateTime();
    emit userParasAdded(userParas);

    isNewParaGroup =false;
    if(ui->txtParaGroup->findText(ui->txtParaGroup->currentText()) < 0)
    {
        ui->txtParaGroup->addItem(ui->txtParaGroup->currentText());
        ui->cbbParaGroup->addItem(ui->txtParaGroup->currentText());
        ui->listWidget->addItem(ui->txtParaGroup->currentText());
    }
}
//删除参数组
void DlgUserParaSetting::on_btnDtl_clicked()
{
    if(ui->listWidget->currentRow() < 0)
    {
        QMessageBox::warning(this, "警告", "请在右边列表中选择要删除的参数组！");
        return;
    }
    emit userParasDeleted(ui->listWidget->currentItem()->text());
    qDebug() << "删除列表";
    QListWidgetItem *item = ui->listWidget->currentItem();
    ui->listWidget->removeItemWidget(item);
    delete item;
//    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
    isNewParaGroup = false;
}
//参数组发生变化
void DlgUserParaSetting::on_txtParaGroup_currentTextChanged(const QString &arg1)
{
    bool flag = false;
    for(int i=0; i< ui->txtParaGroup->count(); i++)
    {
        if(arg1 == ui->txtParaGroup->itemText(i))
        {
            flag = true;
            break;
        }
    }
    if(flag)
    {
        initParasWidget();
        isNewParaGroup = false;
    }
    else
        isNewParaGroup = true;
}
//确认录入列车
void DlgUserParaSetting::on_btnOK_2_clicked()
{
    if(ui->txtTrain->text().trimmed() == "")
    {
        QMessageBox::warning(this, "提示", "列车号不可为空");
        return;
    }

    if(ui->cbbParaGroup->currentText().trimmed() == "")
    {
        QMessageBox::warning(this, "提示", "请为当前列车选择一个参数组");
        return;
    }

    if(trainList.contains(ui->txtTrain->text()))
    {
        QMessageBox::warning(this, "提示", "已存在该列车的参数信息，请删除后重新录入");
        return;
    }
    //加入表格
    ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(ui->txtTrain->text()));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm")));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem(ui->cbbParaGroup->currentText()));

    trainList << ui->txtTrain->text();

    emit userTrainAdded(ui->txtTrain->text().trimmed(), QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"),
                        ui->cbbParaGroup->currentText());
}
//删除列车
void DlgUserParaSetting::on_btnDtl_2_clicked()
{
    if(ui->tableWidget->currentRow() < 0)
    {
        QMessageBox::warning(this, "提示", "请选择右侧表中的一行列车");
        return;
    }

    emit userTrainDeleted(ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text(),
                          ui->tableWidget->item(ui->tableWidget->currentRow(), 1)->text());

    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}
//点击了当前的列表节点
void DlgUserParaSetting::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString paraName = item->text();
    DBSqlite db;
    db.connectDB();
    QString sql = QString("select * from UserParasGroup where GroupName = '%1'").arg(paraName);
    db.runSql(sql);
    db.query->next();
    initParasWidgetFromDB(db.query);
    db.close();
}
//单击了当前列车表格
void DlgUserParaSetting::on_tableWidget_cellClicked(int row, int column)
{
    ui->txtTrain->setText(ui->tableWidget->item(row, 0)->text());
    ui->cbbParaGroup->setCurrentText(ui->tableWidget->item(row, 2)->text());
}
