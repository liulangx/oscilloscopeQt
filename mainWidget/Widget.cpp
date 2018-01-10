#include "mainWidget/Widget.h"
#include "ui_widget.h"

#ifdef _LLMINGW32_
    #define random rand
#endif


const long int queueSize = 5000;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    m_cirBuffer(queueSize),
    m_allocVBOandVAO(new CAllocVBOandVAO)
{
    //qDebug() << "main: " << thread()->currentThreadId();
    //setFocusPolicy(Qt::FocusPolicy::TabFocus);
    this->setObjectName("Widget");
    m_thebaselayout = new QGridLayout(this);
    this->setLayout(m_thebaselayout);
    ui->setupUi(this);
    //connect(this, SIGNAL(buildOpenGLWidget()), this, SLOT(onBuildOpenGLWidget()), Qt::ConnectionType::DirectConnection);
    m_dataRAW.setDir();
}
Widget::~Widget()
{
    for(map<u_short, CopyQOpenglWidget*>::iterator it = m_copyQOpenglWidgets.begin(); it != m_copyQOpenglWidgets.end(); ++it)
    {
        LLDELETE(it->second)
    }
    m_copyQOpenglWidgets.clear();

    if(m_thebaselayout)
        LLDELETE(m_thebaselayout)
    for (map<u_short, CLLOpenglWidget*>::iterator it = m_llOpengglWidgets.begin(); it != m_llOpengglWidgets.end(); ++it)
    {
        LLDELETE(it->second)
    }
    m_llOpengglWidgets.clear();

    for (map<u_short, COFFOpenglWidget*>::iterator it = m_OFFOpenglWidgets.begin(); it != m_OFFOpenglWidgets.end(); ++it)
    {
        LLDELETE(it->second)
    }
    m_OFFOpenglWidgets.clear();


    LLDELETE(m_allocVBOandVAO)
}

//void Widget::addPoint(float x, float y, float z)
//{
////    for (map<u_short, QOpenglMain*>::iterator it = m_charts.begin(); it != m_charts.end(); ++it)
////    {
////        it->second->addPoint(0, { x, y, z});
////    }
//    for (map<u_short, CLLOpenglWidget*>::iterator it = m_llOpengglWidgets.begin(); it != m_llOpengglWidgets.end(); ++it)
//    {
//        it->second->addPoint(0, { x, y, z});
//    }

//}

void Widget::update()
{
//    for (map<u_short, QOpenglMain*>::iterator it = m_charts.begin(); it != m_charts.end(); ++it)
//    {
//        it->second->update();
//    }
    for (map<u_short, CLLOpenglWidget*>::iterator it = m_llOpengglWidgets.begin(); it != m_llOpengglWidgets.end(); ++it)
    {
        if(it->second)
            it->second->update();
    }
    //开启后显示为空白

    //外显窗口的刷新是专门从另外一个线程中进行的

}

