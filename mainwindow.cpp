#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogs/dlghistorydatas.h"
#include "dialogs/dlguserparasetting.h"
#include "dialogs/dlgmoniterealtime.h"
#include "dialogs/dlgdevicelist.h"
#include "dialogs/dlgmanageparasetting.h"
#include "dialogs/dlgmanagesearchstdwheel.h"
#include "dialogs/dlgmanagesearchstdwheel.h"
#include "dialogs/dlgmanagesearchoridatas.h"
#include "dialogs/dlgmanagesearchmiddatas.h"
#include "dialogs/dlglasercps.h"
#include "dialogs/dlguserparastat.h"
#include <QMenu>
#include <dialogs/dlgpasswardinput.h>

/*
#include <algorithm>
#include <QAxObject>
#include <QAxWidget>
#include <QDebug>
#include <QPen>
#include <QSettings>
#include <QVector>
#include <QFileDialog>
#include <QWheelEvent>
#include <QIcon>
#include <QMouseEvent>
#include <QSplitter>
#include <qmath.h>
#include <QProcess>
#include <QStandardItemModel>
#include "importthread.h"
#include "exportthread.h"
#include "wheelsinfothread.h"
#include "dialogs/dlgcheckdatas.h"
#include "dbmysql.h"
#include "dialogs/dlgchecktrain.h"
#include "dialogs/dlgselectstandard.h"
#include "dialogs/dlgpathsetting.h"
#include "arithmetic.h"
*/

Wheels* MainWindow::wheel = new Wheels();
Train* MainWindow::train = new Train();
QT_CHARTS_USE_NAMESPACE

using namespace std;

using namespace QtCharts;

MainWindow::MainWindow(QString userName, bool isManage, QWidget *parent) :
    QMainWindow(parent),
    userName(userName),
    isManage(isManage),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(!isManage)
    {
        ui->actionDevList->setVisible(false);
        ui->actionDebugParaSetting->setVisible(false);
        ui->actionDebugSearch->setVisible(false);
        ui->actionLog->setVisible(false);
    }
    else
    {
        ui->actionDevList->setVisible(true);
        ui->actionDebugParaSetting->setVisible(true);
        ui->actionDebugSearch->setVisible(true);
        ui->actionLog->setVisible(true);
    }

    isConnectedToDev = false;

    initMenu();
//    isConnectedToDev = isConnectDev();

    qDebug()<<isConnectedToDev;

    ui->mainToolBar->setVisible(false);

/*
    settingIni();

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::Any, 8887);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(ProcessPendingDatagram()));


    tcpServer = new QTcpServer(this);
    if(!tcpServer->listen(QHostAddress::Any, 8890))
        qDebug() << tcpServer->errorString();
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(onSocketConnect()));

    qDebug() <<"OK";
    initMenus();
    init3DTrain();
    init3DWheel();
    initTools();
    initStatusBar();

//    train = new Train();

    //    wheel = new Wheels();

    this->isManage = isManage;
    this->userName = userName;
    this->setWindowTitle("welcome user "+this->userName);
    if(isManage)
        ui->menu_usermanage->setVisible(true);
    else
        ui->menu_usermanage->setVisible(false);

    plotPressFlag = false;
    isSplice = false;
    deta = 50.0;

    ui->btn_Search->setIcon(QIcon(":/images/search.png"));
//    QStandardItemModel *model = new QStandardItemModel();
//    model->setHorizontalHeaderLabels(QStringList("暂无数据"));
//    ui->searchResultTable->setModel(model);
    connect(this, SIGNAL(searchTrain(QString, QString, QDateTime)), ui->searchResultTable, SLOT(onSearch(QString,QString, QDateTime)));
    connect(ui->searchResultTable, SIGNAL(trainDoubleClicked(QString, QDateTime)), this, SLOT(onInitTrain(QString, QDateTime)));


    QRegExp regx("\-?[0-9]+\.?[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx);
    //布局
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal,this->centralWidget());
    QSplitter *leftSplitter = new QSplitter(Qt::Vertical, mainSplitter);
    QSplitter *rightSplitter = new QSplitter(Qt::Vertical, mainSplitter);

    ui->searchPanel->setLayout(ui->gridLayout);
    ui->searchPanel->setParent(leftSplitter);
    //    ui->searchResultList->setParent(leftSplitter);
    ui->searchResultTable->setParent(leftSplitter);
    leftSplitter->setStretchFactor(1, 1);
//    ui->searchResultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->openGLTrain->setParent(rightSplitter);
    ui->openGLWheel->setParent(rightSplitter);

    mainSplitter->setStretchFactor(1, 1);
    this->setCentralWidget(mainSplitter);

    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateTimeEdit->setDateTime(QDateTime(QDate(2017, 8, 22), QTime(13, 5, 58)));
*/
}

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isConnectedToDev = false;

    initMenu();
    ui->mainToolBar->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//添加用户参数
