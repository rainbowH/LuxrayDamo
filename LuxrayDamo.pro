#-------------------------------------------------
#
# Project created by QtCreator 2017-12-06T10:22:36
#
#-------------------------------------------------

QT       += core gui network charts sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = LuxrayDamo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += qaxcontainer

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    arithmetic.cpp \
    wheelsdatathread.cpp \
    wheelsinfothread.cpp \
    dialogs/dlgcheckdatas.cpp \
    dialogs/dlgchecktrain.cpp \
    dialogs/dlglogin.cpp \
    dialogs/dlgpathsetting.cpp \
    dialogs/dlgplot.cpp \
    dialogs/dlgpolarview.cpp \
    dialogs/dlgselectstandard.cpp \
    dialogs/dlgwheeldtl.cpp \
    dialogs/dlgdevcommunic.cpp \
    objects/point.cpp \
    objects/wheels.cpp \
    dbmysql.cpp \
    dbsqlite.cpp \
    exportthread.cpp \
    importthread.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    tableview.cpp \
    tcpclientsocket.cpp \
    tcpserver.cpp \
    SelfWidget/toolbutton.cpp \
    SelfWidget/iconwidget.cpp \
    dialogs/dlgusersearch.cpp \
    dialogs/dlgmoniterealtime.cpp \
    dialogs/dlghistorydatas.cpp \
    dialogs/dlgtrainsearchshow.cpp \
    dialogs/dlgtraintable.cpp \
    dialogs/dlguserparasetting.cpp \
    dialogs/dlgwheelshow.cpp \
    SelfWidget/progressbarui.cpp \
    dialogs/dlgwelcome.cpp \
    dialogs/dlgpasswardinput.cpp \
    dialogs/dlgdevicelist.cpp \
    dialogs/dlgmanageparasetting.cpp \
    dialogs/dlgmanagesearchstdwheel.cpp \
    dialogs/dlgmanagesearchoridatas.cpp \
    dialogs/dlgmanagesearchmiddatas.cpp \
    dialogs/dlgmideddydatas.cpp \
    dialogs/dlgmidedgeparadatas.cpp \
    dialogs/dlguserparastat.cpp \
    dialogs/dlglasercps.cpp \
    SelfWidget/slider.cpp \
    dialogs/dlgpalorparasetting.cpp \
    SelfWidget/chart.cpp \
    objects/log.cpp \
    dialogs/dlgdamageanalyze.cpp \
    SelfWidget/tablewidget.cpp \
    SelfWidget/wheelicon.cpp \
    SelfWidget/DateEdit/DefDateTimeEdit.cpp \
    SelfWidget/DateEdit/DefineCalendar.cpp \
    3DWheelPainter/CartesianCoordinate.cpp \
    3DWheelPainter/dlgtrain3d.cpp \
    3DWheelPainter/Font.cpp \
    3DWheelPainter/Line.cpp \
    3DWheelPainter/LoadModel.cpp \
    3DWheelPainter/LocalSpreadEngine.cpp \
    3DWheelPainter/Mesh.cpp \
    3DWheelPainter/Object.cpp \
    3DWheelPainter/OpenglPointMsg.cpp \
    3DWheelPainter/PolarCoordinate.cpp \
    3DWheelPainter/qgotowidget.cpp \
    3DWheelPainter/qlocalmainwindow.cpp \
    3DWheelPainter/qlocalopenglwidget.cpp \
    3DWheelPainter/qlocalspreadmainwindow.cpp \
    3DWheelPainter/qlocalspreadopenglwidget.cpp \
    3DWheelPainter/QuaternionsCamera.cpp \
    3DWheelPainter/RoamingCamera.cpp \
    3DWheelPainter/Ruler.cpp \
    3DWheelPainter/ShaderProgram.cpp \
    3DWheelPainter/StandardTreadMsg.cpp \
    3DWheelPainter/stb_image.cpp \
    3DWheelPainter/Text.cpp \
    3DWheelPainter/TextPointObject.cpp \
    3DWheelPainter/Wheel.cpp \
    3DWheelPainter/WheelGroup.cpp \
    3DWheelPainter/WorldEngine.cpp \
    3DWheelPainter/LocalEngine.cpp \
    3DWheelPainter/coordinateaxis.cpp \
    3DWheelPainter/qworldtrainmainwindow.cpp \
    3DWheelPainter/EulerAngleCamera.cpp \
    3DWheelPainter/qworldopenglwidget.cpp \
    dialogs/dlgpolarcoord.cpp \
    SelfWidget/ticks.cpp \
    SectionPlot/coordpoints.cpp \
    SectionPlot/diagram.cpp \
    SectionPlot/dlglathe.cpp

