#ifndef CGRID_H
#define CGRID_H
#include "textureindex.h"
#include "MyOpenglWidget/Clines.h"
#include "Renders/Crender.h"
#include "Renders/CGridRender.h"
#include "DataManageMent/CAllocVBOandVAO.h"
#include <math.h>
#include <vector>
#include <QOpenGLFunctions_4_3_Core>
#include <QGLShader>
#include <QGLShaderProgram>
#include <QOpenGLTexture>

using namespace std;
class CGrid : protected QOpenGLFunctions_4_3_Core
{
public:                 //functional functions
    enum RangeChange {
        NotChange,
        Change
    };
    enum Range{
        xRange,
        yRange,
        zRange
    };
    enum DRAWTYPE {
        XZY = 0,
        XY,
        XZ,
        YZ
    };
    CGrid();
    ~CGrid();
    void setPrecision(GLint precision);
    void setFontSize(GLfloat _fontSize = 6.0f);
    void setGridColor(vector4f color);
    bool setRot(QMatrix4x4 _rot);
    void setGridOff();
    void setGridOn();
    void setAnixLabelOff();
    void setAnixLabelOn();
    void createAndUpdateAnixnumberWithPnt();
    void setDrawType(int drawtype);
public:                  //functions must be called
    void initial(Crender* _render, CGridRender *_gridRender, QOpenGLContext* context, QSurface* surface);
    void setAlloc(CAllocVBOandVAO* allocVBOandVAO);     //this function should be called at first, for memory management
    void draw();
    void setCordi(vector2f  _x, vector2f _y, vector2f _z);
    void setCordiAlone(vector2f  _x, vector2f _y, vector2f _z);
    void getGridNumber(int & _xGridNumPerAnix, int & _yGridNumPerAnix, int & _zGridNumPerAnix);
    void setGridNumber(int _xGridNumPerAnix, int _yGridNumPerAnix, int _zGridNumPerAnix);
    void adjustGridNumber(vector2f _x, vector2f _y, vector2f _z, RangeChange xChange, RangeChange yChange, RangeChange zChange);
public:
    /*---------复制窗口时调用start-----------*/
    void setCopyFromOtherOpenWidgetMark(GLboolean _b);
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
    void getCordRangei(vector2f &_x, vector2f &_y, vector2f &_z);
    void setCordRangei(vector2f &_x, vector2f &_y, vector2f &_z);
    /*---------复制窗口时调用end-----------*/
private:
    void drawGrid();
    void drawAnixNumber();
    void drawAnixPoint();
    void drawAnixMinus();
    void drawAnixLabel();
    /*getAnxiIntRange:
     * 函数描述：将网格坐标归为整数
     * 根据更改后的坐标范围[m_*range._y, m_*range._x]来调整网格数
     * PS：后期经过更改，去除了边界上的上边界和下边界的判断，即让MinUpper == Minf, MaxLower == Maxf.
     * 参数描述：
     * Maxf: 坐标上限
     * Minf: 坐标下限
     * MaxLower: 坐标上限 第一位非0整数部分
     * MinUpper: 坐标下限 第一位非0整数部分+1
     * front: MinUpper归一化到[0,1]
     * back:  MaxLower归一化到[0,1]
     */
    bool getAnxiIntRange(GLfloat Maxf, GLfloat Minf, GLfloat &MaxLower, GLfloat &MinUpper, GLfloat &front, GLfloat &back);
    bool getAnxiIntRange(GLfloat Maxf, GLfloat Minf, GLfloat &MaxLower, GLfloat &MinUpper);

    /* createAndUpdateGrid:
     * 函数描述： 调用createAndUpdateGrid(), createGrid();
     */
    void createAndUpdateGrid();
    /* createGrid:
     * 函数描述： 创建网格数组，包含坐标和颜色;
     *          初始化和每次调整坐标范围后会调用该函数
     */
    void createGrid();
    /* end */

    /* createGrid:
     * 函数描述：创建缓冲区(动态的)，包含坐标和颜色
     *          初始化和每次调整坐标范围后会调用该函数
     */
    void updataGrid();
    /* end */

    /*adjustGridNumber:
     * 函数描述：根据更改后的坐标范围[m_*range._y, m_*range._x]来调整网格数
     * 参数描述：anixGridNumberPerAnix: 需要更改的m_*GridNumPerAnix
     *         range: 确定的坐标类型的参数
     */
    bool adjustGridNumber(Range range, int &anixGridNumberPerAnix);
    /* end */