void MainWindow::onAddUserParas(UserParas *userParas)
{
    userParasList.append(userParas);
    DBSqlite *db = new DBSqlite;
    db->connectDB();
    QString sql;

    if(!db->isExistValue(userParas->paraGroup, "GroupName", "UserParasGroup"))
    {
        sql = QString("Insert into UserParasGroup(GroupName, RadialsType, DMax, DMin,"
                      "DeltaRMax, ShMin, ShMax, SdMin, SdMax, QrMin, QrMax, SdStart, "
                      "InLengthMin, InLengthMax, DeltaD1, DeltaD2, DeltaD3) values('%1',"
                      "'%2', '%3', %4, %5, %6, %7, %8, %9, %10, %11, %12, %13, %14, %15,"
                      "%16, %17)").arg(userParas->paraGroup)
                .arg(userParas->radialsType).arg(userParas->DMax).arg(userParas->DMin).arg(userParas->deltaRMax)
                .arg(userParas->shMin).arg(userParas->shMax).arg(userParas->sdMin).arg(userParas->sdMax)
                .arg(userParas->qrMin).arg(userParas->qrMax).arg(userParas->sdStart).arg(userParas->inLengthMin)
                .arg(userParas->inLengthMax).arg(userParas->deltaD1).arg(userParas->deltaD2).arg(userParas->deltaD3);
        db->runSql(sql);
    }
    db->close();
    delete db;
}
//删除用户参数组
void MainWindow::onDeleteUserParas(QString paraGroup)
{
    for(int i=0; i< userParasList.length();i++)
    {
        if(paraGroup == userParasList.at(i)->paraGroup)
        {
            userParasList.removeAt(i);
            break;
        }
    }

    DBSqlite db;
    db.connectDB();
    QString sql = QString("delete from UserParasGroup where GroupName = '%1'").arg(paraGroup);
    if(db.runSql(sql))
        QMessageBox::about(this, "成功", "删除参数组成功");
    else
        QMessageBox::about(this, "成功", "删除参数组失败");
    db.close();


}
//添加列车参数组到数据库
void MainWindow::onAddUserTrainParas(QString train, QString datetime, QString paragroup)
{
    //插入数据库
    DBSqlite db;
    db.connectDB();
    QString sql = QString("Insert into UserParas(Train, DateTime, ParaGroup) values('%1', '%2', '%3')")
            .arg(train).arg(datetime).arg(paragroup);

    if(db.runSql(sql))
        QMessageBox::about(this, "成功", "保存列车成功");
    else
        QMessageBox::about(this, "成功", "保存列车成功失败");
}
//删除列车参数组到数据库
void MainWindow::onDeleteUserTrainParas(QString train, QString datetime)
{
    DBSqlite db;
    db.connectDB();
    QString sql = QString("delete from UserParas where Train = '%1' and DateTime = '%2'")
            .arg(train).arg(datetime);
    if(db.runSql(sql))
    {
        QMessageBox::about(this, "提示", "删除列车成功");
        return;
    }
    else
    {
        QMessageBox::about(this, "提示", "删除列车失败");
        return;
    }
    db.close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!this->isConnectedToDev)
    {
        emit loginShowed();
        event->accept();
    }
    else
    {
        QMessageBox::warning(this, "提示", "当前通信设备处于连接状态，无法退出，请断开后再退出。");
        event->ignore();
    }
}
//是否连接设备
bool MainWindow::isConnectDev()
{
//    int select = QMessageBox::question(this, "", "是否连接设备？", QMessageBox::Yes, QMessageBox::No);
//    if(select == QMessageBox::Yes)
//        return true;
//    else
//        return false;
}
//初始化部件
void MainWindow::initWidget()
{

}

void MainWindow::initMenu()
{
    actionDevCommunic = ui->menuBar->addAction("设备通讯");
    connect(actionDevCommunic, SIGNAL(triggered(bool)), this, SLOT(on_actionDevCommu_triggered()));

    actionMoniteRealTime = ui->menuBar->addAction("实时监控");
    connect(actionMoniteRealTime, SIGNAL(triggered(bool)), this, SLOT(on_actionMoniteRealTime_triggered()));
    if(!isConnectedToDev)
        actionMoniteRealTime->setEnabled(false);

    actionHistoryDatas = ui->menuBar->addAction("历史数据");
    connect(actionHistoryDatas, SIGNAL(triggered(bool)), this, SLOT(on_actionHistoryDatas_triggered()));

    menuParaSetting = new QMenu("参数设置");
//    menuParaSetting->setIcon(QIcon(":/images/action_paraSetting.png"));
    ui->menuBar->addMenu(menuParaSetting);

        actionUserParaSetting = menuParaSetting->addAction("设置");
        connect(actionUserParaSetting, SIGNAL(triggered(bool)), this, SLOT(on_actionUserParaSetting_triggered()));
        actionUserParaStat = menuParaSetting->addAction("统计");
        connect(actionUserParaStat, SIGNAL(triggered(bool)), this, SLOT(on_actionUserParaStat_triggered()));
//        actionUserParaList = menuParaSetting->addAction("列表");
//    connect()

    actionDevList = ui->menuBar->addAction("设备清单");
    connect(actionDevList, SIGNAL(triggered(bool)), this, SLOT(on_actionDeviceList_triggered()));

    actionManageParaSetting = ui->menuBar->addAction("参数设置");
    connect(actionManageParaSetting, SIGNAL(triggered(bool)), this, SLOT(on_actionManageParaSetting_triggered()));
    menuSearch = new QMenu("搜索");
        actionStdWheelDatas = menuSearch->addAction("标准轮数据");
        actionStdWheelDatas->setFont(QFont("楷体", 11));
        connect(actionStdWheelDatas, SIGNAL(triggered(bool)), this, SLOT(on_actionManageSearchStdWheel_triggered()));

        actionLaserCPSDatas = menuSearch->addAction("激光扫轨数据");
        actionLaserCPSDatas->setFont(QFont("楷体", 11));
        connect(actionLaserCPSDatas, SIGNAL(triggered(bool)), this, SLOT(on_actionmanageSearchLaserCPS_triggered()));

        actionOriDatas = menuSearch->addAction("原始数据");
        connect(actionOriDatas, SIGNAL(triggered(bool)), this, SLOT(on_actionManageSearchOriDatas_triggered()));
        actionOriDatas->setFont(QFont("楷体", 11));

        actionProcessDatas = menuSearch->addAction("中间数据");
        connect(actionProcessDatas, SIGNAL(triggered(bool)), this, SLOT(on_actionManageSearchMidDatas_triggered()));
        actionProcessDatas->setFont(QFont("楷体", 11));

        actionResultDatas = menuSearch->addAction("计算结果");
        connect(actionResultDatas, SIGNAL(triggered(bool)), this, SLOT(on_actionManageSearchResultDatas_triggered()));
        actionResultDatas->setFont(QFont("楷体", 11));
    ui->menuBar->addMenu(menuSearch);
    actionLog = ui->menuBar->addAction("日志");

    QMenu *menuAbout = new QMenu("帮助");
        actionManage = menuAbout->addAction("管理");
        actionManage->setFont(QFont("楷体", 11));
        connect(actionManage, SIGNAL(triggered(bool)), this, SLOT(onEnterInManage()));

        actionAbout = menuAbout->addAction("关于");
        actionAbout->setFont(QFont("楷体", 11));
    ui->menuBar->addMenu(menuAbout);

    actionDevList->setVisible(false);
    actionManageParaSetting->setVisible(false);
    menuSearch->setVisible(false);
    menuSearch->menuAction()->setVisible(false);
    actionLog->setVisible(false);

}