HEADERS += \
        mainwindow.h \
    arithmetic.h \
    dbmysql.h \
    dbsqlite.h \
    exportthread.h \
    importthread.h \
    mainwindow.h \
    qcustomplot.h \
    wheelsdatathread.h \
    wheelsinfothread.h \
    dialogs/dlgcheckdatas.h \
    dialogs/dlgchecktrain.h \
    dialogs/dlglogin.h \
    dialogs/dlgpathsetting.h \
    dialogs/dlgplot.h \
    dialogs/dlgpolarview.h \
    dialogs/dlgselectstandard.h \
    objects/point.h \
    objects/wheels.h \
    tableview.h \
    dialogs/dlgwheeldtl.h \
    dialogs/dlgdevcommunic.h \
    SelfWidget/toolbutton.h \
    tcpclientsocket.h \
    tcpserver.h \
    SelfWidget/iconwidget.h \
    dialogs/dlgusersearch.h \
    dialogs/dlgmoniterealtime.h \
    dialogs/dlghistorydatas.h \
    dialogs/dlgtrainsearchshow.h \
    dialogs/dlgtraintable.h \
    dialogs/dlguserparasetting.h \
    dialogs/dlgwheelshow.h \
    SelfWidget/progressbarui.h \
    dialogs/dlgwelcome.h \
    dialogs/dlgpasswardinput.h \
    dialogs/dlgdevicelist.h \
    dialogs/dlgmanageparasetting.h \
    dialogs/dlgmanagesearchstdwheel.h \
    dialogs/dlgmanagesearchoridatas.h \
    dialogs/dlgmanagesearchmiddatas.h \
    dialogs/dlgmideddydatas.h \
    dialogs/dlgmidedgeparadatas.h \
    dialogs/dlguserparastat.h \
    dialogs/dlglasercps.h \
    SelfWidget/slider.h \
    dialogs/dlgpalorparasetting.h \
    SelfWidget/chart.h \
    fftw3.h \
    objects/log.h \
    dialogs/dlgdamageanalyze.h \
    SelfWidget/tablewidget.h \
    SelfWidget/wheelicon.h \
    SelfWidget/DateEdit/DefDateTimeEdit.h \
    SelfWidget/DateEdit/DefineCalendar.h \
    3DWheelPainter/CartesianCoordinate.h \
    3DWheelPainter/CoordinateObject.h \
    3DWheelPainter/dlgtrain3d.h \
    3DWheelPainter/Engine_inl.h \
    3DWheelPainter/FilePath.h \
    3DWheelPainter/Font.h \
    3DWheelPainter/Line.h \
    3DWheelPainter/LineObject.h \
    3DWheelPainter/LoadModel.h \
    3DWheelPainter/LocalSpreadEngine.h \
    3DWheelPainter/Mark.h \
    3DWheelPainter/Mesh.h \
    3DWheelPainter/MeshObject.h \
    3DWheelPainter/Object.h \
    3DWheelPainter/OpenglPointMsg.h \
    3DWheelPainter/PickMsg.h \
    3DWheelPainter/Polar.h \
    3DWheelPainter/PolarCoordinate.h \
    3DWheelPainter/qgotowidget.h \
    3DWheelPainter/qlocalmainwindow.h \
    3DWheelPainter/qlocalopenglwidget.hpp \
    3DWheelPainter/qlocalspreadmainwindow.hpp \
    3DWheelPainter/qlocalspreadopenglwidget.hpp \
    3DWheelPainter/QuaternionsCamera.h \
    3DWheelPainter/Range.h \
    3DWheelPainter/RoamingCamera.h \
    3DWheelPainter/Ruler.h \
    3DWheelPainter/ShaderProgram.h \
    3DWheelPainter/StandardTreadMsg.h \
    3DWheelPainter/stb_image.h \
    3DWheelPainter/Text.h \
    3DWheelPainter/TextPointObject.h \
    3DWheelPainter/Texture.h \
    3DWheelPainter/Vertex.h \
    3DWheelPainter/Wheel.h \
    3DWheelPainter/WheelGroup.h \
    3DWheelPainter/WheelObject.h \
    3DWheelPainter/WorldEngine.h \
    3DWheelPainter/LocalEngine.h \
    3DWheelPainter/coordinateaxis.h \
    3DWheelPainter/qworldtrainmainwindow.hpp \
    3DWheelPainter/EulerAngleCamera.h \
    3DWheelPainter/qworldopenglwidget.hpp \
    dialogs/dlgpolarcoord.h \
    SelfWidget/ticks.h \
    SectionPlot/coordpoints.h \
    SectionPlot/diagram.h \
    SectionPlot/dlglathe.h

    SelfWidget/toolbutton.h

