#include "Widgets/COFFOpenglwidget.h"

COFFOpenglWidget::COFFOpenglWidget(u_char _frameType, CAllocVBOandVAO *_allocVBOandVAO, Widget *_mainWidget, QString _checkBoxSenderName, QWidget *parent)
    : QWidget(parent),
      m_allocVBOandVAO(/*new CAllocVBOandVAO*/_allocVBOandVAO),
      m_mainVLayout(new QVBoxLayout(this)),
      m_hBoxLayout(new QHBoxLayout(this)),
      m_mainWidget(_mainWidget),
      m_checkBoxSenderName(_checkBoxSenderName),
      m_checkBoxAnix(new QCheckBox(this)),
      m_checkBoxGrid(new QCheckBox(this)),
      m_checkBoxXyPlane(new QCheckBox(this)),
      m_checkBoxXzPlane(new QCheckBox(this)),
      m_checkBoxYzPlane(new QCheckBox(this))

{
    if(0 == _frameType)
    {
        m_chart = new QOpenglMain(_frameType, _allocVBOandVAO, CChart::DRAWTYPE::XZY);
    }
    else
    {
        m_chart = new QOpenglMain(_frameType, _allocVBOandVAO, CChart::DRAWTYPE::XY);
    }

    m_comboBox = new CComboBox(m_chart, this);

}

COFFOpenglWidget::COFFOpenglWidget(u_char _frameType, CAllocVBOandVAO *_allocVBOandVAO, QString _checkBoxSenderName, QWidget *parent)
    : QWidget(parent),
      m_allocVBOandVAO(/*new CAllocVBOandVAO*/_allocVBOandVAO),
      m_chart(new QOpenglMain(_frameType, _allocVBOandVAO, CChart::DRAWTYPE::XZY)),
      m_mainVLayout(new QVBoxLayout(this)),
      m_hBoxLayout(new QHBoxLayout(this)),
      m_checkBoxSenderName(_checkBoxSenderName),
      m_checkBoxAnix(new QCheckBox(this)),
      m_checkBoxGrid(new QCheckBox(this)),
      m_checkBoxXyPlane(new QCheckBox(this)),
      m_checkBoxXzPlane(new QCheckBox(this)),
      m_checkBoxYzPlane(new QCheckBox(this)),
      m_label(new QLabel(this))

{
    if(0 == _frameType)
    {
        m_chart = new QOpenglMain(_frameType, _allocVBOandVAO, CChart::DRAWTYPE::XZY);
    }
    else
    {
        m_chart = new QOpenglMain(_frameType, _allocVBOandVAO, CChart::DRAWTYPE::XY);
    }
    m_comboBox = new CComboBox(m_chart, this);
}

COFFOpenglWidget::~COFFOpenglWidget()
{
    LLDELETE(m_checkBoxAnix)
    LLDELETE(m_checkBoxGrid)
    LLDELETE(m_checkBoxXyPlane)
    LLDELETE(m_checkBoxXzPlane)
    LLDELETE(m_checkBoxYzPlane)
    LLDELETE(m_comboBox)
    LLDELETE(m_label)
    LLDELETE(m_hBoxLayout)
    LLDELETE(m_mainVLayout)
    LLDELETE(m_chart)
}

void COFFOpenglWidget::setChartObjectName(QString _title)
{
    m_chart->setObjectName(_title);
}

void COFFOpenglWidget::setChartCopyFromOtherOpenWidgetMark(bool _notCopyMark)
{
    m_chart->setCopyFromOtherOpenWidgetMark(_notCopyMark);
}

void COFFOpenglWidget::setChartVaoAndVbosBuffer(vector<GLfloat>& _tmpVBO, vector<GLuint>& _tmpVBOIndex)
{
    m_chart->setVaoAndVbosBuffer(_tmpVBO, _tmpVBOIndex);
}

void COFFOpenglWidget::setChartCordRangei(vector2f _xRange, vector2f _yRange, vector2f _zRange)
{
    m_chart->setCordRangei(_xRange, _yRange, _zRange);
}

void COFFOpenglWidget::setChartDrawType(CChart::DRAWTYPE _drawType)
{
    m_chart->setDrawType(_drawType);
}

void COFFOpenglWidget::setChartMove(QMatrix4x4 _move)
{
    m_chart->setMove(_move);
}

void COFFOpenglWidget::setChartRotation(QMatrix4x4 _rotation)
{
    m_chart->setRotation(_rotation);
}

void COFFOpenglWidget::setChartGridNumer(int _xGridNumber, int _yGridNumber, int _zGridNumber)
{
    m_chart->setGridNumer(_xGridNumber, _yGridNumber, _zGridNumber);
}

