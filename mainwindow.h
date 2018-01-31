#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QTimer>
#include <QUdpSocket>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QWidgetAction>
#include "qcustomplot.h"
#include "objects/wheels.h"
#include "dbsqlite.h"
#include "dialogs/dlgpolarview.h"
#include "dialogs/dlgdevcommunic.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString userName, bool isManage, QWidget *parent = 0);
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QList<UserParas *> userParasList;       //用于存储用户设置的各种列车的参数

public slots:
    void onAddUserParas(UserParas *userParas);
    void onDeleteUserParas(QString paraGroup);
    void onAddUserTrainParas(QString train, QString datetime, QString paragroup);
    void onDeleteUserTrainParas(QString train, QString datetime);

signals:
    void loginShowed();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QString userName;
    bool isManage;          //是否是管理员
    bool isConnectedToDev;      //是否连接到硬件设备

    QAction *actionDevCommunic;
    QAction *actionMoniteRealTime;
    QAction *actionHistoryDatas;
    QMenu *menuParaSetting;
        QAction *actionUserParaSetting ;
        QAction *actionUserParaStat;
        QAction *actionUserParaList;
    QAction *actionDevList ;
    QAction *actionManageParaSetting;
    QMenu *menuSearch;
        QAction *actionStdWheelDatas;
        QAction *actionLaserCPSDatas;
        QAction *actionOriDatas;
        QAction *actionProcessDatas;
        QAction *actionResultDatas;
    QAction *actionLog;
    QAction *actionManage;
    QAction *actionAbout;

    bool isConnectDev();
    void initWidget();
    void initMenu();


public:
    static Wheels *wheel;
    static Train *train;
/*
    EddyDatas *standardEddyA, *standardEddyB;
    QList<LaserDatas*> lasers;
    LaserDatas *laser;
    QList<Wheels *>wheelList;
    Axle *axle;
    QList<Axle *> axleList;
    DBSqlite *db;

    void searchAll();
    QList<QList<Point>> importLaserFromTxt(QString name);

signals:
    void searchTrain(QString, QString, QDateTime);

protected:


public slots:
    void dealDatas(QList<QList<QVariant>> datas);
    void dealSectionDatas(QList<QList<QVariant>> datas);
    void importEddyFromExcel(QString name);

    void actionEddyDatas();
    void actionCheckDatas();
    void actionPolarChart();
    void actionParaSetting();
    void actionCheckTrain();
    void actionUserManage();
    void actionExportEddy();
    void actionExample();
    void threadInsertEddyDB();
    void actionStandardCPS();
    void actionSetPath();
    void actionParaEddySetting();
    void actionPara7080Setting();
    void actionPara7200Setting();

    void actionUpload();

    void actionStartMonitor();
    void actionStopMonitor();

    void actionExportTo2D();
//3dtrain
    void actionOutputExcel();
//3dwheel
    void actionOutputAWheel();
//设置标准轮数据
    void onSelectStandard(QString name);

//建立文件夹
    QString createDir(Train tmpTrain);

    //设置进度条
    void onProgressChanged(int value);

    void ProcessPendingDatagram();
    // 从数据库和文件列表中获得初始化列车、轴、轮子的槽函数
    void onInitTrain(QString TrainNo, QDateTime datetime);
    void onInitWheel(QString RFID, QDateTime datetime);

private slots:
    void on_btn_Search_clicked();
    void onWidgetEnable(bool flag);
//超时提醒
    void onTimeOut();
//tcp相关槽函数
    void onSocketConnect();
    void onSocketReceived();

private:
    int index_Plot;
    QString publicPath;
    Ui::MainWindow *ui;
    QHostAddress *IPAddress;
    QAction *action_upload;
    DlgPolarView *dlg2D;

    bool isSplice;
    bool isManage;
    QString userName;

    bool plotPressFlag;
    QPoint startQPoint;
    double deta;

    QList<double> datasList;
    QVector<double> x;
    QList<QList<double>> datasTable;

    QAction *JointAction;
    QAction *action_startmonitor;
    QAction *action_stopmonitor;
    QAction *action_to2D;

     QProcess *process;
     //RFID
     QTimer *RFIDTimer;
     quint16 rfidPID;
     QUdpSocket *udpSocket;
     QHostAddress hostIP;
     quint16 clientPort;

     QTcpServer *tcpServer;
     QTcpSocket *tcpSocket;

    void initMenus();
    void initTools();
    void initStatusBar();
    void initSearchTable();
    void initQSS();
    void init3DTrain();
    void init3DWheel();
    void settingIni();

    //测试专用区，正式版需要删除
*/
private slots:
    void on_actionDevCommu_triggered();
    void on_actionHistoryDatas_triggered();
    void on_actionUserParaSetting_triggered();
    void on_actionUserParaStat_triggered();
    void on_actionMoniteRealTime_triggered();
    void on_actionDeviceList_triggered();
    void on_actionManageParaSetting_triggered();
    void on_actionManageSearchStdWheel_triggered();
    void on_actionmanageSearchLaserCPS_triggered();
    void on_actionManageSearchOriDatas_triggered();
    void on_actionManageSearchMidDatas_triggered();
    void on_actionManageSearchResultDatas_triggered();
    void onShowDevCommu();
    void onShowManage();
    void onEnterInManage();
    void onChangeIsConnected(bool isConnected);
};

#endif // MAINWINDOW_H
