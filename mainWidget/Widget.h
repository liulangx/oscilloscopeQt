#ifndef WIDGET_H
#define WIDGET_H
#include "mainWidget/tools.h"
#include "UDPServer/CAsioAsyncServer.h"
#include "MyOpenglWidget/QOpenglMain.h"
#include "Widgets/CCopyQOpenglWidget.h"
#include "Widgets/CLLOpenglWidget.h"
#include "Widgets/COFFOpenglwidget.h"
#include "DataManageMent/CDataRAW.h"

#include <boost/thread/mutex.hpp>

#include <boost/thread.hpp>

#include <map>
#include <fstream>
#include <QLayout>
#include <QBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QTime>
#include <QThread>

class CopyQOpenglWidget;
class CLLOpenglWidget;
class COFFOpenglWidget;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = Q_NULLPTR);
    ~Widget();
//    void addPoint(float x, float y, float z);
    void update();
    void recvMessage();
//    void updateOutWidget();
    void run();
    void closeTimer();

//signals:
//    void buildOpenGLWidget();
private slots:
    void onTimeOut();
    void onHideOtherOpenGLWidget();
    void onSetOutDisplayState();
    void onReadFromFileOutDisplayState();

private:
    void onBuildOpenGLWidget(u_char _frameType);
    void readCirBufToCreateOpenGLImages();
    void removeLayout(QWidget* _widget);
    void deleteOutWidget(u_int _indexCopy);
    void deleteOFFOutWidget(u_int _indexCopy);

    void createOutWidget();
private:
    Ui::Widget *ui;
    //boost circularBuffer类
    CirBufferLL             m_cirBuffer;
    //asio收发类
    CAsioAsyncServer*       m_server;
    //收到信号后建立的Opengl窗口类的map, 后期应当将list修改为map
    map<u_short, QOpenglMain*>      m_charts;           //窗口编号和类
    map<u_short, CLLOpenglWidget*>  m_llOpengglWidgets;

    /* 复制窗口实现描述:
     * 1. 以下指针都是为了给单独弹窗的小窗口提供临时指针的类，后期将会改为list，不再对放大的窗口进行每次删除;
     * 2. 实际上复制该窗口是直接将点击的对应窗口进行的数据拷贝，由于上下文环境改变，单纯的复制该窗口类是不可行的，
     * 3. 此处采用将数据、网格设置和坐标轴设置进行拷贝后，直接新建一个QOpenglMain类，将前面的数据传递进去后，利用新的widget进行显示的。
     */
    QOpenglMain*            m_tmpOpenglMain = nullptr;    //将选定的窗口的临时指针读取出来

    map<u_short, CopyQOpenglWidget*> m_copyQOpenglWidgets;
    QString                 m_outIndex = "";            //传递其标号

    QGridLayout*            m_thebaselayout;
    CAllocVBOandVAO*        m_allocVBOandVAO;
    QTimer                  m_timer;
    boost::thread*          m_threadRecv;
    vector<u_short>         m_imageNumbers;

    boost::mutex            m_mutexOutWidget;       //为生成的复制窗口进行读写保护

    //以下是用来存储和读取文件信息的。
    map<u_short, ofstream>  m_fins;
    map<u_short, QString>   m_fNames;
    CDataRAW                m_dataRAW;
    //读取文件信息后生成的窗口, 从窗口提取
    map<u_short, COFFOpenglWidget*> m_OFFOpenglWidgets;


protected:
    friend class CopyQOpenglWidget;
    friend class CLLOpenglWidget;
};

#endif // WIDGET_H