void COFFOpenglWidget::setChartLinesVboBufferData(list<vector<GLfloat> >& _tmpLinesVbo, vector<size_t>& _tmpLinesAmount)
{
    m_chart->setLinesVboBufferData(_tmpLinesVbo, _tmpLinesAmount);
}

void COFFOpenglWidget::setChartLineColor(map<u_short, vector4f> _lineColors)
{
    for(map<u_short, vector4f>::iterator it = _lineColors.begin(); it != _lineColors.end(); ++it)
    {
        m_chart->setColor(it->first, it->second);

        m_chart->setLinesEnabled(it->first, true);

        QString name = "— " + QString::number(it->first);
        vector3f color = {it->second._r, it->second._g, it->second._b};
        QVariant data1 = QVariant::fromValue(it->first);
        m_comboBox->addNewItemFromOpenglMain(name, data1, color);
    }
//    m_chart->setLineColor(_lineColors);
}

void COFFOpenglWidget::setTitle(QString _title)
{
    setWindowTitle(_title);
}

void COFFOpenglWidget::setMainLayout()
{
    m_hBoxLayout->addWidget(m_checkBoxGrid, 0);
    m_hBoxLayout->addWidget(m_checkBoxAnix, 0);
    m_hBoxLayout->addWidget(m_checkBoxXyPlane, 0);
    m_hBoxLayout->addWidget(m_checkBoxXzPlane, 0);
    m_hBoxLayout->addWidget(m_checkBoxYzPlane, 0);
    m_hBoxLayout->addWidget(m_comboBox, 1);
    m_hBoxLayout->addStretch();
    m_checkBoxAnix->setText("坐标");
    m_checkBoxAnix->setChecked(true);
    m_checkBoxGrid->setText("网格");
    m_checkBoxGrid->setChecked(true);
    m_checkBoxXyPlane->setText("XY");
    m_checkBoxXyPlane->setChecked(false);
    m_checkBoxXzPlane->setText("XZ");
    m_checkBoxXzPlane->setChecked(false);
    m_checkBoxYzPlane->setText("YZ");
    m_checkBoxYzPlane->setChecked(false);

    m_checkBoxAnix->setMaximumHeight(15);
    m_checkBoxGrid->setMaximumHeight(15);
    m_checkBoxXyPlane->setMaximumHeight(15);
    m_checkBoxXzPlane->setMaximumHeight(15);
    m_checkBoxYzPlane->setMaximumHeight(15);

    connect(m_checkBoxAnix, SIGNAL(stateChanged(int)), m_chart, SLOT(onSetAnixState()));
    connect(m_checkBoxGrid, SIGNAL(stateChanged(int)), m_chart, SLOT(onSetGridState()));
    connect(m_checkBoxXyPlane, SIGNAL(stateChanged(int)), m_chart, SLOT(onXYPlaneState()));
    connect(m_checkBoxXzPlane, SIGNAL(stateChanged(int)), m_chart, SLOT(onXZPlaneState()));
    connect(m_checkBoxYzPlane, SIGNAL(stateChanged(int)), m_chart, SLOT(onYZPlaneState()));
    QStringList x;
    if(m_markForFile)
    {
        x = m_checkBoxSenderName.split("/");
        //m_label->setText(m_checkBoxSenderName.split("/").last());
        QString tmpName = x.last();
        m_label->setText(tmpName);
        m_mainVLayout->addWidget(m_label, 0, Qt::AlignHCenter);
        m_label->setMaximumHeight(20);
    }
    m_mainVLayout->addLayout(m_hBoxLayout);
    m_mainVLayout->addWidget(m_chart);
    setLayout(m_mainVLayout);
}

void COFFOpenglWidget::update()
{
    if(m_chart)
        m_chart->update();
}

void COFFOpenglWidget::setMarkForFile(bool _b)
{
    m_markForFile = _b;
}


void COFFOpenglWidget::closeEvent(QCloseEvent *event)
{
    //使用了友元类对原widget中的qcheckbutton的状态进行更改
//    int r = QMessageBox::question(this, tr("警告"), tr("确定要退出么？"), QMessageBox::Yes|QMessageBox::Default, QMessageBox::No|QMessageBox::Escape);
//    if(r == QMessageBox::Yes)
//    {
    if(!m_markForFile)
    {
        event->accept();
        QCheckBox* tmpCheckBox = m_mainWidget->findChild<QCheckBox*>(m_checkBoxSenderName);
//        QString tmpName = tmpCheckBox->objectName();
//        QString indexS = tmpName.split(' ')[1];
//        u_int indexUint = indexS.toUInt();

//        qDebug() << "tmpName: " << tmpName;
//        qDebug() << "tmpName: " << indexS;
//        qDebug() << "tmpName: " << indexUint;
        tmpCheckBox->setChecked(false);
        tmpCheckBox->setEnabled(true);
    }

//    }
//    else
//    {
//        event->ignore();
//    }
}
