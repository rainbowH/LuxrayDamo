#include "mainwindow.h"
#include <qglobal.h>
#include <qsurfaceformat.h>
#ifndef  QT_NO_OPENGL

#include <QApplication>
#include <QVector>
#include <QDebug>
#include "arithmetic.h"
#include "dialogs/dlglogin.h"
#include "3DWheelPainter/qlocalmainwindow.h"
#include "3DWheelPainter/qlocalspreadmainwindow.hpp"
#include <QSplashScreen>
#include <QPixmap>
#include "dialogs/dlgpolarcoord.h"
#include "dialogs/dlgwelcome.h"
#include "dialogs/dlgdevicelist.h"
#include "dialogs/dlgplot.h"
#include "dialogs/dlgwheelshow.h"
#include "objects/wheels.h"
#include "SelfWidget/progressbarui.h"
#include <QTextCodec>
#include "SelfWidget/wheelicon.h"
#endif // ! QT_NO_OPENGL


//void initOpenglVersion()
//{
//    QSurfaceFormat format;
//    format.setDepthBufferSize(24);
//    format.setStencilBufferSize(8);
//    format.setVersion(3, 3);
//    format.setProfile(QSurfaceFormat::CoreProfile);
//    QSurfaceFormat::setDefaultFormat(format);
//}



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int N = 10000;

//    QPixmap pixmap(":/images/startshow.png");
//    QSplashScreen splash(pixmap);
//    splash.show();
//    a.processEvents();
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    //测试数据
//    QList<double> jointedDatas, lastr;
//    for(int i=0; i< 100; i++)
//        jointedDatas.append((double)i);
//    qDebug() <<"meanR: " <<Arithmetic::calcMeanRadius(jointedDatas, &lastr,0, 2, 1, 4);
//    for(int i=0; i<lastr.length(); i++)
//        qDebug() <<lastr.at(i);

    QFile file(":/QSS/main.qss");
    file.open(QFile::ReadOnly);
    qApp->setStyleSheet(file.readAll());
    file.close();

//DlgPolarCoord *polar = new DlgPolarCoord;
//polar->show();
    DlgWelcome welcome;
    welcome.show();

//    DlgWheelShow *wheel = new DlgWheelShow;
//    wheel->showMaximized();

//    splash.finish(&w);
//    DlgLogin dlg;
//    dlg.show();

    return a.exec();
}
