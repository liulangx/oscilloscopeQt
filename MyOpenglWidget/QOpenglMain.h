//utf8
#pragma once
#define u_char uchar
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <QTimer>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QMouseEvent>
#include <QGridLayout>
#include <QOpenGLWindow>
#include <QDockWidget>
#include "Chart.h"
#include "Renders/Crender.h"
#include <QOffscreenSurface>

struct VaoVbos {
    //anix
    GLuint						m_anixVao;
    GLuint						m_anixVbo;
    GLuint						m_anixVboIndex;						//索引
    //grid
    GLuint                      m_gridVaoGridAnixnumDgt;        //数字VAO
    GLuint                      m_gridVboGridAnixnumDgt;        //VBO
    GLuint                      m_gridVboGridAnixnumDgtTexture;   //texture vbo

    GLuint                      m_gridVaoGridAnixnumPnt;        //小数点VAO
    GLuint                      m_gridVboGridAnixnumPnt;        //VBO
    GLuint                      m_gridVboGridAnixnumPntTexture;   //texture vbo

    GLuint                      m_gridVaoGridAnixnumMinus;        //负号VAO
    GLuint                      m_gridVboGridAnixnumMinus;        //VBO
    GLuint                      m_gridVboGridAnixnumMinusTexture;   //texture vbo

    GLuint                      m_gridVaoGridAnixnumLabel;        //"xyz/"
    GLuint                      m_gridVboGridAnixnumLabel;        //VBO
    GLuint                      m_gridVboGridAnixnumLabelTexture;   //texture vbo

    //clines
    list<GLuint>                      m_linesVaos;
    list<GLuint>                      m_linesVbos;
};
class QOpenglMain : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
	Q_OBJECT

public:
    //构造函数
    explicit QOpenglMain(u_char _frameType, CAllocVBOandVAO* callocVBOandVAO, CChart::DRAWTYPE drawType, QWidget *parent = 0);
    //析构函数
	~QOpenglMain();

    //设置线条的颜色
    void setColor(size_t _lineIndex, vector4f _color);

    //添加线条的点
    bool addPoint(size_t _lineindex, const vector3f& _posion);
    //设置网格开关
    void setGridOnOrOff();
    void setGridOnOrOff(bool _b);

    //设置坐标轴开关
    void setAnixLabelOnOrOff();

    //设置坐标轴精度
    void setAxisPrecision(GLint _precision);

    void setLineXYPlaneOn();
    void setLineXZPlaneOn();
    void setLineYZPlaneOn();

    //设置线条显示或隐藏
    void setLinesEnabled(size_t _lineIndex, bool _b);


    //获取cchart类吗m_chart
    //CChart *getChart();

    /*--------------------------------------start--------------------------------------------*/
    /*以下public函数都是为了单独显示选定窗口来使用的*/
    //首先执行setCopyFromOtherOpenWidgetMark(false)来将该窗口类设置为复制类窗口
    void setCopyFromOtherOpenWidgetMark(GLboolean _b);
    //调用 getAllVaoAndVboAndVboIndex来提取所有的Opengl VAO和VBO索引号码。实际上是想在该上下文中提取出opengl缓冲区中数据
    void getAllVaoAndVboAndVboIndex();
    //getVaoAndVbos()并未被调用
    VaoVbos getVaoAndVbos();
    //setVaoAndVbosBuffer并未被调用
    void setVaoAndVbosBuffer(vector<GLfloat> &_anixVboBuffer, vector<GLuint> &_anixVboIndexBuffer);
    //getAnixVboBufferData,getAnixVboIndexBufferData函数可以不被调用,但必须保证坐标轴anix被正常绘制;
    vector<GLfloat>& getAnixVboBufferData();
    vector<GLuint>& getAnixVboIndexBufferData();
    //getLinesVboBufferData,在被复制类中调用，得到其缓冲区数据
    //setLinesVboBufferData,在复制类中调用，设置其缓冲区数据
    void getLinesVboBufferData(list<vector<GLfloat> > & _linesBuffer, vector<size_t> &_linesPointAmout);
    void setLinesVboBufferData(list<vector<GLfloat> > & _linesBuffer, vector<size_t> &_linesAmount);
    //getCordRangei,在被复制类中调用，得到其坐标轴上下限
    //setCordRangei,在复制类中调用，设置其坐标轴上下限
    void getCordRangei(vector2f &_x, vector2f &_y, vector2f &_z);
    void setCordRangei(vector2f &_x, vector2f &_y, vector2f &_z);
    //getRotation, getMove, 在被复制类中调用，得到其旋转、平移矩阵值
    //setRotation, setMove, 在复制类中调用，设置其旋转、平移矩阵值
    void getRotation(QMatrix4x4 & _rotation);
    void setRotation(QMatrix4x4 & _rotation);
    void getMove(QMatrix4x4 & _move);
    void setMove(QMatrix4x4 & _move);
    //setDrawType, 在复制类中调用，设定该窗口的显示类型
    //getRotation, 在被复制类中调用，得到该窗口的显示类型
    void setDrawType(CChart::DRAWTYPE _type);
    void getDrawType(CChart::DRAWTYPE & _type);
    //getGridNumer, 在被复制类中调用，得到该窗口的网格数
    //setGridNumer, 在复制类中调用，设定该窗口的网格数
    void getGridNumer(int & _xGridNumPerAnix, int & _yGridNumPerAnix, int & _zGridNumPerAnix);
    void setGridNumer(int _xGridNumPerAnix, int _yGridNumPerAnix, int _zGridNumPerAnix);
    //getLineColor, 在被复制类中调用，得到该窗口中线条的颜色
    //setLineColor, 在复制类中调用，设定该窗口中线条的颜色
    void getLineColor(map<u_short, vector4f> &_lineColor);
    void setLineColor(map<u_short, vector4f> &_lineColor);
    void updateLines();
    void drawOnce();
    /*-----------------------------------------------end------------------------------------------*/
    //检查坐标上下限是否变化，true是变化，false是无变化
    bool checkRangeChange(vector2f &_xRange, vector2f &_yRange, vector2f &_zRange);