    /* createAndUpdateAnixnumberWithPnt:
     * 函数描述：1. 根据更改后的坐标范围[m_*range._y, m_*range._x]来调整坐标值, 通过顺序调用
     * createAnixnumber(), updateAnixnumber(), updateAnixDecimalPnt()实现;
     *         2. adjustGridNumber()中会调用
     * 参数描述：NULL;
     */
    void createAnixnumber();            //调用addAnxisNumber
    void updateAnixnumber();
    void updateAnixDecimalPnt();
    void updateAnixMinus();
    void updateAnixLabel();
    /* end */
    GLboolean getIntvaluePointposOffsetForXAnix(GLfloat xAnix, GLint& xAnixValue, GLfloat& xAnixOffset, GLboolean& greaterThanOneMark);
    GLboolean getIntvaluePointposOffsetForYAnix(GLfloat yAnix, GLint& yAnixValue, GLfloat& yAnixOffset, GLboolean& greaterThanOneMark);
    GLboolean getIntvaluePointposOffsetForZAnix(GLfloat zAnix, GLint& zAnixValue, GLfloat& zAnixOffset, GLboolean& greaterThanOneMark);
    void addAnxisNumber(GLfloat anixNumber, Range range, GLfloat anixOffset, GLfloat size);
    void addAxisLabel(Range range, GLfloat axisOffset, GLfloat size);
    //
    void clearGridVector();

private:
    Crender*					m_therender;

private:
    DRAWTYPE                    m_drawtype = DRAWTYPE::XZY;
    CAllocVBOandVAO*            m_allocVBOandVAO;       //must be set at first;
    QOpenGLContext*             m_context;
    QSurface*                   m_surface;
    vector2f                    m_x = {0,1};			//坐标X, 该参数与坐标轴有关
    vector2f                    m_y = {0,1};			//坐标Y, 该参数与坐标轴有关
    vector2f                    m_z = {0,1};			//坐标Z, 该参数与坐标轴有关
    vector2f                    m_xRange = {0.01,-0.01};			//坐标XRange
    vector2f                    m_yRange = {0.01,-0.01};			//坐标YRange
    vector2f                    m_zRange = {0.01,-0.01};			//坐标ZRange
    bool                        m_isFirstRange = true;
    QMatrix4x4                  m_rot = {
                                    1.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f,
                                };

    //建立网格
    vector<float>               m_gridPositions;
    int                         m_xGridNumPerAnix = 4;     //x !!!Note: 生成缓冲区时应该注意当网格点数增加后，可能会导致缓冲区超界
    int                         m_yGridNumPerAnix = 4;     //y
    int                         m_zGridNumPerAnix = 4;     //z
    vector4f                    m_gridColor = {200/255.0f,200/255.0f,200/255.0f,1};
    GLfloat                     m_viewScale = 1.0f;//0.7;
    GLuint                      m_vaoGrid;
    GLuint                      m_vboGrid;
    //建立坐标值
    QOpenGLTexture*             m_textureDgt;                 //数字贴图
    QOpenGLTexture*             m_texturePnt;                 //小数点贴图
    QOpenGLTexture*             m_textureMinus;                 //负号贴图
    QOpenGLTexture*             m_textureLabel;

    GLuint                      m_vaoGridAnixnumDgt;        //数字VAO
    GLuint                      m_vboGridAnixnumDgt;        //VBO
    GLuint                      m_vboGridAnixnumDgtTexture;   //texture vbo
    vector<float>               m_gridAnixnumDgtPositions;  //数字位置容器
    vector<float>               m_gridAnixnumDgtTexture;  //数字Texture位置容器
    GLint                       m_DigPositionSize = -1;

    GLuint                      m_vaoGridAnixnumPnt;        //小数点VAO
    GLuint                      m_vboGridAnixnumPnt;        //VBO
    GLuint                      m_vboGridAnixnumPntTexture;   //texture vbo
    vector<float>               m_gridAnixnumPntPositions;  //小数点位置容器
    vector<float>               m_gridAnixnumPntTexture;  //小数点Texture位置容器
    GLint                       m_PntPositionSize = -1;

    GLuint                      m_vaoGridAnixnumMinus;        //负号VAO
    GLuint                      m_vboGridAnixnumMinus;        //VBO
    GLuint                      m_vboGridAnixnumMinusTexture;   //texture vbo
    vector<float>               m_gridAnixnumMinusPositions;  //负号位置容器
    vector<float>               m_gridAnixnumMinusTexture;  //负号Texture位置容器
    GLint                       m_MinusPositionSize = -1;

    GLuint                      m_vaoGridAnixnumLabel;        //"xyz/"
    GLuint                      m_vboGridAnixnumLabel;        //VBO
    GLuint                      m_vboGridAnixnumLabelTexture;   //texture vbo
    vector<float>               m_gridAnixnumLabelPositions;  //"xyz/"位置容器
    vector<float>               m_gridAnixnumLabelTexture;  //"xyz/"Texture位置容器
    GLint                       m_LabelPositionSize = -1;



    CGridRender*                 m_gridRender;      //网格的坐标值，xyz符号由该人render渲染，由外部传入

    vector3f                    m_xAnixBase = {1, 0, 0};
    vector3f                    m_yAnixBase = {1, 0, 0};
    GLfloat                     m_fontSize = 0.10;
    GLfloat                     m_xSize = 0.06; //0.01
    GLfloat                     m_xOffsetBits = 0;      //x坐标在x轴方向的最大偏移位数
    GLfloat                     m_ySize = 0.06; //0.01
    GLfloat                     m_yOffsetBits = 0;      //y坐标在x轴方向的最大偏移位数
    GLfloat                     m_zSize = 0.06; //0.01
    GLfloat                     m_offset = 0.02;
    vector4f                    m_color = {51/255.0f,221/255.0f,255/255.0f,1};
    GLint                       m_precision = 3;
    vector<GLuint>              m_anixNumber;
    GLint                       m_tensNumber;
    //开关
    GLboolean                   m_gridOn = true;
    GLboolean                   m_anixLabelOn = true;
    GLboolean                   m_anixLabelNotEmpty = true;
    //PosisitonChange
    GLfloat                     m_zOffsetForDrawType = 0; // x,z

    GLboolean                   m_notCopyFromOtherOpenGLWidget = true;

};

#endif // CGRID_H
