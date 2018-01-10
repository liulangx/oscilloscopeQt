#include "Widgets/CCopyQOpenglWidget.h"


CopyQOpenglWidget::CopyQOpenglWidget(u_char _frameType, CChart::DRAWTYPE _drawType, CAllocVBOandVAO *_allocVBOandVAO, Widget *_mainWidget, QString _checkBoxSenderName, QWidget *parent)
    : QWidget(parent),
      m_allocVBOandVAO(/*new CAllocVBOandVAO*/_allocVBOandVAO),
      m_chart(new QOpenglMain(_frameType, _allocVBOandVAO, _drawType)),
      m_mainVLayout(new QVBoxLayout),
      m_hBoxLayout(new QHBoxLayout),
      m_mainWidget(_mainWidget),
      m_checkBoxSenderName(_checkBoxSenderName),
      m_checkBoxAnix(new QCheckBox(this)),
      m_checkBoxGrid(new QCheckBox(this)),
      m_checkBoxXyPlane(new QCheckBox(this)),
      m_checkBoxXzPlane(new QCheckBox(this)),
      m_checkBoxYzPlane(new QCheckBox(this)),
      m_comboBox(new CComboBox(m_chart, this))
{

}

CopyQOpenglWidget::~CopyQOpenglWidget()
{
    LLDELETE(m_checkBoxAnix)
    LLDELETE(m_checkBoxGrid)
    LLDELETE(m_checkBoxXyPlane)
    LLDELETE(m_checkBoxXzPlane)
    LLDELETE(m_checkBoxYzPlane)
    LLDELETE(m_comboBox)
    LLDELETE(m_hBoxLayout)
    LLDELETE(m_mainVLayout)
    LLDELETE(m_chart)
}

void CopyQOpenglWidget::setChartObjectName(QString _title)
{
    m_chart->setObjectName(_title);
}

void CopyQOpenglWidget::setChartCopyFromOtherOpenWidgetMark(bool _notCopyMark)
{
    m_chart->setCopyFromOtherOpenWidgetMark(_notCopyMark);
}

void CopyQOpenglWidget::setChartVaoAndVbosBuffer(vector<GLfloat>& _tmpVBO, vector<GLuint>& _tmpVBOIndex)
{
    m_chart->setVaoAndVbosBuffer(_tmpVBO, _tmpVBOIndex);
}

void CopyQOpenglWidget::setChartCordRangei(vector2f _xRange, vector2f _yRange, vector2f _zRange)
{
    m_chart->setCordRangei(_xRange, _yRange, _zRange);
}

void CopyQOpenglWidget::setChartDrawType(CChart::DRAWTYPE _drawType)
{
    m_chart->setDrawType(_drawType);
}

void CopyQOpenglWidget::setChartMove(QMatrix4x4 _move)
{
    m_chart->setMove(_move);
}

void CopyQOpenglWidget::setChartRotation(QMatrix4x4 _rotation)
{
    m_chart->setRotation(_rotation);
}

void CopyQOpenglWidget::setChartGridNumer(int _xGridNumber, int _yGridNumber, int _zGridNumber)
{
    m_chart->setGridNumer(_xGridNumber, _yGridNumber, _zGridNumber);
}

void CopyQOpenglWidget::setChartLinesVboBufferData(list<vector<GLfloat> >& _tmpLinesVbo, vector<size_t>& _tmpLinesAmount)
{
    m_chart->setLinesVboBufferData(_tmpLinesVbo, _tmpLinesAmount);
}

void CopyQOpenglWidget::setChartLineColor(map<u_short, vector4f> _lineColors)
{
    m_chart->setLineColor(_lineColors);
    for(map<u_short, vector4f>::iterator it = _lineColors.begin(); it != _lineColors.end(); ++it)
    {
        m_chart->setLinesEnabled(it->first, true);
        QString name = "— " + QString::number(it->first);
        vector3f color = {it->second._r, it->second._g, it->second._b};
        QVariant data1 = QVariant::fromValue(it->first);
        m_comboBox->addNewItemFromOpenglMain(name, data1, color);
    }
}

void CopyQOpenglWidget::setLinesEnabled(size_t _lineIndex, bool _b)
{
    m_chart->setLinesEnabled(_lineIndex, _b);
}

void CopyQOpenglWidget::setTitle(QString _title)
{
    setWindowTitle(_title);
}

void CopyQOpenglWidget::setMainLayout()
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


    m_mainVLayout->addLayout(m_hBoxLayout);
    m_mainVLayout->addWidget(m_chart);
    setLayout(m_mainVLayout);
}

void CopyQOpenglWidget::update()
{
    if(m_chart)
        m_chart->update();
}


void CopyQOpenglWidget::closeEvent(QCloseEvent *event)
{
    //使用了友元类对原widget中的qcheckbutton的状态进行更改
//    int r = QMessageBox::question(this, tr("警告"), tr("确定要退出么？"), QMessageBox::Yes|QMessageBox::Default, QMessageBox::No|QMessageBox::Escape);
//    if(r == QMessageBox::Yes)
//    {
        event->accept();
        QCheckBox* tmpCheckBox = m_mainWidget->findChild<QCheckBox*>(m_checkBoxSenderName);
        QString tmpName = tmpCheckBox->objectName();
        QString indexS = tmpName.split(' ')[1];
        u_int indexUint = indexS.toUInt();

//        qDebug() << "tmpName: " << tmpName;
//        qDebug() << "tmpName: " << indexS;
//        qDebug() << "tmpName: " << indexUint;
        tmpCheckBox->setChecked(false);
        tmpCheckBox->setEnabled(true);

        //m_mainWidget->m_charts.at(indexUint);
//    }
//    else
//    {
//        event->ignore();
//    }
}