FORMS += \
        mainwindow.ui \
    dlgcheckdatas.ui \
    dlgchecktrain.ui \
    dlgpolarview.ui \
    mainwindow.ui \
    dialogs/dlglogin.ui \
    dialogs/dlgpathsetting.ui \
    dialogs/dlgplot.ui \
    dialogs/dlgselectstandard.ui \
    dialogs/dlgwheeldtl.ui \
    dialogs/dlgdevcommunic.ui \
    dialogs/dlgusersearch.ui \
    dialogs/dlgmoniterealtime.ui \
    dialogs/dlghistorydatas.ui \
    dialogs/dlgtrainsearchshow.ui \
    dialogs/dlgtraintable.ui \
    dialogs/dlguserparasetting.ui \
    dialogs/dlgwheelshow.ui \
    3DWheelPainter/dlgtrain3d.ui \
    SelfWidget/progressbarui.ui \
    dialogs/dlgwelcome.ui \
    dialogs/dlgpasswardinput.ui \
    dialogs/dlgdevicelist.ui \
    dialogs/dlgmanageparasetting.ui \
    dialogs/dlgmanagesearchstdwheel.ui \
    dialogs/dlgmanagesearchoridatas.ui \
    dialogs/dlgmanagesearchmiddatas.ui \
    dialogs/dlgmideddydatas.ui \
    dialogs/dlgmidedgeparadatas.ui \
    dialogs/dlguserparastat.ui \
    dialogs/dlglasercps.ui \
    dialogs/dlgpalorparasetting.ui \
    SelfWidget/chart.ui \
    dialogs/dlgdamageanalyze.ui \
    SelfWidget/tablewidget.ui \
    SelfWidget/wheelicon.ui \
    dialogs/dlgpolarcoord.ui \
    SectionPlot/dlglathe.ui




INCLUDEPATH += D:/QT/Tools/GLM/glm-master/



SUBDIRS += \
    LuxrayDamo.pro

RESOURCES += \
    images.qrc

DISTFILES +=

win32: LIBS += -LD:/Qt64/mingw64/lib/ -llibassimp.dll

INCLUDEPATH += D:/Qt64/mingw64/include
DEPENDPATH += D:/Qt64/mingw64/include



win32: LIBS += E:/Qt/fftw-3.3.5/libfftw3-3.dll

INCLUDEPATH += $$PWD/E:/Qt/fftw-3.3.5
DEPENDPATH += $$PWD/E:/Qt/fftw-3.3.5


win32: LIBS += -L$$PWD/../../../fftw-3.3.5/ -lfftw3l-3

INCLUDEPATH += $$PWD/../../../fftw-3.3.5
DEPENDPATH += $$PWD/../../../fftw-3.3.5