/*
void MainWindow::settingIni()
{
    QString iniPath = qApp->applicationDirPath()+"/hubtesting.ini";
    QFile file(iniPath);
    if(!file.exists())
    {
        file.open(QFile::ReadWrite);
        file.close();
    }
    QSettings settings(iniPath, QSettings::IniFormat);
    //设置公共路径
    QString path = settings.value("Group1/Path").toString();
    if(path == "")
    {
        QMessageBox::about(this, "提示", "初次使用，请设置公共文件夹路径！（此消息仅初次使用弹出）");
        QString tmp = QFileDialog::getExistingDirectory(this, "选择合适的目录", "./");
        if(tmp == "")
        {
            return;
        }
        publicPath = tmp;
        settings.setValue("Group1/Path", tmp);
    }
    else
        publicPath = path;

//    if(publicPath == "")
//       return false;

    //    设置标准轮
    QString standardPath = settings.value("Group1/StandardWheelPath").toString();
    if(standardPath == "")
    {
        QMessageBox::about(this, "提示", "未发现默认标准轮数据，请设置");
        DlgSelectStandard *dlg = new DlgSelectStandard();
        connect(dlg, SIGNAL(standardSelected(QString)), this, SLOT(onSelectStandard(QString)));
        dlg->exec();
    }
    else
    {
        onSelectStandard(standardPath);
    }
}

void MainWindow::initMenus()
{
    connect(ui->action_datas, SIGNAL(triggered(bool)), this, SLOT(actionCheckDatas()));
    connect(ui->action_polar, SIGNAL(triggered(bool)), this, SLOT(actionPolarChart()));
    connect(ui->action_train, SIGNAL(triggered(bool)), this, SLOT(actionCheckTrain()));
    connect(ui->action_eddydatas, SIGNAL(triggered(bool)), this, SLOT(actionEddyDatas()));
    //    connect(ui->menu_usermanage, SIGNAL(triggered(bool)), this, SLOT(actionUserManage()));
    connect(ui->actionexample, SIGNAL(triggered(bool)), this, SLOT(actionExample()));
    connect(ui->action_expEddy, SIGNAL(triggered(bool)), this, SLOT(actionExportEddy()));
    connect(ui->action_StandardCPS, SIGNAL(triggered(bool)), this, SLOT(actionStandardCPS()));
    connect(ui->action_pubPath, SIGNAL(triggered(bool)), this, SLOT(actionSetPath()));
    //参数设置
    connect(ui->action_ParaEddy, SIGNAL(triggered(bool)), this, SLOT(actionParaEddySetting()));
    connect(ui->action_Para7080, SIGNAL(triggered(bool)), this, SLOT(actionPara7080Setting()));
    connect(ui->action_Para7200, SIGNAL(triggered(bool)), this, SLOT(actionPara7200Setting()));
    connect(ui->action_ParaRange, SIGNAL(triggered(bool)), this, SLOT(actionParaSetting()));


}

void MainWindow::initTools()
{
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    action_upload = new QAction(QIcon(":/images/upload.png"), "至大数据");
    action_upload->setFont(QFont("楷体", 12));
    connect(action_upload, SIGNAL(triggered(bool)), this, SLOT(actionUpload()));
    ui->mainToolBar->addAction(action_upload);

}

void MainWindow::initStatusBar()
{
    ui->statusBar->addWidget(ui->progressBar);
    ui->statusBar->addWidget(ui->label);
    ui->progressBar->setValue(0);
    ui->label->setText("");
}

void MainWindow::initSearchTable()
{

}

void MainWindow::initQSS()
{
    QFile file(":/QSS/main.qss");
    file.open(QFile::ReadOnly);
    this->setStyleSheet(file.readAll());
}

void MainWindow::init3DTrain()
{
    action_startmonitor = new QAction(QIcon(":/images/start_monitor.png"), "开始监控");
    connect(action_startmonitor, SIGNAL(triggered(bool)), this, SLOT(actionStartMonitor()));
    action_stopmonitor = new QAction(QIcon(":/images/stop_monitor.png"), "停止监控");
    connect(action_stopmonitor, SIGNAL(triggered(bool)), this, SLOT(actionStopMonitor()));
    action_stopmonitor->setEnabled(false);
    QAction *action_output = new QAction(QIcon(":/images/3Doutput.png"), "输出数据");
    connect(action_output, SIGNAL(triggered(bool)), this, SLOT(actionOutputExcel()));
    ui->openGLTrain->appendAction(action_startmonitor, 0);
    ui->openGLTrain->appendAction(action_stopmonitor, 1);
    ui->openGLTrain->appendAction(action_output);

}

void MainWindow::init3DWheel()
{
    action_to2D = new QAction(QIcon(":/images/Output2D.png"), "输出至2D");
    ui->openGLWheel->appendAction(action_to2D);
    connect(action_to2D, SIGNAL(triggered(bool)), this, SLOT(actionExportTo2D()));
    dlg2D = new DlgPolarView();
    connect(dlg2D, SIGNAL(widgetEnable(bool)), this, SLOT(onWidgetEnable(bool)));
//    action_to2D->setEnabled(false);
    connect(ui->openGLWheel, SIGNAL(PickSectionMsg(QVector<QVector2D>)), dlg2D, SIGNAL(PickSectionMsg(QVector<QVector2D>)));
    connect(ui->openGLWheel, SIGNAL(PickWidth(float)), dlg2D, SIGNAL(PickWidth(float)));
    QAction *action_output = new QAction(QIcon(":/images/3Doutput.png"), "输出数据");
    connect(action_output, SIGNAL(triggered(bool)), this, SLOT(actionOutputAWheel()));
    ui->openGLWheel->appendAction(action_output);


}

void MainWindow::actionExportTo2D()
{
    //    QMessageBox::about(this, "haha", "wwww");

    dlg2D->show();
}

void MainWindow::dealDatas(QList<QList<QVariant> > datas)
{
//    EddyDatas *eddyDatas;
    datasTable.clear();

    for(int i=0, k=0; i<datas.length(); i++)
    {
        QVariantList tmp = datas.at(i);
        bool ok;    int index;
        QString title = tmp.at(0).toString().trimmed();
        index = title.right(title.length()-1).trimmed().toInt(&ok, 10);
        if((ok && (title.at(0) == 'A' || title.at(0) =='B')) && title.length()<4)
        {
            tmp.removeFirst();
            QList<double> tmp2;
            for(int j=0; j<tmp.length(); j++)
            {
                tmp2.append(tmp.at(j).toDouble());
            }
            datasTable.append(tmp2);

            k++;
        }
        else
            continue;
    }

//    eddyDatas = new EddyDatas(datasTable);
//    wheel->eddy = eddyDatas;
//    wheel->RFID = "D9508-001";
//    axle = new Axle(1, 500);
//    axle->addWheel(wheel);
//    train->addAxle(axle);
}

void MainWindow::dealSectionDatas(QList<QList<QVariant> > datas)
{
    for(int i=0; i< datas.length(); i++)
    {
        QVector<QVariant> tmp = datas.at(i).toVector();
        reverse(tmp.begin(), tmp.end());
        datas.replace(i, tmp.toList());
    }


}

void MainWindow::importEddyFromExcel(QString name)
{
    qDebug() << name;
//    name = "W:/CarWheel/LuxrayDamo/build-LuxrayDamo-Desktop_Qt_MinGW_w64_64bit_MSYS2-Debug/debug/标准轮数据/5km标准轮A.csv";

    ui->progressBar->setValue(10);
    QAxWidget excel("Excel.Application");
    excel.setProperty("Visible", false);
    QAxObject *work_books = excel.querySubObject("WorkBooks");

    ui->progressBar->setValue(20);
    work_books->dynamicCall("Open(const QString &)", QString(name));
    //获取标题
    QVariant title = excel.property("Caption");

    QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
    QAxObject *work_sheets = work_book->querySubObject("Sheets");
    //获取工作表的数目
    int sheet_count = work_sheets->property("Count").toInt();

    QAxObject *sheet1 = work_book->querySubObject("Sheets(int)", 1);// 获取第一个表
    //获取表名
    QString sheet_title = sheet1->property("Name").toString();

    QAxObject *used_range = sheet1->querySubObject("UsedRange");
    QAxObject *rows = used_range->querySubObject("Rows");
    QAxObject *columns = used_range->querySubObject("Columns");

    int row_start = used_range->property("Row").toInt();  //获取起始行
    int column_start = used_range->property("Column").toInt();  //获取起始列
    int row_count = rows->property("Count").toInt();  //获取行数
    int column_count = columns->property("Count").toInt();  //获取列数

    ui->progressBar->setValue(50);
    //获取单元格数据
    QVariant var = used_range->dynamicCall("Value");
    delete used_range;

    QVariantList list1 =  var.toList();
    QList<QList<QVariant>> values;
    for(int j=0; j<column_count; j++)
    {
        QVariantList tmp;
        for(int i=0; i<row_count; i++)
        {
            QVariantList list2 = list1.at(i).toList();
            tmp << list2.at(j);
        }
        values.append(tmp);
    }

    ui->progressBar->setValue(70);
    work_books->dynamicCall("Close()");

    excel.dynamicCall("Close()");
    excel.dynamicCall("Quit(void)");

    dealDatas(values);
    ui->progressBar->setValue(90);
    qDebug()<< "Excel操作完了";
    //emit endLoading(row_count, column_count, sheet_title, valueList);
}

void MainWindow::searchAll()
{
    DBSqlite *db = new DBSqlite();
    db->connectDB();
    QSqlQueryModel *model = new QSqlQueryModel();
//    QSqlTableModel *model = new QSqlTableModel(this, db->database);
    db->runSql("select 车号, 计轴, 车轮位置, 车轮编号, 检测时间, 检测人 from WheelModel");
    model->setQuery(*(db->query));
//    model->setTable("WheelModel");
//    model->select();
    ui->searchResultTable->setModel(model);
    db->close();
}

QList<QList<Point> > MainWindow::importLaserFromTxt(QString name)
{
    QFile file(name);
    file.open(QFile::ReadOnly);
    QString text;
    QList<Point> pointList;
    QList<QList<Point>> lasers;
    Point point;
    while(true)
    {
        text = file.readLine();
        if(text == "" || text == "\t")
            break;

        int flag = 0;
        pointList.clear();

        while (true)
        {
            QString tmp = text.left(text.indexOf('\t')+1).trimmed();
            if(tmp == "" || tmp == "\t")
                break;
            text = text.right(text.length() - tmp.length()).trimmed();
            double x_y = tmp.toDouble()/100000.0;
            if(flag%2 == 0)
            {
                if(x_y < -10000.0)
                {
                    tmp = text.left(text.indexOf('\t')+1).trimmed();
                    text = text.right(text.length() - tmp.length()).trimmed();

                }
                else
                {
                    point.setX(x_y);
                }
            }
            else
            {
                if(x_y < -10000.0)
                {
                    point.setX(0);

                }
                else
                {
                    point.setY(x_y);
                    pointList.append(point);
                }
            }
            flag++;
        }
        if(pointList.length() > 0)
                lasers.append(pointList);
    }

    file.close();
    return lasers;
}


//滚轮控制缩放


//==========================菜单相应的槽函数=============================
void MainWindow::actionEddyDatas()
{
    QString name = QFileDialog::getOpenFileName(this, "请选择一个文件", "F:/wheels", "csv文件(*.csv) ;; 所有文件(*.*)");
    if(name == "")
        return;
    //移除当前已有的图层
    index_Plot = 0;
    ImportThread *importThread = new ImportThread(name);
    connect(importThread, SIGNAL(sendDatas(QList<QList<QVariant>>)), this, SLOT(dealDatas(QList<QList<QVariant> >)));
    connect(importThread, SIGNAL(progress(int)), this, SLOT(onProgressBarChanged(int)));
    importThread->start();
    importThread->exit();
}

void MainWindow::actionCheckDatas()
{
    DlgCheckDatas *dlg = new DlgCheckDatas();
    dlg->show();
}

void MainWindow::actionPolarChart()
{
    DlgPolarView *dlg = new DlgPolarView();
    dlg->show();
}

void MainWindow::actionParaSetting()
{
    DlgParaSetting *dlg = new DlgParaSetting();
    dlg->show();
}

void MainWindow::actionCheckTrain()
{
    DlgCheckTrain *dlg = new DlgCheckTrain();
    dlg->show();
}

void MainWindow::actionUserManage()
{

}



void MainWindow::actionExportEddy()
{
    //================测试可删=============================
    QThread *dbThread = new QThread();
    connect(dbThread, SIGNAL(started()), this, SLOT(threadInsertEddyDB()));
    this->moveToThread(dbThread);
    dbThread->start();
    //  dbThread->terminate();
    //=================================================


    //    ExportThread *thread = new ExportThread(1);
    //    thread->start();
    //    thread->exit();
}

void MainWindow::actionExample()
{
    QString name = QFileDialog::getOpenFileName(this, "请选择文件", "F:/wheels", "表格文件(*.xls | *.xlsx | *.csv)");
    if(name == "")
        return;
    WheelsInfoThread *thread = new WheelsInfoThread(name);
    connect(thread, SIGNAL(sendDatas(QList<QList<QVariant> >)), this, SLOT(dealSectionDatas(QList<QList<QVariant> >)));
    thread->start();
    thread->exit();
}
//3dtrain槽函数
void MainWindow::actionOutputExcel()
{
    //QString name = QFileDialog::getSaveFileName(this, "保存文件", "./", "Excel表格文件(*.xls)");
    QString path = createDir(*train);
    QString name = path +"/"+ train->RFID+".xls";
    qDebug() <<path;
    ExportThread *thread = new ExportThread(1, train, path);
    connect(thread, SIGNAL(progressChanged(int)), this, SLOT(onProgressChanged(int)));
    thread->start();
    thread->exit();
}

void MainWindow::actionOutputAWheel()
{

}
//设置标准轮数据
void MainWindow::onSelectStandard(QString name)
{
    qDebug() << "resetting standardwheel";
    importEddyFromExcel(name);
    standardEddyA = new EddyDatas(datasTable);
    ui->progressBar->setValue(100);
    QSettings setting(qApp->applicationDirPath()+"/hubtesting.ini", QSettings::IniFormat);
    setting.setValue("Group1/StandardWheelPath", name);
    //    ImportThread *thread = new ImportThread(name);

    //    connect(thread, SIGNAL(sendDatas(QList<QList<QVariant> >)), this, SLOT(dealDatas(QList<QList<QVariant>>)));
    //    thread->start();

    //    standardEddyA = new EddyDatas(datasTable);
}

void MainWindow::threadInsertEddyDB()
{
    if(wheel->eddy->Data1.length() <= 0)
        return;
    DBSqlite *database = new DBSqlite();
    database->connectDB();
    //开始事务
    database->database.transaction();
    for(int i=0; i< wheel->eddy->Data1.length(); i++)
    {
        QString sql = QString("insert into EddyDatas(Train, Axle, RFID, Data1, Data2, Data3, Data4,"
                              "DataJointed, DateTime) values('001', 1, '001001', %1, %2, %3, %4, %5, "
                              "'%6')").arg(wheel->eddy->Data1.at(i)).arg(wheel->eddy->Data2.at(i)).arg(wheel->eddy->Data3.at(i)).arg(wheel->eddy->Data4.at(i)).arg(wheel->eddy->DataJointed.at(i)).arg(QTime::currentTime().toString());
        //       qDebug() << "执行"<< sql << " "<<database->runSql(sql) << " " << i;
        database->query->exec(sql);
        //       ui->progressBar->setValue(i);
    }
    database->database.commit();
    database->close();
}

void MainWindow::actionUpload()
{
//    QFile file(qApp->applicationDirPath() +"/json.txt");
//    if(train->axleList.length() <=0)
//    {
//        QMessageBox::warning(this, "提示", "暂无数据");
//        return;
//    }
//    file.open(QFile::WriteOnly);
//    for(int i=0; i< train->axleList.length(); i++)
//    {
//        Axle *tmpAxle = train->axleList.at(i);
//        for(int j=0; j< tmpAxle->wheelsList.length(); j++)
//        {
//            Wheels *tmpWheel = tmpAxle->wheelsList.at(j);
//            QString text = "[\n";
//            file.write(text.toLatin1());
//            text = QString("{\"Wheel_placeid\":%1,\"Wheel_num\":\"%2\",\"Train_id\":%3,\"Diameter\":%4,\"Beat\":%5,\"Border_heigth\":%6,"
//                           "\"Border_deep\":%7,\"Qr\":%8,\"Inside\":%9,\"Checker\":\"%10\",\"Checedate\":\"%11\",\"Unit_id\":%12}").arg(tmpWheel->position)
//                    .arg(tmpWheel->RFID).arg(train->RFID).arg(tmpWheel->mean_r*2).arg(122).arg(tmpWheel->laser->Sh)
//                    .arg(QString("%1,%2").arg(tmpWheel->laser->Sd1).arg(tmpWheel->laser->Sd2))
//                    .arg(QString("%1,%2").arg(tmpWheel->laser->Gr1).arg(tmpWheel->laser->Gr2))
//                    .arg(tmpAxle->Length).arg("lige").arg("2017-11-26").arg(111);
//            file.write(text.toLatin1());
//            text = "\n]";
//            file.write(text.toLatin1());
//        }
//    }
//    file.close();

//    QProcess *upload = new QProcess(this);
//    QString path = qApp->applicationDirPath() + "/Data上传.exe";
//    qDebug() << path;
//    upload->start(path);
}

void MainWindow::actionStandardCPS()
{
    DlgSelectStandard *dlg = new DlgSelectStandard;
    connect(dlg, SIGNAL(standardSelected(QString)), this, SLOT(onSelectStandard(QString)));
    dlg->show();
}

void MainWindow::actionSetPath()
{
    DlgPathSetting *dlg = new DlgPathSetting;
    dlg->setPath(publicPath);
    dlg->exec();

    publicPath = dlg->getPath();
    delete dlg;

    //写入初始化文件中
    QString fileIni = qApp->applicationDirPath()+"/hubtesting.ini";
    QSettings settings(fileIni, QSettings::IniFormat);
    settings.setValue("Group1/Path", publicPath);
}

void MainWindow::actionParaEddySetting()
{
    DlgParaEddySetting *dlg = new DlgParaEddySetting();
    dlg->show();
}

void MainWindow::actionPara7080Setting()
{
    DlgPara7080Setting *dlg = new DlgPara7080Setting();
    dlg->show();
}

void MainWindow::actionPara7200Setting()
{
    DlgPara7200Setting *dlg = new DlgPara7200Setting();
    dlg->show();
}

void MainWindow::actionStartMonitor()
{
//    process = new QProcess(this);
//    process->start("./LJV7_DllSampleAll.exe");
//    action_stopmonitor->setEnabled(true);

//    wheel->calcAllDatas();
    process = new QProcess(this);
    QStringList paraList;
    paraList << qApp->applicationDirPath();
    process->startDetached(qApp->applicationDirPath()+"/RFID.exe", paraList,qApp->applicationDirPath(), &rfidPID);
    action_stopmonitor->setEnabled(true);
    action_startmonitor->setEnabled(false);
    //超时条件启动
    RFIDTimer = new QTimer(this);
    connect(RFIDTimer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    RFIDTimer->start(5000);



//    QThread *thread = new QThread;
//    this->moveToThread(thread);
//    connect(thread, SIGNAL(started()), this, SLOT(onReadRFID()));
//    thread->start();
}

void MainWindow::actionStopMonitor()
{
    process->kill();
    RFIDTimer->stop();
//    process->terminate();
//    RFIDTime->stop();
    QByteArray closeMsg = QString("s").toLatin1();
    qDebug() << "ip="<<hostIP.toString() << "port = "<< clientPort;
    udpSocket->writeDatagram(QString("s").toStdString().c_str(), closeMsg.length(), hostIP, clientPort);
    action_stopmonitor->setEnabled(false);
    action_startmonitor->setEnabled(true);
}

void MainWindow::on_btn_Search_clicked()
{    
    emit searchTrain(ui->line_Train->text(), ui->line_Wheel->text(), ui->dateTimeEdit->dateTime());
//    Model->setHorizontalHeaderLabels(headers);
    //    ui->searchResultTable->setModel(Model);
}

void MainWindow::onWidgetEnable(bool flag)
{
    qDebug() << "get enable";
    this->action_to2D->setEnabled(true);
}
//超时提醒
void MainWindow::onTimeOut()
{
    QMessageBox::warning(this, "超时提醒", "获取数据超时，程序将自动停止。");
    RFIDTimer->stop();
    delete RFIDTimer;
    action_stopmonitor->setEnabled(false);
    action_startmonitor->setEnabled(true);
    process->kill();

}

void MainWindow::onSocketConnect()
{
    tcpSocket = tcpServer->nextPendingConnection();
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(onSocketReceived()));
//    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

void MainWindow::onSocketReceived()
{
    QString msg = tcpSocket->readAll();
    qDebug() << msg;
}


QString MainWindow::createDir(Train tmpTrain)
{
    //年
    QString path = QString("%1/%2").arg(publicPath).arg(tmpTrain.datetime.date().year());
    if(!Arithmetic::isDirExist(path))
    {
        QMessageBox::warning(this, "警告", "新建文件夹失败");
    }
    //月
    path = QString("%1/%2").arg(path).arg(tmpTrain.datetime.date().month());
    if(!Arithmetic::isDirExist(path))
    {
        QMessageBox::warning(this, "警告", "新建文件夹失败");
    }
    //日
    path = QString("%1/%2").arg(path).arg(tmpTrain.datetime.date().day());
    if(!Arithmetic::isDirExist(path))
    {
        QMessageBox::warning(this, "警告", "新建文件夹失败");
    }
    //车次
    path = QString("%1/%2_%3").arg(path).arg(tmpTrain.RFID).arg(tmpTrain.datetime.time().hour());
    if(!Arithmetic::isDirExist(path))
    {
        QMessageBox::warning(this, "警告", "新建文件夹失败");
    }
    return path;
}

//*******func********
//设置进度条
//*******paras*********
//
void MainWindow::onProgressChanged(int value)
{
    ui->progressBar->setValue(value);
    if(value == 100)
    {
        QMessageBox::about(this, "提示", "数据已成功导入");
    }
}
//UDP接收RFID数据
void MainWindow::ProcessPendingDatagram()
{
    RFIDTimer->stop();
    QByteArray byteArray;
    byteArray.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(byteArray.data(), byteArray.size(),& hostIP, &clientPort);

    qDebug() <<"port 8888: byteArray.data() = " <<byteArray.data();

//处理udp接收到的数据
    QByteArray flagByteArray = byteArray.left(4);
    byteArray.remove(0, 4);
    QByteArray typeByteArray = byteArray.left(4);
    byteArray.remove(0, 4);
    QByteArray lengthByteArray = byteArray.left(4);
    byteArray.remove(0, 4);

    //RFID
    if(flagByteArray.toInt() == 0)
    {

    }
    //激光数据
    else if(flagByteArray.toInt() == 1)
    {
        //7080
        if(typeByteArray.data() == "7080")
        {

        }
        //7200
        else if(typeByteArray.data() == "7200")
        {

        }

    }

    wheel = new Wheels();
    wheel->RFID = byteArray.data();

    RFIDTimer->start(5000);
}


void MainWindow::onInitTrain(QString TrainNo, QDateTime datetime)
{
    DBSqlite *db = new DBSqlite;
    db->connectDB();
    QString sql = QString("select * from TrainInfo where 车号 ='%1' and 检测时间 = '%2'").arg(TrainNo)
            .arg(datetime.toString("yyyy-MM-dd HH:mm"));
    db->runSql(sql);
    db->query->next();
//获取列车基本数据
    ui->label->setText("加载列车基本信息");
    train->RFID = db->query->value("车号").toString();
    train->marshalling = db->query->value("编组数").toInt();
    train->datetime = db->query->value("检测时间").toDateTime();
    qDebug() <<train->datetime;
    train->checher = db->query->value("检测人").toString();
//获取轮子数据
    sql = QString("select * from WheelModel where 车号 ='%1' order by 计轴 asc").arg(TrainNo);
    db->runSql(sql);
    Axle *axle = nullptr;
    while(db->query->next())
    {
        //计轴定义
        if(axle == nullptr || db->query->value("计轴").toInt()!=axle->No)
        {
            axle = new Axle(db->query->value("计轴").toInt(), db->query->value("内测距").toInt());
            axle->Length = db->query->value("内测距").toDouble();
        }
        Wheels *tmpWheel = new Wheels();
        tmpWheel->RFID = db->query->value("车轮编号").toString();
        tmpWheel->radiuRunOut = db->query->value("径跳").toDouble();
//添加激光数据
        ui->label->setText("加载激光数据信息");
        QString tmp = db->query->value("激光数据").toString();
        QString laser7080Name = tmp.left(tmp.indexOf(';'));
        QString laser7200Name = tmp.right(tmp.length() - laser7080Name.length() -1);
        //7080和7200
        LaserDatas *lasers = new LaserDatas(importLaserFromTxt(laser7080Name),
                                            importLaserFromTxt(laser7200Name));

        lasers->Sh = db->query->value("Sh").toDouble();
        lasers->Sd1 = db->query->value("Sd1").toDouble();
        lasers->Sd2 = db->query->value("Sd2").toDouble();
        lasers->Gr1 = db->query->value("Gr1").toDouble();
        lasers->Gr2 = db->query->value("Gr2").toDouble();
        tmpWheel->lasers = lasers;
        qDebug() << "激光数据加载完毕";
//添加电涡流数据
        ui->label->setText("加载电涡流信息");
        QString eddyName = db->query->value("电涡流数据").toString();
        importEddyFromExcel(eddyName);
        EddyDatas *eddy = new EddyDatas(datasTable);
        tmpWheel->eddy = eddy;
        qDebug() << "电涡流数据加载完毕";

        axle->addWheel(tmpWheel);
        //如果列车上没有该计轴，就把计轴添加到该列车上
        if(!train->isExistAxle(axle))
        {
            train->addAxle(axle);
        }
    }

    db->close();
    delete db;
    ui->label->setText("数据加载完毕");
    ui->progressBar->setValue(100);
    qDebug() << "列车数据加载完毕";
    QString text = QString("7080激光数据的长度为：%1，7200激光数据的个数为：%2，电涡流拼接数据的长度为：%3")
            .arg(train->axleList.at(0)->wheelsList.at(0)->lasers->laser7080.length())
            .arg(train->axleList.at(0)->wheelsList.at(0)->lasers->laser7200.length())
            .arg(train->axleList.at(0)->wheelsList.at(0)->eddy->DataJointed.length());
    qDebug() << text;
}

void MainWindow::onInitWheel(QString RFID, QDateTime datetime)
{
    DBSqlite *db = new DBSqlite;
    db->connectDB();
    QString sql = QString("select * from WheelModel where 车轮编号 ='%1' and 检测时间 = '%2'").arg(RFID)
            .arg(datetime.toString("yyyy-MM-dd HH:mm:ss"));
    db->runSql(sql);

    QString trainNo = db->query->value("车号").toString();
    //初始化当前车轮

    //初始化列车
    onInitTrain(trainNo, datetime);
}

*/
//点击“设备通讯”
void MainWindow::on_actionDevCommu_triggered()
{
    DlgDevCommunic *devCommunicate = new DlgDevCommunic(isConnectedToDev);
    connect(devCommunicate, SIGNAL(isConnectedChanged(bool)), this, SLOT(onChangeIsConnected(bool)));
    this->setCentralWidget(devCommunicate);
}

