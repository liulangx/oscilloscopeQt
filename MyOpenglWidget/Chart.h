#pragma once
#define u_short ushort
#include "MyOpenglWidget/Clines.h"
#include "DataManageMent/CAllocVBOandVAO.h"
#include "Renders/Crender.h"
#include "Renders/CGridRender.h"
#include "Renders/CLinesRender.h"

#include "GridAndAxis/Anix.h"
#include "GridAndAxis/CGrid.h"

#include <QOpenGLFunctions_4_3_Core>
#include <QGLShader>
#include <QGLShaderProgram>
#include <QtMath>
#include <vector>

using namespace std;
class CChart : protected QOpenGLFunctions_4_3_Core
{
public:
	enum DRAWTYPE {
		XZY = 0,
		XY,
		XZ,
		YZ
	};
    CChart();
    ~CChart();
    void initial(vector3f _location, QOpenGLContext* _context, QSurface* _surface);

    void    draw();
    void    clearUp();
    size_t	addLine(size_t _lineIndex, size_t _pointsize);
    bool addPoint(size_t _lineindex, const vector3f& _position/*, QOpenGLContext *_context*/);
    void	updateChart();			//????chart
    void    setAlloc(CAllocVBOandVAO* allocVBOandVAO);
    void setRot(float _x, float _y/*, float _z*/);			//旋转
    void setRotAlone(float _x, float _y, float _z);
//	void setXrange(float _max, float _min);
//	void setYrange(float _max, float _min);
//	void setZrange(float _max, float _min);
	void setZoom(float _zoom);
    void setMove(float _x,float _y);					//平移
	void setDrawType(DRAWTYPE _type);
    void setColor(size_t _lineIndex, vector4f _color);
    void setGridOnOrOff();
    void setGridOnOrOff(bool _b);
    void setAnixLabelOnOrOff();
    void setAxisPrecision(GLint _precision);

    void setLineXYPlaneOn(/*GLboolean _b*/);
    void setLineXZPlaneOn();
    void setLineYZPlaneOn();

    void setWidthAndHeight(size_t width, size_t height);

    void setFontSize(GLfloat _fontSize = 6.0f);

    /*---------复制窗口时调用start-----------*/
    void setCopyFromOtherOpenWidgetMark(GLboolean _b);
    //Anix
    GLuint getAnixVao();
    GLuint getAnixVbo();
    GLuint getAnixVboIndex();
    //Grid
    GLuint getVaoGridAnixnumDgt();
    GLuint getVboGridAnixnumDgt();
    GLuint getVboGridAnixnumDgtTexture();

    GLuint getVaoGridAnixnumPnt();
    GLuint getVboGridAnixnumPnt();
    GLuint getVboGridAnixnumPntTexture();

    GLuint getVaoGridAnixnumMinus();
    GLuint getVboGridAnixnumMinus();
    GLuint getVboGridAnixnumMinusTexture();

    GLuint getVaoGridAnixnumLabel();
    GLuint getVboGridAnixnumLabel();
    GLuint getVboGridAnixnumLabelTexture();
    //lines
    list<GLuint> getLinesVao();
    list<GLuint> getLinesVbo();

    //Anix
    void setAnixVboBuffer(vector<GLfloat> &_anixVboBuffer);
    void setAnixVboIndexBuffer(vector<GLuint> &_anixVboIndexBuffer);
    //Grid
    void setVaoGridAnixnumDgt(GLuint _gridVaoGridAnixnumDgt);
    void setVboGridAnixnumDgt(GLuint _gridVboGridAnixnumDgt);
    void setVboGridAnixnumDgtTexture(GLuint _gridVboGridAnixnumDgtTexture);

    void setVaoGridAnixnumPnt(GLuint _gridVaoGridAnixnumPnt);
    void setVboGridAnixnumPnt(GLuint _gridVboGridAnixnumPnt);
    void setVboGridAnixnumPntTexture(GLuint _gridVboGridAnixnumPntTexture);

