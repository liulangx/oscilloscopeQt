#include "mainWidget/mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_allocVBOandVAO(new CAllocVBOandVAO),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_widgetCentral = new Widget(this);
    m_widgetCentral->setStatusTip("本窗口采用opengl实时渲染");

    createActions();
    createMenus();

    m_widgetCentral->run();
    setCentralWidget(m_widgetCentral);
    show();
    raise();
}

MainWindow::~MainWindow()
{
    boost::mutex::scoped_lock lock(m_mutexOutWidget);
    for (vector<COFFOpenglWidget*>::iterator it = m_OFFOpenglWidgetsFile.begin(); it != m_OFFOpenglWidgetsFile.end(); ++it)
    {
        delete (*it);
    }
    m_OFFOpenglWidgetsFile.clear();
    lock.unlock();

    LLDELETE(separatorAction)
    LLDELETE(openAction)
    LLDELETE(fileMenu)
    LLDELETE(m_widgetCentral)
    LLDELETE(m_allocVBOandVAO)
    LLDELETE(ui)
}

void MainWindow::onReadFromFileOutDisplayState(QString fileName)
{
    m_dataRAW.setDir(fileName);
    QStringList titleList = fileName.split('/');
    QString title = titleList.last();
    QString index = title.split(".").at(0).split("_").at(1);
    QString winTitle = fileName.split("img_").at(0) + "图像" + index;
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

    m_dataRAW.readDataFromFileNoName(indexUshort, _tmpLinesVbo, _tmpLinesAmount, lineColors);
    m_dataRAW.readLineRangeFromFileNoName(indexUshort, frameType, _xRange, _yRange, _zRange);


    COFFOpenglWidget* _OFFOpenglWidget = new COFFOpenglWidget(frameType, m_allocVBOandVAO, winTitle);
    _OFFOpenglWidget->setMarkForFile(true);
    _OFFOpenglWidget->setGeometry(40,40,400,400);
    _OFFOpenglWidget->show();
    _OFFOpenglWidget->setWindowTitle(winTitle);
    _OFFOpenglWidget->setChartObjectName(title);
    _OFFOpenglWidget->setChartCopyFromOtherOpenWidgetMark(false);
    _OFFOpenglWidget->setChartCordRangei(_xRange, _yRange, _zRange);
    _OFFOpenglWidget->setChartLinesVboBufferData(_tmpLinesVbo, _tmpLinesAmount);
    _OFFOpenglWidget->setChartLineColor(lineColors);
    _OFFOpenglWidget->setMainLayout();
    _OFFOpenglWidget->raise();

    m_OFFOpenglWidgetsFile.push_back(_OFFOpenglWidget);
}

void MainWindow::open()
{
    //QString fileName = QFileDialog::getOpenFileName(this, tr("打开已有的图像"), ".", tr("数据文件(*.dat)"));
    QStringList fileName = QFileDialog::getOpenFileNames(this, tr("打开已有的图像"), ".", tr("数据文件(*.dat)"));
    for(QStringList::iterator it = fileName.begin(); it != fileName.end(); ++it)
    {
        if(!((*it).isEmpty()))
            onReadFromFileOutDisplayState(*it);
    }
    //    system("pwd >> text.txt");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
   m_widgetCentral->closeTimer();
   event->accept();
}

void MainWindow::createActions()
{
   openAction = new QAction(tr("&Open"), this);
//   openAction->setIcon(QIcon(""));
   openAction->setShortcut(QKeySequence::Open);
   openAction->setStatusTip("打开一个现有文件");
   connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("文件(&F)"));
    fileMenu->addAction(openAction);
    separatorAction = fileMenu->addSeparator();
}

