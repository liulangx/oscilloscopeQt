#-------------------------------------------------
#
# Project created by QtCreator 2017-07-10T00:26:30
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QOpenglMainProgram
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS  _LLMINGW32_ u_short=ushort u_char=uchar u_int=uint WIN32_LEAN_AND_MEAN
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#CONFIG +=precompile_header

SOURCES += \
    Widgets/COFFOpenglwidget.cpp \
    Widgets/CLLOpenglWidget.cpp \
    Widgets/CCopyQOpenglWidget.cpp \
    UDPServer/CAsioAsyncServer.cpp \
    Renders/Crender.cpp \
    Renders/CLinesRender.cpp \
    Renders/CGridRender.cpp \
    MyOpenglWidget/QOpenglMain.cpp \
    MyOpenglWidget/Clines.cpp \
    MyOpenglWidget/Chart.cpp \
    mainWidget/Widget.cpp \
    mainWidget/mainwindow.cpp \
    mainWidget/main.cpp \
    GridAndAxis/CGrid.cpp \
    GridAndAxis/Anix.cpp \
    DataManageMent/CDataRAW.cpp \
    DataManageMent/CAllocVBOandVAO.cpp \
    ComboBox/CComboBox.cpp


HEADERS += \
    Widgets/COFFOpenglwidget.h \
    Widgets/CCopyQOpenglWidget.h \
    UDPServer/CAsioAsyncServer.h \
    Renders/Crender.h \
    Renders/CLinesRender.h \
    Renders/CGridRender.h \
    MyOpenglWidget/QOpenglMain.h \
    MyOpenglWidget/Clines.h \
    MyOpenglWidget/Chart.h \
    mainWidget/Widget.h \
    mainWidget/mainwindow.h \
    GridAndAxis/textureindex.h \
    GridAndAxis/CGrid.h \
    GridAndAxis/Anix.h \
    DataManageMent/CDataRAW.h \
    DataManageMent/CAllocVBOandVAO.h \
    ComboBox/CComboBox.h \
    Widgets/CLLOpenglWidget.h \
    mainWidget/tools.h


FORMS += \
        ui/mainwindow.ui \
    ui/widget.ui

DISTFILES += \
    ../build-QOpenglMainProgram-Desktop_Qt_5_9_0_GCC_64bit-Debug/Shaders/lines.geo \
    ../build-QOpenglMainProgram-Desktop_Qt_5_9_0_GCC_64bit-Debug/Shaders/lines.Frag \
    ../build-QOpenglMainProgram-Desktop_Qt_5_9_0_GCC_64bit-Debug/Shaders/coord.frag \
    ../build-QOpenglMainProgram-Desktop_Qt_5_9_0_GCC_64bit-Debug/Shaders/basic.Frag \
    ../build-QOpenglMainProgram-Desktop_Qt_5_9_0_GCC_64bit-Debug/Shaders/lines.vert \
    ../build-QOpenglMainProgram-Desktop_Qt_5_9_0_GCC_64bit-Debug/Shaders/coord.vert \
    ../build-QOpenglMainProgram-Desktop_Qt_5_9_0_GCC_64bit-Debug/Shaders/basic.vert
#INCLUDEPATH += C:\msys64\mingw64\include
#PRECOMPILED_HEADER = C:\msys64\mingw64\include\boost\asio.hpp
#LIBS +=  -lboost_system-mt.dll \
#         -lboost_thread-mt.dll \
#         -lwsock32 \
#         -lws2_32
#LIBS += C:/msys64/mingw64/lib/libboost_system-mt.a C:/msys64/mingw64/lib/libboost_thread-mt.a
#INCLUDEPATH +=/usr/include
#LIBS += /usr/lib/x86_64-linux-gnu/libboost_system.so /usr/lib/x86_64-linux-gnu/libboost_thread.so /usr/lib/x86_64-linux-gnu/libGLU.so


#win32:CONFIG(release, debug|release): \
#    LIBS += -lboost_system-mt.dll -lboost_thread-mt.dll -lwsock32 -lws2_32 \
#    DEFINES += QT_DEPRECATED_WARNINGS _LLMINGW32_ u_short=ushort u_char=uchar u_int=uint WIN32_LEAN_AND_MEAN
#else:win32:CONFIG(debug, debug|release): \
LIBS += -lboost_system-mt.dll -lboost_thread-mt.dll -lwsock32 -lws2_32
#else:unix: \
#    QMAKE_CXXFLAGS += -g \
#    LIBS += /usr/lib/x86_64-linux-gnu/libboost_system.so /usr/lib/x86_64-linux-gnu/libboost_thread.so /usr/lib/x86_64-linux-gnu/libGLU.so \
#    INCLUDEPATH +=/usr/include \
#    DEFINES += QT_DEPRECATED_WARNINGS _LLMINGW32_ u_short=ushort u_char=uchar u_int=uint WIN32_LEAN_AND_MEAN