    void setVaoGridAnixnumMinus(GLuint _gridVaoGridAnixnumMinus);
    void setVboGridAnixnumMinus(GLuint _gridVboGridAnixnumMinus);
    void setVboGridAnixnumMinusTexture(GLuint _gridVboGridAnixnumMinusTexture);

    void setVaoGridAnixnumLabel(GLuint _gridVaoGridAnixnumLabel);
    void setVboGridAnixnumLabel(GLuint _gridVboGridAnixnumLabel);
    void setVboGridAnixnumLabelTexture(GLuint _gridVboGridAnixnumLabelTexture);
    //lines
    void setLinesVao(list<GLuint> _linesVaos);
    void setLinesVbo(list<GLuint> _linesVbos);

    void getCordRangei(vector2f &_x, vector2f &_y, vector2f &_z);
    void setCordRangei(vector2f &_x, vector2f &_y, vector2f &_z);
    void getRotation(QMatrix4x4 & _rotation);
    void setRotation(QMatrix4x4 & _rotation);
    void getMove(QMatrix4x4 & _move);
    void setMove(QMatrix4x4 & _move);
    void setDrawTypeAlone(CChart::DRAWTYPE &_type);
    //void setDrawTypeFo(CChart::DRAWTYPE &_type);
    void getDrawType(DRAWTYPE & _type);
    void getGridNumer(int & _xGridNumPerAnix, int & _yGridNumPerAnix, int & _zGridNumPerAnix);
    void setGridNumer(int _xGridNumPerAnix, int _yGridNumPerAnix, int _zGridNumPerAnix);
    void setLinesVboBufferData(list<vector<GLfloat> > & _linesBuffer, vector<size_t> &_linesAmount);
    map<u_short, vector4f> getLineColor();
    void setLineColor(map<u_short, vector4f> &_lineColors);
    vector<size_t> getLinesPointsAmount();

    void updateLines();
    /*---------复制窗口时调用end-----------*/

    bool checkRangeChange(vector2f& _xRange, vector2f& _yRange, vector2f& _zRange);

    void setLinesEnabled(size_t _lineIndex, bool _b);
protected:
	void setDrawAnx();
    void setDrawLine(vector4f &_lineColor);
    DRAWTYPE            m_drawtype = XZY;
    float               m_zoom = 1;
    QMatrix4x4          m_rotation = {
                                1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 0, 1
                            };
    vector2f            m_xRange = {1,0};
    vector2f            m_yRange = {1,0};
    vector2f            m_zRange = {1,0};
    CGrid::RangeChange  m_xRanChange = CGrid::RangeChange::NotChange;
    CGrid::RangeChange  m_yRanChange = CGrid::RangeChange::NotChange;
    CGrid::RangeChange  m_zRanChange = CGrid::RangeChange::NotChange;

    QMatrix4x4          m_move = {
                                1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 0, 1
                            };
    vector3f            m_basecord = {0.0f, 0.0f, 0.0f};					//被遗弃了

    QMatrix4x4          m_projection = {
                                        1, 0, 0, 0,
                                        0, 1, 0, 0,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1
                                    };
    float               m_viewLineFactor = 1.0f;
    vector4f            m_lineColor = {1.0f, 0.0f, 0.0f, 1.0f};

    map<size_t, int>      m_isFirstPoint;
    //vector<int>*        m_isFirstPoint = new vector<int>;



private:
    CGrid                       m_grid;
    GLboolean                   m_gridOn = true;
    GLboolean                   m_anixLabelOn = true;
    QSurface*                   m_surface;
    QOpenGLContext*             m_context;
	bool						m_isfirstdata;
	vector3f					m_cordOrigin = {0,0,0};					//???????
	Crender 					m_therender;
    CGridRender                 m_gridRender;
    CLinesRender                m_linesRender;

