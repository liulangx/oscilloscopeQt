#ifndef COPYQOPENGLWIDGET_H
#define COPYQOPENGLWIDGET_H
#include "mainWidget/tools.h"
#include "MyOpenglWidget/QOpenglMain.h"
#include "DataManageMent/CAllocVBOandVAO.h"
#include "ComboBox/CComboBox.h"
#include <QWidget>
#include <QCheckBox>
//#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QOpenGLFunctions_4_3_Core>
#include <mainWidget/Widget.h>
#include <QThread>

class Widget;

class CopyQOpenglWidget : public QWidget, public QOpenGLFunctions_4_3_Core
{
public:
    explicit CopyQOpenglWidget(u_char _frameType, CChart::DRAWTYPE _drawType, CAllocVBOandVAO* _allocVBOandVAO, Widget* _mainWidget, QString _checkBoxSenderName, QWidget *parent = 0);
    ~CopyQOpenglWidget();
    void setChartObjectName(QString _title);
    void setChartCopyFromOtherOpenWidgetMark(bool _notCopyMark);
    void setChartVaoAndVbosBuffer(vector<GLfloat> &_tmpVBO, vector<GLuint> &_tmpVBOIndex);
    void setChartCordRangei(vector2f _xRange, vector2f _yRange, vector2f _zRange);
    void setChartDrawType(CChart::DRAWTYPE _drawType);
    void setChartMove(QMatrix4x4 _move);
    void setChartRotation(QMatrix4x4 _rotation);
    void setChartGridNumer(int _xGridNumber, int _yGridNumber, int _zGridNumber);
    void setChartLinesVboBufferData(list<vector<GLfloat> > &_tmpLinesVbo, vector<size_t> &_tmpLinesAmount);
    void setChartLineColor(map<u_short, vector4f> _lineColors);

    void setLinesEnabled(size_t _lineIndex, bool _b);

    void setTitle(QString _title);
    void setMainLayout();
    void update();
//public:

//    QOpenglMain* m_chart;
private:
//    friend class CComboBox;

    QOpenglMain* m_chart;
    void closeEvent(QCloseEvent *event);
    QCheckBox* m_checkBoxAnix;
    QCheckBox* m_checkBoxGrid;
    QCheckBox* m_checkBoxXyPlane;
    QCheckBox* m_checkBoxXzPlane;
    QCheckBox* m_checkBoxYzPlane;
    CComboBox* m_comboBox;

    QVBoxLayout* m_mainVLayout;
    QHBoxLayout* m_hBoxLayout;

    CAllocVBOandVAO* m_allocVBOandVAO;
    QString m_checkBoxSenderName;
    Widget* m_mainWidget;
};

#endif // COPYQOPENGLWIDGET_H
