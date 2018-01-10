#pragma once
#include <QOpenGLFunctions_4_3_Core>
#include <QGLShader>
#include <QGLShaderProgram>
#include <QPainter>
#include "MyOpenglWidget/Clines.h"
#include "Renders/Crender.h"
#include "DataManageMent/CAllocVBOandVAO.h"

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include <vector>
using namespace std;
class CAnix : protected QOpenGLFunctions_4_3_Core
{
public:
    void initial(Crender* _render, QOpenGLContext* context, QSurface* surface);
	void draw();
	void clearUp();
	void setCordi(vector2f  _x, vector2f _y, vector2f _z);
    void setCordiAlone(vector2f  _x, vector2f _y, vector2f _z);
	vector2f getXCord();
	vector2f getYCord();
	vector2f getZCord();
	CAnix();
	~CAnix();
    void setAlloc(CAllocVBOandVAO* allocVBOandVAO);
public:
    /*---------复制窗口时调用start-----------*/
    void setCopyFromOtherOpenWidgetMark(GLboolean _b);
    GLuint getAnixVao();
    GLuint getAnixVbo();
    GLuint getAnixVboIndex();
    //Anix
    void setAnixVboBuffer(vector<GLfloat> &_anixVboBuffer);
    void setAnixVboIndexBuffer(vector<GLuint>& _anixVboIndexBuffer);
    void copyBufferFromOthers();
    void setCordiAlone();
    /*---------复制窗口时调用end-----------*/
private:
    void genCord();
	void updataCord();
	void genAnix();						//生成坐标轴和箭头buffer(参数为坐标线位置)
private:
	Crender*					m_therender;

private:
    QOpenGLContext*             m_context;
    QSurface*                   m_surface;
    vector2f                    m_x = {0,1};			//坐标X
    vector2f                    m_y = {0,1};			//坐标Y
    vector2f                    m_z = {0,1};			//坐标Z
	vector2f					m_arrowbase = {0.01f,0.05f};		//箭头底半径 箭头长度 
	vector<GLuint>				m_arrowIndex;					//箭头检索
	vector<float>				m_arrowpoint;					//箭头的点
    vector4f					m_xcolor = {1, 0, 0, 1};//{45/255.0f,57/255.0f,227/255.0f,1};						//x轴颜色
    vector4f					m_ycolor = {0, 1, 0, 1};//{45/255.0f,57/255.0f,227/255.0f,1};							//y轴颜色
    vector4f					m_zcolor = {0, 0, 1, 1};//{45/255.0f,57/255.0f,227/255.0f,1};	   					//z轴颜色
	float*						m_arrow;						//线段的点
    CAllocVBOandVAO*            m_allocVBOandVAO;
private:
	GLuint						m_vao;
	GLuint						m_vbo;
	GLuint						m_vboIndex;						//索引
    GLuint                      m_copyVao = -1;
    GLuint                      m_copyVbo = -1;
    GLuint                      m_copyVboIndex = -1;
    GLboolean                   m_notCopyFromOtherOpenGLWidget = true;
    GLboolean                   m_copyCompleted = false;
    vector<GLfloat>             m_vboBuffer;
    vector<GLuint>              m_vboIndexBuffer;
};

