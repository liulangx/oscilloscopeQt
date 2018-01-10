#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainWidget/tools.h"
#include "Widget.h"
#include "DataManageMent/CDataRAW.h"
#include "Widgets/COFFOpenglwidget.h"
#include "DataManageMent/CAllocVBOandVAO.h"
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <boost/thread/mutex.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void onReadFromFileOutDisplayState(QString fileName);
private slots:
    void open();
    void closeEvent(QCloseEvent* event);

private:
    void createActions();
    void createMenus();


    Ui::MainWindow *ui;
    QMenu *fileMenu;

    QAction *separatorAction;
    QAction *openAction;

    Widget *m_widgetCentral;

    //读取文件信息后生成的窗口, 从文件提取
    vector<COFFOpenglWidget*> m_OFFOpenglWidgetsFile;

    CDataRAW m_dataRAW;
    CAllocVBOandVAO*  m_allocVBOandVAO;
    boost::mutex m_mutexOutWidget;
};

#endif // MAINWINDOW_H