protected:
    //初始化窗口，在paintGL执行前自行调用
    void initializeGL();
    //绘制窗口
	void paintGL();	

    /*-------当复制窗口时调用start------*/
    //anix
    void getAnixVao();
    void getAnixVbo();
    void getAnixVboIndex();
    //grid
    void getVaoGridAnixnumDgt();
    void getVboGridAnixnumDgt();
    void getVboGridAnixnumDgtTexture();

    void getVaoGridAnixnumPnt();
    void getVboGridAnixnumPnt();
    void getVboGridAnixnumPntTexture();

    void getVaoGridAnixnumMinus();
    void getVboGridAnixnumMinus();
    void getVboGridAnixnumMinusTexture();

    void getVaoGridAnixnumLabel();
    void getVboGridAnixnumLabel();
    void getVboGridAnixnumLabelTexture();
    //lines
    void getLinesVao();
    void getLinesVbo();
    /*-------当复制窗口时调用end------*/
private:

	CChart	m_chart;
    double  m_time = 0;
    float   m_zoom = 1.0;
	bool	m_leftPushDown = false;		//左键按下
	bool	m_rightPushDown = false;	//右键按下
	int		m_mouseCordx = 0;			//左键鼠标位置x			
	int		m_mouseCordy = 0;			//左键鼠标位置y
    int     m_drawtype = 0;
    CAllocVBOandVAO* m_allocVBOandVAO;
    VaoVbos m_vaoVbos;
    //非复制窗口标志, 当为复制窗口是为false
    GLboolean m_notCopyFromOtherOpenGLWidget = true;

    /*-------当复制窗口时调用start------*/
    vector<GLfloat> m_vboData;
    vector<GLuint>  m_vboIndexData;
    list<vector<GLfloat> > m_linesVboData;
    /*-------当复制窗口时调用end------*/

    u_char m_frameType = 0;

private slots:
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent* event);
    void onSetGridState();
    void onSetAnixState();
    void onXYPlaneState();
    void onXZPlaneState();
    void onYZPlaneState();

};