    CAnix						m_anix;
    map<size_t, Clines*>        m_lines;
    map<size_t, bool>           m_linesEnabled;
    //vector<Clines*>				m_lines;
	vector3f					m_scale = { 0,0,0 };
	vector3f					m_maxpoints = { 0,0,0 };		//????��?????(????????)
	vector3f					m_minpoints = { 0,0,0 };		//????��???��??(????????)
	bool						m_scalechange = false;
	int							m_shadermin;					//????uniform
	int							m_shadermax;					//??��??uniform
	GLuint						m_vao;
	GLuint						m_lineBuffer;		//????????
    CAllocVBOandVAO*            m_allocVBOandVAO;

protected:
    //CRender
    GLuint						m_uniformIndexChosePntAnixGrid;
    GLuint						m_uniformIndexRangeXPntAnixGrid;
    GLuint						m_uniformIndexRangeYPntAnixGrid;
    GLuint						m_uniformIndexRangeZPntAnixGrid;
    GLuint						m_uniformIndexScalePntAnixGrid;
    GLuint						m_uniformIndexRotPntAnixGrid;
    GLuint						m_uniformIndexMovePntAnixGrid;			//???????????
    GLuint						m_uniformIndexBasecordPntAnixGrid;
    GLuint						m_uniformIndexDrawtypePntAnixGrid;		//?????????
    GLuint                      m_uniformIndexProjectionPntAnixGrid;
    GLuint                      m_uniformIndexViewLinefactorPntAnixGrid;
    GLuint                      m_uniformIndexLineColorPntAnixGrid;   
    //CLinesRender
    GLuint						m_uniformIndexChosePntAnixGridForLines;
    GLuint						m_uniformIndexRangeXPntAnixGridForLines;
    GLuint						m_uniformIndexRangeYPntAnixGridForLines;
    GLuint						m_uniformIndexRangeZPntAnixGridForLines;
    GLuint						m_uniformIndexScalePntAnixGridForLines;
    GLuint						m_uniformIndexRotPntAnixGridForLines;
    GLuint						m_uniformIndexMovePntAnixGridForLines;			//???????????
    GLuint						m_uniformIndexBasecordPntAnixGridForLines;
    GLuint						m_uniformIndexDrawtypePntAnixGridForLines;		//?????????
    GLuint                      m_uniformIndexProjectionPntAnixGridForLines;
    GLuint                      m_uniformIndexViewLinefactorPntAnixGridForLines;
    GLuint                      m_uniformIndexLineColorPntAnixGridForLines;
    GLuint                      m_uniformIndexXyPlaneMarkForLines;
    GLuint                      m_uniformIndexXzPlaneMarkForLines;
    GLuint                      m_uniformIndexYzPlaneMarkForLines;
    GLuint                      m_uniformIndexXLowerForLines;
    GLuint                      m_uniformIndexYLowerForLines;
    GLuint                      m_uniformIndexZLowerForLines;
    //GridRender for texture;
    GLuint                      m_uniformIndexScaleAnixtexture;
    GLuint                      m_uniformIndexMoveAnixtexture;
    GLuint                      m_uniformIndexBasecordAnixtexture;
    GLuint                      m_uniformIndexProjectionAnixtexture;
    GLuint                      m_uniformIndexRotAnixtexture;
    GLuint                      m_uniformIndexDrawtypeAnixtexture;
    GLuint                      m_uniformIndexRangeXAnixtexture;
    GLuint                      m_uniformIndexRangeYAnixtexture;
    GLuint                      m_uniformIndexRangeZAnixtexture;
//    GLuint                      m_uniformIndexRotAnixtexture;

	bool						m_stataChange=true;		//????????
    bool                        m_RangeChange = false;
    GLboolean                   m_notCopyFromOtherOpenGLWidget = true;
    list<GLuint>                m_lineVaos;
    list<GLuint>                m_lineVbos;
    //GLboolean                   m_isFirstData = true;
    GLboolean                   m_first2Two = false;
    GLfloat                     sdfsfsfsd = 1/ 3.0f;
    size_t                      m_width;
    size_t                      m_height;
};

