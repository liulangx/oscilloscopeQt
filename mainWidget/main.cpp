#include "mainWidget/mainwindow.h"
//#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("飞行器数据可视化平台");
    w.show();
    w.showMaximized();
    w.showMinimized();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}