//点击“历史数据”
void MainWindow::on_actionHistoryDatas_triggered()
{
    DlgHistoryDatas *dlg = new DlgHistoryDatas(this);
    this->setCentralWidget(dlg);
//    dlg->show();
}
//用户的参数设置
void MainWindow::on_actionUserParaSetting_triggered()
{
    DlgUserParaSetting *dlg = new DlgUserParaSetting(this);
    connect(dlg, SIGNAL(userParasAdded(UserParas*)), this, SLOT(onAddUserParas(UserParas*)));
    connect(dlg, SIGNAL(userParasDeleted(QString)), this, SLOT(onDeleteUserParas(QString)));
    connect(dlg, SIGNAL(userTrainAdded(QString,QString,QString)), this, SLOT(onAddUserTrainParas(QString,QString,QString)));
    connect(dlg, SIGNAL(userTrainDeleted(QString,QString)), this, SLOT(onDeleteUserTrainParas(QString,QString)));
    this->setCentralWidget(dlg);
    //    dlg->show();
}
//用户的参数统计
void MainWindow::on_actionUserParaStat_triggered()
{
    DlgUserParaStat *dlg = new DlgUserParaStat(this);
    this->setCentralWidget(dlg);
}
//实时监控
void MainWindow::on_actionMoniteRealTime_triggered()
{
    DlgMoniteRealTime *dlg = new DlgMoniteRealTime(this);
    this->setCentralWidget(dlg);
    //    dlg->show();
}
//设备清单
void MainWindow::on_actionDeviceList_triggered()
{
    DlgDeviceList *dlg = new DlgDeviceList(this);
    this->setCentralWidget(dlg);
}