void Widget::recvMessage()
{
    try{

        boost::asio::io_service io_server;
        unsigned short port = 8003;

        m_server = new CAsioAsyncServer(io_server, port);
        m_server->run();

        //boost::thread t(boost::bind(&boost::asio::io_service::run, &io_server));
        io_server.run();
        //t.join();
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

//void Widget::updateOutWidget()
//{
//    while(1)
//    {
//        //当在主线程中开启复制窗口的刷新时，会导致opengl缓冲区复制过程出错，全部变量被重置为0,所以采用新开启一个线程去刷新复制窗口，同时，在删除复制窗口类的位置使用互斥锁进行保护。
//        usleep(50000); //50ms刷新一次。
//        for (map<u_short, CopyQOpenglWidget*>::iterator it = m_copyQOpenglWidgets.begin(); it != m_copyQOpenglWidgets.end(); ++it)
//        {
//            it->second->update();
//        }
//        for (map<u_short, COFFOpenglWidget*>::iterator it = m_OFFOpenglWidgets.begin(); it != m_OFFOpenglWidgets.end(); ++it)
//        {
//            it->second->update();
//        }
////        for (map<u_short, COFFOpenglWidget*>::iterator it = m_OFFOpenglWidgetsFile.begin(); it != m_OFFOpenglWidgetsFile.end(); ++it)
////        {
////            it->second->update();
////        }
//    }
//}

void Widget::run()
{
    m_timer.start(20);

    //recvMessage();
    m_threadRecv = new boost::thread(std::bind(&Widget::recvMessage, this));//开启数据读取线程
//    boost::thread(std::bind(&Widget::updateOutWidget, this)); //开启复制窗口刷新线程
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
}

void Widget::closeTimer()
{
    m_timer.destroyed();
    m_server->close();
//    m_threadRecv->join();
}

//void Widget::keyPressEvent(QKeyEvent* event)
//{
//    if (Qt::Key_A==event->key())
//    {
//        //onBuildOpenGLWidget();
//        emit buildOpenGLWidget();
//    }
//}
int drawType = CChart::XZY;
void Widget::readCirBufToCreateOpenGLImages()
{
    //清空并重新读取CirBuffer中的数据
    m_cirBuffer.clear();
    m_server->getCirBuffer(&m_cirBuffer);

    float x = 0;
    float y = 0;
    float z = 0;
    vector4f _color;
//    QOpenglMain* _tmpCurrent;
    u_short imgNumber;
    u_short lineNumber;
    //u_short lastNumber = -1;
    bool createLineMark = false;
    vector2f xRange = {1.0f, 0.0f};
    vector2f yRange = {1.0f, 0.0f};
    vector2f zRange = {1.0f, 0.0f};


    for(boost::circular_buffer<Message>::iterator it = m_cirBuffer.begin(); it != m_cirBuffer.end(); )
    {
        imgNumber = it->mData.imageNumber;
        lineNumber = it->mData.lineNumber;

        vector<u_short>::iterator itFind = ::find(m_imageNumbers.begin(), m_imageNumbers.end(), imgNumber);
        if(m_imageNumbers.end() == itFind)
        {
            m_imageNumbers.push_back(imgNumber);
            onBuildOpenGLWidget(it->frameType);
            m_dataRAW.addNewImage(imgNumber);
            continue;
        }
        else
        {
            x = it->mData.dataX;
            y = it->mData.dataY;
            z = it->mData.dataZ;
            //_tmpCurrent = m_charts.at(imgNumber);
            //_tmpCurrent = m_llOpengglWidgets.at(imgNumber)->getQOpenglMain();


            if(m_llOpengglWidgets.at(imgNumber))
            {
                 createLineMark = m_llOpengglWidgets.at(imgNumber)->addPoint(lineNumber, { x, y, z});
                //_tmpCurrent->addPoint(0, { x, y, z});
                if(m_llOpengglWidgets.at(imgNumber)->checkRangeChange(xRange, yRange, zRange))
                {
                    m_dataRAW.addNewLineRange(imgNumber, m_llOpengglWidgets.at(imgNumber)->getFrameType(), xRange, yRange, zRange);
                }
                //_tmpCurrent->setColor(0, _color);
                if(createLineMark)
                {
                    _color._r = float(random()) / float(RAND_MAX); _color._g = float(random()) / float(RAND_MAX);
                    _color._b = float(random()) / float(RAND_MAX); _color._a = float(random()) / float(RAND_MAX);
                    m_llOpengglWidgets.at(imgNumber)->setColor(lineNumber, _color);
                    m_dataRAW.addNewLineColor(imgNumber, lineNumber, _color);
                }
            }

            m_dataRAW.writeDataToFile(imgNumber, lineNumber, it->frame, x, y, z);
        }
        ++it;
        if(m_cirBuffer.size() != 0)
            m_cirBuffer.pop_front();
        //lastNumber = imgNumber;
    }
}

void Widget::removeLayout(QWidget *_widget)
{
    QLayout* layout = _widget->layout ();
    if (layout != 0)
    {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != 0)
        {
            //qDebug() << layout->objectName();
            layout->removeItem (item);

            delete item;
        }
        delete layout;
        m_thebaselayout = nullptr;
    }
}

void Widget::deleteOutWidget(u_int _indexCopy)
{
    //写了一个临界区或者说互斥锁来保护删除过程
    boost::mutex::scoped_lock lock(m_mutexOutWidget);
    //delete m_copyQOpenglWidgets.at(_indexCopy);
    //m_copyQOpenglWidgets.at(_indexCopy) = nullptr;
    m_copyQOpenglWidgets.erase(m_copyQOpenglWidgets.find(_indexCopy));
    lock.unlock();
}

void Widget::deleteOFFOutWidget(u_int _indexCopy)
{
    //写了一个临界区或者说互斥锁来保护删除过程
    boost::mutex::scoped_lock lock(m_mutexOutWidget);
//    delete m_OFFOpenglWidgets.at(_indexCopy);
//    m_OFFOpenglWidgets.at(_indexCopy) = nullptr;
    m_OFFOpenglWidgets.erase(m_OFFOpenglWidgets.find(_indexCopy));
    lock.unlock();
}

//void Widget::deleteOFFOutWidgetFile(u_int _indexCopy)
//{

//}

void Widget::createOutWidget()
{

}

//void Widget::onReadFromFileOutDisplayState(QString fileName)
//{

//}


void Widget::onTimeOut()
{
    //将cirbuf中的数据读出并赋给opengl窗口
    readCirBufToCreateOpenGLImages();
    //刷新opengl窗口内容
    update();
}
void Widget::onBuildOpenGLWidget(u_char _frameType)
{
    QString title = QString("newchart ") + QString::number((*(m_imageNumbers.end()-1)));
    CLLOpenglWidget* newOpenglWidget = nullptr;
    if(0 == _frameType)
    {
        drawType = CChart::DRAWTYPE::XZY;
        newOpenglWidget = new CLLOpenglWidget(_frameType, this, m_allocVBOandVAO, CChart::DRAWTYPE(drawType), (u_short)(*(m_imageNumbers.end()-1)), this);
    }
    else if(1 == _frameType)
    {
        drawType = CChart::DRAWTYPE::XY;
        newOpenglWidget = new CLLOpenglWidget(_frameType, this, m_allocVBOandVAO, CChart::DRAWTYPE(drawType), (u_short)(*(m_imageNumbers.end()-1)), this);
    }
    newOpenglWidget->setObjectName(title);

    m_thebaselayout->addWidget(newOpenglWidget, m_llOpengglWidgets.size() / 3, m_llOpengglWidgets.size() % 3);
    if(newOpenglWidget)
        m_llOpengglWidgets.insert(std::pair<u_short, CLLOpenglWidget*>((u_short)(*(m_imageNumbers.end()-1)), newOpenglWidget));
}
void Widget::onHideOtherOpenGLWidget()
{
    QCheckBox* checkSender = qobject_cast<QCheckBox*>(sender());
    QString title = checkSender->objectName();
    QStringList titleList = title.split(' ');
    QString index = titleList.at(1);
    GLint indexInt = index.toInt();

    u_short indexUshort = 0;
    if (indexInt >= 0)
    {
        indexUshort = static_cast<u_short>(indexInt);
    }
    GLboolean isChecked = GL_TRUE;
    if(checkSender->isChecked())
    {
        isChecked = GL_TRUE;
    }
    else
    {
        isChecked = GL_FALSE;
    }
    for(map<u_short, CLLOpenglWidget*>::iterator it = m_llOpengglWidgets.begin(); it != m_llOpengglWidgets.end(); ++it)
    {
        if(it->first != indexUshort)
        {
            if(isChecked)
            {
                it->second->hide();
            }
            else
            {
                it->second->show();
            }
        }
    }
}

void Widget::onSetOutDisplayState()
{
    QCheckBox* checkSender = qobject_cast<QCheckBox*>(sender());
    //当勾选按键属于非勾选状态时，直接退出该函数。
    if(!checkSender->isChecked())
        return;
    //在勾选成功后禁用勾选按键功能。
    if(checkSender)
    {
        if(checkSender->isEnabled())
            checkSender->setEnabled(false);
//        for(map<u_short, QCheckBox*>::iterator it = m_buttonOutWidgets.begin(); it != m_buttonOutWidgets.end(); ++it)
//        {
//            it->second->setEnabled(false);
//        }
//        QMessageBox msgBox;
//        msgBox.setText(QString("找到信号发射源"));
//        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(QString("未找到信号发射源"));
        msgBox.exec();
    }

    QString title = checkSender->objectName();
    QStringList titleList = title.split(' ');
    QString index = titleList.at(1);
    QString winTitle = QString("图像 ") + index;
    GLint indexInt = index.toInt();
    u_short indexUshort = 0;
    if (indexInt >= 0)
    {
        indexUshort = static_cast<u_short>(indexInt);
    }

    title = QString("newchart ") + index;
    m_tmpOpenglMain = m_llOpengglWidgets.at(indexUshort)->getQOpenglMain();

//    vector<GLfloat> _tmpVbo;
//    vector<GLuint> _tmpVboIndex;
    list<vector<GLfloat> > _tmpLinesVbo;
    vector<size_t> _tmpLinesAmount;
    vector2f _xRange = {0, 1};
    vector2f _yRange = {0, 1};
    vector2f _zRange = {0, 1};
    QMatrix4x4 _rotation = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    QMatrix4x4 _move = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    CChart::DRAWTYPE _drawType = CChart::DRAWTYPE::XZY;
    int xGridNumber = 4;
    int yGridNumber = 4;
    int zGridNumber = 4;
    map<u_short, vector4f> lineColors;

    u_char frameType = 0;
    if(m_tmpOpenglMain)
    {
        m_tmpOpenglMain->getAllVaoAndVboAndVboIndex();
//        _tmpVbo = m_tmpOpenglMain->getAnixVboBufferData();
//        _tmpVboIndex = m_tmpOpenglMain->getAnixVboIndexBufferData();
        m_tmpOpenglMain->getLinesVboBufferData(_tmpLinesVbo, _tmpLinesAmount);
        m_tmpOpenglMain->getCordRangei(_xRange, _yRange, _zRange);
        m_tmpOpenglMain->getMove(_move);
        m_tmpOpenglMain->getRotation(_rotation);
        m_tmpOpenglMain->getDrawType(_drawType);
//        m_tmpOpenglMain->getGridNumer(xGridNumber, yGridNumber, zGridNumber);
        m_tmpOpenglMain->getLineColor(lineColors);
        frameType = m_llOpengglWidgets.at(indexUshort)->getFrameType();
    }
//    if(_tmpVbo.size() != 0)
//        qDebug() << "1:  " << _tmpVbo.at(0) << " " << _tmpVbo.size();



    if(m_copyQOpenglWidgets.find(indexUshort) == m_copyQOpenglWidgets.end())
    {
        CopyQOpenglWidget* _copyQOpenglWidget;
        if(0 == frameType)
        {
            _copyQOpenglWidget = new CopyQOpenglWidget(frameType, CChart::DRAWTYPE::XZY, m_allocVBOandVAO, this, checkSender->objectName());
        }
        else
        {
            _copyQOpenglWidget = new CopyQOpenglWidget(frameType, CChart::DRAWTYPE::XY, m_allocVBOandVAO, this, checkSender->objectName());
        }
        _copyQOpenglWidget->setGeometry(40,40,400,400);

        _copyQOpenglWidget->setWindowTitle(winTitle);
        _copyQOpenglWidget->setChartObjectName(title);
        _copyQOpenglWidget->setChartCopyFromOtherOpenWidgetMark(false);
//        _copyQOpenglWidget->setChartVaoAndVbosBuffer(_tmpVbo, _tmpVboIndex);;
        _copyQOpenglWidget->setChartCordRangei(_xRange, _yRange, _zRange);;
        _copyQOpenglWidget->setChartDrawType(_drawType);
        _copyQOpenglWidget->setChartMove(_move);
        _copyQOpenglWidget->setChartRotation(_rotation);
//        _copyQOpenglWidget->setChartGridNumer(xGridNumber, yGridNumber, zGridNumber);
        _copyQOpenglWidget->setChartLinesVboBufferData(_tmpLinesVbo, _tmpLinesAmount);
        _copyQOpenglWidget->setChartLineColor(lineColors);
        _copyQOpenglWidget->setMainLayout();
        _copyQOpenglWidget->show();
        _copyQOpenglWidget->raise();

        m_copyQOpenglWidgets.insert(std::pair<u_short, CopyQOpenglWidget*>(indexUshort, _copyQOpenglWidget));
    }
    else{
        deleteOutWidget(indexUshort);
//        delete m_copyQOpenglWidgets.at(indexUshort);
        //m_copyQOpenglWidgets.erase(m_copyQOpenglWidgets.find(indexUshort));
        CopyQOpenglWidget* _copyQOpenglWidget;
        if(0 == frameType)
        {
            _copyQOpenglWidget = new CopyQOpenglWidget(frameType, CChart::DRAWTYPE::XZY, m_allocVBOandVAO, this, checkSender->objectName());
        }
        else
        {
            _copyQOpenglWidget = new CopyQOpenglWidget(frameType, CChart::DRAWTYPE::XY, m_allocVBOandVAO, this, checkSender->objectName());
        }
        _copyQOpenglWidget->setGeometry(40,40,400,400);

        _copyQOpenglWidget->setWindowTitle(winTitle);
        _copyQOpenglWidget->setChartObjectName(title);
        _copyQOpenglWidget->setChartCopyFromOtherOpenWidgetMark(false);
//        _copyQOpenglWidget->setChartVaoAndVbosBuffer(_tmpVbo, _tmpVboIndex);;
        _copyQOpenglWidget->setChartCordRangei(_xRange, _yRange, _zRange);;
        _copyQOpenglWidget->setChartDrawType(_drawType);
        _copyQOpenglWidget->setChartMove(_move);
        _copyQOpenglWidget->setChartRotation(_rotation);
        _copyQOpenglWidget->setChartGridNumer(xGridNumber, yGridNumber, zGridNumber);
        _copyQOpenglWidget->setChartLinesVboBufferData(_tmpLinesVbo, _tmpLinesAmount);
        _copyQOpenglWidget->setChartLineColor(lineColors);
        _copyQOpenglWidget->setMainLayout();
        _copyQOpenglWidget->show();
        _copyQOpenglWidget->raise();

        m_copyQOpenglWidgets.insert(std::pair<u_short, CopyQOpenglWidget*>(indexUshort, _copyQOpenglWidget));
    }
}

void Widget::onReadFromFileOutDisplayState()
{
    QCheckBox* checkSender = qobject_cast<QCheckBox*>(sender());
    //当勾选按键属于非勾选状态时，直接退出该函数。
    if(!checkSender->isChecked())
        return;
    //在勾选成功后禁用勾选按键功能。
    if(checkSender)
    {
        if(checkSender->isEnabled())
            checkSender->setEnabled(false);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(QString("未找到信号发射源"));
        msgBox.exec();
    }

    QString title = checkSender->objectName();
    QStringList titleList = title.split(' ');
    QString index = titleList.at(1);
    QString winTitle = QString("图像 ") + index;
    GLint indexInt = index.toInt();
    u_short indexUshort = 0;
    if (indexInt >= 0)
    {
        indexUshort = static_cast<u_short>(indexInt);
    }

    list<vector<GLfloat> > _tmpLinesVbo;
    vector<size_t> _tmpLinesAmount;
    vector2f _xRange = {1, 0};
    vector2f _yRange = {1, 0};
    vector2f _zRange = {1, 0};
    map<u_short, vector4f> lineColors;
    u_char frameType = 0;

    m_dataRAW.readDataFromFile(indexUshort, _tmpLinesVbo, _tmpLinesAmount, lineColors);
    m_dataRAW.readLineRangeFromFile(indexUshort, frameType, _xRange, _yRange, _zRange);



    if(m_OFFOpenglWidgets.find(indexUshort) == m_OFFOpenglWidgets.end())
    {
        COFFOpenglWidget* _OFFOpenglWidget = new COFFOpenglWidget(frameType, m_allocVBOandVAO, this, checkSender->objectName());
        _OFFOpenglWidget->setGeometry(40,40,400,400);
        _OFFOpenglWidget->setWindowTitle(winTitle);
        _OFFOpenglWidget->setChartObjectName(title);
        _OFFOpenglWidget->setChartCopyFromOtherOpenWidgetMark(false);
        _OFFOpenglWidget->setChartCordRangei(_xRange, _yRange, _zRange);
        _OFFOpenglWidget->setChartLinesVboBufferData(_tmpLinesVbo, _tmpLinesAmount);
        if(0 == frameType)
            _OFFOpenglWidget->setChartDrawType(CChart::DRAWTYPE::XZY);
        else
            _OFFOpenglWidget->setChartDrawType(CChart::DRAWTYPE::XY);
        _OFFOpenglWidget->setChartLineColor(lineColors);
        _OFFOpenglWidget->setMainLayout();
        _OFFOpenglWidget->show();
        _OFFOpenglWidget->raise();

        m_OFFOpenglWidgets.insert(std::pair<u_short, COFFOpenglWidget*>(indexUshort, _OFFOpenglWidget));
    }
    else{
        deleteOFFOutWidget(indexUshort);
        COFFOpenglWidget* _OFFOpenglWidget = new COFFOpenglWidget(frameType, m_allocVBOandVAO, this, checkSender->objectName());
        _OFFOpenglWidget->setGeometry(40,40,400,400);

        _OFFOpenglWidget->setWindowTitle(winTitle);
        _OFFOpenglWidget->setChartObjectName(title);
        _OFFOpenglWidget->setChartCopyFromOtherOpenWidgetMark(false);
        _OFFOpenglWidget->setChartCordRangei(_xRange, _yRange, _zRange);
        _OFFOpenglWidget->setChartLinesVboBufferData(_tmpLinesVbo, _tmpLinesAmount);
        if(0 == frameType)
            _OFFOpenglWidget->setChartDrawType(CChart::DRAWTYPE::XZY);
        else
            _OFFOpenglWidget->setChartDrawType(CChart::DRAWTYPE::XY);
        _OFFOpenglWidget->setChartLineColor(lineColors);

        _OFFOpenglWidget->setMainLayout();
        _OFFOpenglWidget->show();
        _OFFOpenglWidget->raise();

        m_OFFOpenglWidgets.insert(std::pair<u_short, COFFOpenglWidget*>(indexUshort, _OFFOpenglWidget));
    }
}

