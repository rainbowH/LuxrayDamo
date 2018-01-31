#ifndef DLGPLOT_H
#define DLGPLOT_H

#include <QMainWindow>
#include <QList>
#include <QtGui>
#include <QtCore>
#include <QPainter>
#include <QToolBar>
#include "qcustomplot.h"
#include "objects/point.h"
#include "SectionPlot/coordpoints.h"
#include "SectionPlot/diagram.h"
#include "SectionPlot/dlglathe.h"
namespace Ui {
class DlgPlot;
}

class DlgPlot : public QMainWindow
{
    Q_OBJECT

public:
    explicit DlgPlot(QWidget *parent = 0);
    ~DlgPlot();
    void drawCoordPoint(double x,double y);
    void initParam();
//    void judgePointOnCurve(double x,double y);
    void drawLines(QList<double> coordXArr,QList<double> coordYArr,int index);
    void moveMouse(double xpos,double ypos);
    void moveLines(double coordX,double coordY);
    void addTextToVerLine(double coordX,double coordY);
    void removeAllGraph();
    int getNearestValueIndex(double value);

    QVector<QVector2D> datas;
signals:
    void xRangeChanged(QCPRange);
    void yRangeChanged(QCPRange);
    void onDrawPlot(QVector<QVector2D> datas);
    void closed();
public slots:
    void drawPlot(QList<QList<QVariant>> datas);
    void xChangeRange(double xMin, double xMax);
    void yChangeRange(double yMin, double yMax);
    void onRecoveryDialog();//用于关闭放大窗口后恢复原始嵌入窗口的一些属性

    void drawPlot(QVector<QVector2D> datas);
    int getYMaxIndex();

protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::DlgPlot *ui;
    //x,y点参数
    QList<double> xValues;
    QList<double> yValues;
    //画刷,画笔
    QBrush backRole;
    QPen pen;
    //获得坐标点
    QList<double> coordX;
    QList<double> coordY;
    //绘点
    QCPScatterStyle myScatter;
    //文本
    QCPItemText *phaseTracerText;
    //保存点坐标
    QList<double> coordXArr;
    QList<double> coordYArr;

    //添加工具栏
    QAction *m_zoomIn;
    QAction *m_ZoomOut;

    //X，Y轴最大最小
    double xMin;
    double yMin;
    double xMax;
    double yMax;
    int cur_index;    //当前索引点
    bool isMoveLine;  //设置移动的直线
    Point lastPoint;
    double disPoint;  //点距离的判断
    double disLine;   //线距离点的判断

    Diagram *diagram_Sh, *diagram_Sd;
    double offValue;      //70的位置
    double yOffValue;     //y偏移10或12
    double textOffset;    //文本位置
    bool isCopyDialog;     //窗体的弹出
    bool isSelectPoint;     //判断是否选中点
    double minradius;       //距离判断
    double SdLineOut;       //sd线两端的突出
    Point oldPoint;
    double horShift;        //水平偏移
    bool isHide;            //是否隐藏
    double mRangeZoomFactor;    //放大缩小倍数
    int scaleNum;           //放大缩小次数
    double xCoordOffset;    //x轴扩张


    void drawCoodinate();
    void initDiagram();
    void drawDiagramSh();
    void drawDiagramSd();
    void drawDiagramShLinesAndText();
    void drawDiagramSdLinesAndText();
    int getFirstYOffset10or12cm(double value);
    int getSecondYOffset10or12cm(double value);
    void addPointOnPlot(Diagram *diagram);
    void hideMeasureLines();
    void showMeasureLines();
    bool judgeIsClickPoint(Diagram *diagram,double x,double y);
    void drawMovingDiagramSd(double y);
    void drawMovingDiagram(Diagram *diagram,double x,double y);
    int judgeMouseMoveOnCurve(double x,double y);
protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_ZoomIn_triggered();
    void on_ZoomOut_triggered();
    void on_actionmodify_triggered();
    void on_actionhide_triggered();
    void on_actionsaveDiagram_triggered();
    void on_actioneconomyplan_triggered();
    void on_actionplanline_triggered();
};

#endif // DLGPLOT_H
