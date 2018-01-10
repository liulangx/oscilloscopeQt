#ifndef COFFOPENGLWIDGET_H
#define COFFOPENGLWIDGET_H
//Open From Files == OFF

#include "mainWidget/tools.h"
#include "MyOpenglWidget/QOpenglMain.h"
#include "DataManageMent/CAllocVBOandVAO.h"
#include "ComboBox/CComboBox.h"
#include <QWidget>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QOpenGLFunctions_4_3_Core>
#include <mainWidget/Widget.h>
#include <QThread>

class Widget;

class COFFOpenglWidget : public QWidget, public QOpenGLFunctions_4_3_Core
{
public:
    explicit COFFOpenglWidget(u_char _frameType, CAllocVBOandVAO* _allocVBOandVAO, Widget* _mainWidget, QString _checkBoxSenderName, QWidget *parent = 0);
    explicit COFFOpenglWidget(u_char _frameType, CAllocVBOandVAO* _allocVBOandVAO, QString _checkBoxSenderName, QWidget *parent = 0);
    ~COFFOpenglWidget();
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

    void setTitle(QString _title);
    void setMainLayout();
    void update();
    void setMarkForFile(bool _b);

    //void show();
//public:
//    QOpenglMain* m_chart;
private:
//    friend class CComboBox;
    QOpenglMain* m_chart;

    void closeEvent(QCloseEvent *event);
    QLabel *    m_label = nullptr;
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

    bool m_markForFile = false;

};

#endif // COFFOPENGLWIDGET_H
