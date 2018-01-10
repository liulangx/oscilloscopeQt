#pragma once
#include "Renders/CLinesRender.h"
#include "Renders/Crender.h"
#include "DataManageMent/CAllocVBOandVAO.h"

#include <QOpenGLFunctions_4_3_Core>
#include <QGLShader>
#include <QGLShaderProgram>
#include <list>

#include <QMessageBox>
//#include <GL/glu.h>

typedef float SPFLOAT;
class Crender;
struct vector2f
{
	SPFLOAT _x, _y;
};
struct vector3f
{
	SPFLOAT _x, _y, _z;
};
struct vector4f
{
	SPFLOAT _r, _g, _b, _a;
public:
    void operator =(vector4f b)
    {
        _r = b._r;
        _g = b._g;
        _b = b._b;
        _a = b._a;
    }
};
struct vector4b
{
	GLubyte _r, _g, _b, _a;
};

class Clines:private QOpenGLFunctions_4_3_Core
{
public:
	enum Attribute
	{
		LINE_POSITION,
		LINE_COLOUR,
		LINE_INDEX
	};
    Clines(CAllocVBOandVAO* allocVBOandVAO);			//必须获取一个渲染环境
	~Clines();
    void initial(CLinesRender* _render, size_t _pointsize, QOpenGLContext *_context);
    //setRenderParam 在initial执行后立刻调用，为了能在draw阶段使用geometry shader
    void setRenderParam(GLuint _uniformIndexXyPlaneMarkForLines, GLuint _uniformIndexXzPlaneMarkForLines, GLuint _uniformIndexYzPlaneMarkForLines, GLuint _uniformIndexXLowerForLines, GLuint _uniformIndexYLowerForLines, GLuint _uniformIndexZLowerForLines);
	void draw();
    void drawOnce();
	void clearUp();
    void setColor(vector4f color);
    vector4f getColor() const;
    void addLine(const vector3f& _posion, GLfloat _xLower, GLfloat _yLower, GLfloat _zLower);
    void setXYPlaneOn();
    void setXZPlaneOn();
    void setYZPlaneOn();

    /*---------复制窗口时调用start-----------*/
    void setLinesVboBufferData();
    GLuint getLinesVao();
    GLuint getLinesVbo();
    //lines
    size_t getPointsAmout() const;
    void setLinesVao(GLuint _linesVaos);
    void setLinesVbo(GLuint _linesVbos);
    void setLinesVboBufferData(vector<GLfloat> & _linesBuffer, size_t _linesAmount);
    void updateLines();
    void setLowers(GLfloat _xLower, GLfloat _yLower, GLfloat _zLower);
    /*---------复制窗口时调用end-----------*/
private:

    GLboolean  xyMark = true;
    GLboolean  xzMark = true;
    GLboolean  yzMark = true;

private:

    //Crender*					m_therender;
    CLinesRender*				m_linesRender;
	int							m_shadermin;					//最大点uniform
	int							m_shadermax;					//最小点uniform
private :
	GLuint						m_vao;
    vector4f                    m_color = {1, 0, 0, 1};
    GLuint						m_vbo;		//顶点数组
	size_t						m_pointCount = 0;		//记录顶点数据个数
	size_t						m_pointSize;
	size_t						m_persize;				//每一个内存段大小
    //size_t						m_draws = 0;			//已经画了的点
    CAllocVBOandVAO*            m_allocVBOandVAO;
    QOpenGLContext*             m_context;

    //线段在三个方向上的分量

    GLfloat                    m_xLower = 0.0f;
    GLfloat                    m_yLower = 0.0f;
    GLfloat                    m_zLower = 0.0f;

    GLfloat                     m_zeroGate = 0.00000001f;

    GLboolean                   m_XYPlaneOn = false;
    GLboolean                   m_XZPlaneOn = false;
    GLboolean                   m_YZPlaneOn = false;

    GLuint                      m_uniformIndexXyPlaneMarkForLines;
    GLuint                      m_uniformIndexXzPlaneMarkForLines;
    GLuint                      m_uniformIndexYzPlaneMarkForLines;

    GLuint                      m_uniformIndexXLowerForLines;
    GLuint                      m_uniformIndexYLowerForLines;
    GLuint                      m_uniformIndexZLowerForLines;

    //复制窗口时用到的数据
    GLboolean                   m_notCopyFromOtherOpenGLWidget = true;
    vector<GLfloat>             m_copyVboBuffer;        //复制的缓冲区数据
};

