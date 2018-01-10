#include "Widgets/CLLOpenglWidget.h"

CLLOpenglWidget::CLLOpenglWidget(u_char _frameType, Widget *_mainWidget, CAllocVBOandVAO *_callocVBOandVAO, CChart::DRAWTYPE _drawType, u_short _imageNumber, QWidget *_parent)
    : QWidget(_parent),
      m_frameType(_frameType),
      m_qOpenglMain(new QOpenglMain(_frameType, _callocVBOandVAO, _drawType)),
      m_imageNumber(_imageNumber),
      m_mainWidget(_mainWidget),
      m_mainLayout(new QGridLayout),
      m_hBoxLayout(new QHBoxLayout),
      m_label(new QLabel),
      m_buttonGrid(new QCheckBox(this)),
      m_buttonAnix(new QCheckBox(this)),
      m_buttonHideOtherWidget(new QCheckBox(this)),
      m_buttonOutWidget(new QCheckBox(this)),
      m_buttonXYPlane(new QCheckBox(this)),
      m_buttonXZPlane(new QCheckBox(this)),
      m_buttonYZPlane(new QCheckBox(this)),
      m_buttonOFF(new QCheckBox(this)),
      m_comboBox(new CComboBox(m_qOpenglMain, this))
{
    QString title = QString("图像 ") + QString::number(m_imageNumber);
    m_label->setText(title);
    m_label->setMaximumHeight(15);

    m_mainLayout->addWidget(m_label, 0, 0, Qt::AlignHCenter);

    m_buttonGrid->setText("网格");
    m_buttonGrid->setChecked(true);
    m_buttonGrid->setStatusTip("显示/隐藏网格");
    m_buttonAnix->setText("坐标");
    m_buttonAnix->setChecked(true);
    m_buttonAnix->setStatusTip("显示/隐藏坐标");
    m_buttonHideOtherWidget->setText("隐藏其他");
    m_buttonHideOtherWidget->setChecked(false);
    m_buttonHideOtherWidget->setStatusTip("隐藏其他窗口");
    m_buttonHideOtherWidget->setObjectName(QString("buttonHideOtherWidget ") + QString::number(m_imageNumber));
    m_buttonOutWidget->setText("复制");
    m_buttonOutWidget->setStatusTip("从opengl缓冲区读取当前选中的窗口，并绘制和当前窗口一致的新窗口");
    m_buttonOutWidget->setChecked(false);
    m_buttonOutWidget->setObjectName(QString("buttonOutWidget ") + QString::number(m_imageNumber));
    m_buttonXYPlane->setText("XY");
    m_buttonXYPlane->setChecked(false);
    m_buttonXYPlane->setStatusTip("显示/隐藏XY平面的投影");
    m_buttonXZPlane->setText("XZ");
    m_buttonXZPlane->setChecked(false);
    m_buttonXZPlane->setStatusTip("显示/隐藏XZ平面的投影");
    m_buttonYZPlane->setText("YZ");
    m_buttonYZPlane->setChecked(false);
    m_buttonYZPlane->setStatusTip("显示/隐藏YZ平面的投影");

    m_buttonOFF->setText("提取");
    m_buttonOFF->setStatusTip("该按钮从当前保存的文件中提取数据，并绘制和当前窗口数据一样的新窗口");
    m_buttonOFF->setChecked(false);
    m_buttonOFF->setObjectName(QString("m_buttonOFF ") + QString::number(m_imageNumber));

    connect(m_buttonGrid, SIGNAL(stateChanged(int)), m_qOpenglMain, SLOT(onSetGridState()));
    connect(m_buttonAnix, SIGNAL(stateChanged(int)), m_qOpenglMain, SLOT(onSetAnixState()));
    connect(m_buttonHideOtherWidget, SIGNAL(stateChanged(int)), m_mainWidget, SLOT(onHideOtherOpenGLWidget()));
    connect(m_buttonOutWidget, SIGNAL(stateChanged(int)), m_mainWidget, SLOT(onSetOutDisplayState()));
    connect(m_buttonXYPlane, SIGNAL(stateChanged(int)), m_qOpenglMain, SLOT(onXYPlaneState()));
    connect(m_buttonXZPlane, SIGNAL(stateChanged(int)), m_qOpenglMain, SLOT(onXZPlaneState()));
    connect(m_buttonYZPlane, SIGNAL(stateChanged(int)), m_qOpenglMain, SLOT(onYZPlaneState()));
    connect(m_buttonOFF, SIGNAL(stateChanged(int)), m_mainWidget, SLOT(onReadFromFileOutDisplayState()));

    m_hBoxLayout->addWidget(m_buttonGrid);
    m_hBoxLayout->addWidget(m_buttonAnix);
    m_hBoxLayout->addWidget(m_buttonHideOtherWidget);
    m_hBoxLayout->addWidget(m_buttonOutWidget);
    m_hBoxLayout->addWidget(m_buttonXYPlane);
    m_hBoxLayout->addWidget(m_buttonXZPlane);
    m_hBoxLayout->addWidget(m_buttonYZPlane);
    m_hBoxLayout->addWidget(m_buttonOFF);
    m_hBoxLayout->addWidget(m_comboBox, 1);
    m_hBoxLayout->addStretch();

    m_mainLayout->addLayout(m_hBoxLayout, 1, 0);
    m_mainLayout->addWidget(m_qOpenglMain, 2, 0);
    setLayout(m_mainLayout);
}

CLLOpenglWidget::~CLLOpenglWidget()
{
    LLDELETE(m_qOpenglMain)
    LLDELETE(m_hBoxLayout)
    LLDELETE(m_mainLayout)
    LLDELETE(m_buttonAnix)
    LLDELETE(m_buttonGrid)
    LLDELETE(m_buttonHideOtherWidget)
    LLDELETE(m_buttonOutWidget)
    LLDELETE(m_buttonXYPlane)
    LLDELETE(m_buttonXZPlane)
    LLDELETE(m_buttonYZPlane)
    LLDELETE(m_comboBox)
    LLDELETE(m_buttonOFF)
    LLDELETE(m_label)
}

bool CLLOpenglWidget::addPoint(size_t _lineindex, const vector3f &_posion)
{
    return m_qOpenglMain->addPoint(_lineindex, _posion);
}

void CLLOpenglWidget::setColor(size_t _lineIndex, vector4f _color)
{
    m_qOpenglMain->setColor(_lineIndex, _color);
    QString name = "— " + QString::number(_lineIndex);
    vector3f color = {_color._r, _color._g, _color._b};
    QVariant data1 = QVariant::fromValue(_lineIndex);
//    size_t x = data1.value<size_t>();
    m_comboBox->addNewItemFromOpenglMain(name, data1, color);
}

bool CLLOpenglWidget::checkRangeChange(vector2f &_xRange, vector2f &_yRange, vector2f &_zRange)
{
    return m_qOpenglMain->checkRangeChange(_xRange, _yRange, _zRange);
}

u_char CLLOpenglWidget::getFrameType() const
{
    return m_frameType;
}

void CLLOpenglWidget::update()
{
    m_qOpenglMain->update();
}

void CLLOpenglWidget::setLinesEnabled(size_t _lineIndex, bool _b)
{
    m_qOpenglMain->setLinesEnabled(_lineIndex, _b);
}

QOpenglMain *CLLOpenglWidget::getQOpenglMain() const
{
    return m_qOpenglMain;
}

