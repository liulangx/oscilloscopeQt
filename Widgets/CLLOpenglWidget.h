#ifndef CLLOPENGLWIDGET_H
#define CLLOPENGLWIDGET_H
#include "mainWidget/tools.h"
#include "MyOpenglWidget/QOpenglMain.h"
#include "mainWidget/Widget.h"
#include "ComboBox/CComboBox.h"
#include <QCheckBox>
#include <QLabel>

class Widget;

class CLLOpenglWidget : public QWidget
{
public:
    explicit CLLOpenglWidget(u_char _frameType, Widget *_mainWidget, CAllocVBOandVAO *_callocVBOandVAO, CChart::DRAWTYPE _drawType, u_short _imageNumber,QWidget *_parent = nullptr);
    ~CLLOpenglWidget();

    bool addPoint(size_t _lineindex, const vector3f & _posion);
    void setColor(size_t _lineIndex, vector4f _color);
    bool checkRangeChange(vector2f& _xRange, vector2f& _yRange, vector2f& _zRange);

    u_char getFrameType() const;

    void update();

    void setLinesEnabled(size_t _lineIndex, bool _b);

    QOpenglMain *getQOpenglMain() const;

//public:
//    QOpenglMain *m_qOpenglMain;
private:
//    friend class CComboBox;
    QOpenglMain *m_qOpenglMain;
    Widget      *m_mainWidget;

    QGridLayout *m_mainLayout;
    QLabel      *m_label;
    QHBoxLayout *m_hBoxLayout;
    QCheckBox   *m_buttonGrid;
    QCheckBox   *m_buttonAnix;
    QCheckBox   *m_buttonHideOtherWidget;
    QCheckBox   *m_buttonOutWidget;
    QCheckBox   *m_buttonXYPlane;
    QCheckBox   *m_buttonXZPlane;
    QCheckBox   *m_buttonYZPlane;
    CComboBox   *m_comboBox;

    QCheckBox   *m_buttonOFF;

    u_short     m_imageNumber;
    u_char      m_frameType = 0;
};

#endif // CLLOPENGLWIDGET_H