void MainWindow::on_actionManageParaSetting_triggered()
{
    DlgManageParaSetting *dlg = new DlgManageParaSetting(this);
    this->setCentralWidget(dlg);
}

void MainWindow::on_actionManageSearchStdWheel_triggered()
{
    DlgManageSearchStdWheel *dlg = new DlgManageSearchStdWheel(this);
    this->setCentralWidget(dlg);
}

void MainWindow::on_actionmanageSearchLaserCPS_triggered()
{
    DlgLaserCPS *dlg = new DlgLaserCPS();
    this->setCentralWidget(dlg);
}

void MainWindow::on_actionManageSearchOriDatas_triggered()
{
    DlgManageSearchOriDatas *dlg = new DlgManageSearchOriDatas(this);
    this->setCentralWidget(dlg);
}

void MainWindow::on_actionManageSearchMidDatas_triggered()
{
    DlgManageSearchMidDatas *dlg = new DlgManageSearchMidDatas(this);
    this->setCentralWidget(dlg);
}

void MainWindow::on_actionManageSearchResultDatas_triggered()
{
    DlgHistoryDatas *dlg = new DlgHistoryDatas(this);
    this->setCentralWidget(dlg);
}

void MainWindow::onShowDevCommu()
{
    DlgDevCommunic *devCommunicate = new DlgDevCommunic(this);
    this->setCentralWidget(devCommunicate);
    isConnectedToDev = true;
    actionMoniteRealTime->setEnabled(true);
}
//管理者界面显示
void MainWindow::onShowManage()
{
    actionDevList->setVisible(true);
    actionManageParaSetting->setVisible(true);
//    menuSearch->setVisible(true);
    menuSearch->menuAction()->setVisible(true);
    actionLog->setVisible(true);
    actionManage->setText("登出");
}
//管理者进行登录
void MainWindow::onEnterInManage()
{
    if(actionManage->text() == "管理")
    {
        DlgPasswardInput * pwdInput = new DlgPasswardInput(1);
        connect(pwdInput, SIGNAL(openManage()), this, SLOT(onShowManage()));
        pwdInput->exec();
    }
    else if(actionManage->text() == "登出")
    {
        actionDevList->setVisible(false);
        actionManageParaSetting->setVisible(false);
        menuSearch->setVisible(false);
        menuSearch->menuAction()->setVisible(false);
        actionLog->setVisible(false);
        actionManage->setText("管理");
    }
}

void MainWindow::onChangeIsConnected(bool isConnected)
{
    this->isConnectedToDev = isConnected;
    if(isConnectedToDev)
        actionMoniteRealTime->setEnabled(true);
    else
        actionMoniteRealTime->setEnabled(false);
}
