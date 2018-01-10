#include "GridAndAxis/CGrid.h"
//设置的网格数最多的情况
const float defaultMaxGridNumber = 10;
//判断是否为0的阈值
const GLfloat zeroGate = 0.0000001;
//圆周率
const float pi = 3.1415926f;
//小数点对于数字在贴图坐标x、y上的倍率，如小数点为数字高度的xPointScale倍，宽度的yPointScale倍
const float xPointScale = 0.40f;
const float yPointScale = 0.40f;
//x\y\z轴偏离轴线倍率
const GLfloat xAnixOffsetRate = 1.1;
const GLfloat yAnixOffsetRate = 1.2;
const GLfloat zAnixOffsetRate = 1.1;
void CGrid::initial(Crender *_render, CGridRender *_gridRender ,QOpenGLContext *context, QSurface *surface)
{
    //glClear(GL_CLEAR_BUFFER);
    initializeOpenGLFunctions();
    //将该grid的拥有类widget类的上下文传递下来，并且makecurrent！！！
    m_context = context;
    m_surface = surface;
    m_therender = _render;
    m_gridRender = _gridRender;
    m_context->makeCurrent(m_context->surface());
    //初始化纹理贴图
    //载入数字纹理贴图
    m_textureDgt = new QOpenGLTexture(QImage("Img/digit.png").mirrored());
    m_textureDgt->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);//LinearMipMapLinear);
    m_textureDgt->setMagnificationFilter(QOpenGLTexture::Linear);
    m_textureDgt->setWrapMode(QOpenGLTexture::Repeat);
    //载入小数点纹理贴图
    m_texturePnt = new QOpenGLTexture(QImage("Img/decimal_point.png").mirrored());
    m_texturePnt->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);//LinearMipMapLinear);
    m_texturePnt->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texturePnt->setWrapMode(QOpenGLTexture::Repeat);
    //载入负号纹理贴图
    m_textureMinus = new QOpenGLTexture(QImage("Img/minus.png").mirrored());
    m_textureMinus->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);//LinearMipMapLinear);
    m_textureMinus->setMagnificationFilter(QOpenGLTexture::Linear);
    m_textureMinus->setWrapMode(QOpenGLTexture::Repeat);

    //载入负号纹理贴图
    m_textureLabel = new QOpenGLTexture(QImage("Img/labels.png").mirrored());
    m_textureLabel->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);//LinearMipMapLinear);
    m_textureLabel->setMagnificationFilter(QOpenGLTexture::Linear);
    m_textureLabel->setWrapMode(QOpenGLTexture::Repeat);

//    if(m_notCopyFromOtherOpenGLWidget)
//    {
        //申请网格的vao和vbo
        m_vaoGrid = m_allocVBOandVAO->allocVAO();
        m_vboGrid = m_allocVBOandVAO->allocVBO();
        //m_gridRender->prepare();

        //申请数字的vao和vbo
        m_vaoGridAnixnumDgt = m_allocVBOandVAO->allocVAO();
        m_vboGridAnixnumDgt = m_allocVBOandVAO->allocVBO();
        m_vboGridAnixnumDgtTexture =  m_allocVBOandVAO->allocVBO();

        //申请小数点的vao和vbo
        m_vaoGridAnixnumPnt = m_allocVBOandVAO->allocVAO();
        m_vboGridAnixnumPnt = m_allocVBOandVAO->allocVBO();
        m_vboGridAnixnumPntTexture =  m_allocVBOandVAO->allocVBO();

        //申请负号的vao和vbo
        m_vaoGridAnixnumMinus = m_allocVBOandVAO->allocVAO();
        m_vboGridAnixnumMinus = m_allocVBOandVAO->allocVBO();
        m_vboGridAnixnumMinusTexture =  m_allocVBOandVAO->allocVBO();

        //申请Label的vao和vbo
        m_vaoGridAnixnumLabel = m_allocVBOandVAO->allocVAO();
        m_vboGridAnixnumLabel = m_allocVBOandVAO->allocVBO();
        m_vboGridAnixnumLabelTexture =  m_allocVBOandVAO->allocVBO();
//    }

    createAndUpdateGrid();
    createAndUpdateAnixnumberWithPnt();
}

void CGrid::setPrecision(GLint precision)
{
    m_precision = precision;
    createAndUpdateAnixnumberWithPnt();
}

void CGrid::setFontSize(GLfloat _fontSize)
{
    m_fontSize = _fontSize / 100.0f;
}

void CGrid::setGridColor(vector4f color)
{
    m_gridColor = {color._r, color._g, color._b, color._a};
}

bool CGrid::setRot(QMatrix4x4 _rot)
{
    m_rot = _rot.inverted();
    return true;
}

void CGrid::setGridOff()
{
    m_gridOn = false;
//    if(m_notCopyFromOtherOpenGLWidget)
//    {
//        createAndUpdateGrid();
//    }
//    else
//    {

//    }
    createAndUpdateGrid();
}

void CGrid::setGridOn()
{
    m_gridOn = true;
//    if(m_notCopyFromOtherOpenGLWidget)
//    {
//        createAndUpdateGrid();
//    }
//    else
//    {

//    }
    createAndUpdateGrid();
}

void CGrid::setAnixLabelOff()
{
    m_anixLabelOn = false;
    createAndUpdateAnixnumberWithPnt();
}

void CGrid::setAnixLabelOn()
{
    m_anixLabelOn = true;
    createAndUpdateAnixnumberWithPnt();
}

void CGrid::draw()
{
    m_context->makeCurrent(m_context->surface());
    m_therender->bindShader();
    drawGrid();
    //启用网格绘制shader类
    m_therender->releaseShader();
    m_gridRender->bindShader();
    drawAnixNumber();
    drawAnixPoint();
    drawAnixMinus();
    drawAnixLabel();
    m_gridRender->releaseShader();
    //m_therender->bindShader();
}

void CGrid::drawGrid()
{
    //绑定网格VAO
    glBindVertexArray(m_vaoGrid);
    //设定网格线宽为0.5
    glLineWidth(0.5);
    //画线
    glDrawArrays(GL_LINES, 0, m_gridPositions.size() / 7);  //位置和颜色
    glBindVertexArray(0);
}
//画坐标值
void CGrid::drawAnixNumber()
{
    //绑定数字VAO
    glBindVertexArray(m_vaoGridAnixnumDgt);
    //绑定数字贴图
    m_textureDgt->bind(0);
    //启用位置和纹理属性，分别为0和1
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    //画四边形
    glDrawArrays(GL_QUADS, 0, m_gridAnixnumDgtPositions.size() / 3);    //位置
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    m_textureDgt->release();
    glBindVertexArray(0);
}

void CGrid::drawAnixPoint()
{
    //绑定小数点VAO
    glBindVertexArray(m_vaoGridAnixnumPnt);
    //绑定小数点贴图
    m_texturePnt->bind(0);
    //启用位置和纹理属性，分别为0和1
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    //画四边形
    glDrawArrays(GL_QUADS, 0, m_gridAnixnumPntPositions.size() / 3);    //位置
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    m_texturePnt->release();
    glBindVertexArray(0);
}

void CGrid::drawAnixMinus()
{
    //绑定MinusVAO
    glBindVertexArray(m_vaoGridAnixnumMinus);
    //绑定Minus贴图
    m_textureMinus->bind(0);
    //启用位置和纹理属性，分别为0和1
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    //画四边形
    glDrawArrays(GL_QUADS, 0, m_gridAnixnumMinusPositions.size() / 3);    //位置
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    m_textureMinus->release();
    glBindVertexArray(0);
}

void CGrid::drawAnixLabel()
{
    //绑定LabelVAO
    glBindVertexArray(m_vaoGridAnixnumLabel);
    //绑定Label贴图
    m_textureLabel->bind(0);
    //启用位置和纹理属性，分别为0和1
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    //画四边形
    glDrawArrays(GL_QUADS, 0, m_gridAnixnumLabelPositions.size() / 3);    //位置
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    m_textureLabel->release();
    glBindVertexArray(0);
}

CGrid::CGrid()
{

}

CGrid::~CGrid()
{
    //执行opengl资源删除时，必须为当前的上下文环境，所以必须执行，makecurrent操作
    m_context->makeCurrent(m_context->surface());
    delete m_textureDgt;
    delete m_texturePnt;
    delete m_textureMinus;
    delete m_textureLabel;
}

void CGrid::setAlloc(CAllocVBOandVAO *allocVBOandVAO)
{
    m_allocVBOandVAO = allocVBOandVAO;
}

void CGrid::setCordi(vector2f _x, vector2f _y, vector2f _z)
{
    m_x._x = 0;
    m_x._y = _x._y - _x._x;
    m_y._x = 0;
    m_y._y = _y._y - _y._x;
    m_z._x = 0;
    m_z._y = _z._y - _z._x;
    createAndUpdateGrid();
    createAndUpdateAnixnumberWithPnt();
}

void CGrid::setCordiAlone(vector2f _x, vector2f _y, vector2f _z)
{
    m_x._x = 0;
    m_x._y = _x._y - _x._x;
    m_y._x = 0;
    m_y._y = _y._y - _y._x;
    m_z._x = 0;
    m_z._y = _z._y - _z._x;
}

void CGrid::getGridNumber(int &_xGridNumPerAnix, int &_yGridNumPerAnix, int &_zGridNumPerAnix)
{
    _xGridNumPerAnix = m_xGridNumPerAnix;
    _yGridNumPerAnix = m_yGridNumPerAnix;
    _zGridNumPerAnix = m_zGridNumPerAnix;
}

void CGrid::setGridNumber(int _xGridNumPerAnix, int _yGridNumPerAnix, int _zGridNumPerAnix)
{
    m_xGridNumPerAnix = _xGridNumPerAnix;
    m_yGridNumPerAnix = _yGridNumPerAnix;
    m_zGridNumPerAnix = _zGridNumPerAnix;
}

void CGrid::getCordRangei(vector2f& _x, vector2f& _y, vector2f& _z)
{
    _x._x = m_xRange._x;
    _x._y = m_xRange._y;
    _y._x = m_yRange._x;
    _y._y = m_yRange._y;
    _z._x = m_zRange._x;
    _z._y = m_zRange._y;
}

void CGrid::setCordRangei(vector2f &_x, vector2f &_y, vector2f &_z)
{
    m_xRange._x = _x._x;
    m_xRange._y = _x._y;
    adjustGridNumber(Range::xRange, m_xGridNumPerAnix);
    //adjustXGrid();
    m_yRange._x = _y._x;
    m_yRange._y = _y._y;
    adjustGridNumber(Range::yRange, m_yGridNumPerAnix);
    //adjustYGrid();
    m_zRange._x = _z._x;
    m_zRange._y = _z._y;
    adjustGridNumber(Range::zRange, m_zGridNumPerAnix);
}

void CGrid::adjustGridNumber(vector2f _x, vector2f _y, vector2f _z, RangeChange xChange, RangeChange yChange, RangeChange zChange)
{
    bool isSuceess = false;
    if(m_isFirstRange)
    {
        m_xRange._x = _x._x;
        m_xRange._x = _x._y;
        isSuceess = adjustGridNumber(Range::xRange, m_xGridNumPerAnix);
        if(!isSuceess)
            return;
        //adjustXGrid();
        m_yRange._x = _y._x;
        m_yRange._y = _y._y;
        isSuceess = adjustGridNumber(Range::yRange, m_yGridNumPerAnix);
        if(!isSuceess)
            return;
        //adjustYGrid();
        m_zRange._x = _z._x;
        m_zRange._y = _z._y;
        isSuceess = adjustGridNumber(Range::zRange, m_zGridNumPerAnix);
        if(!isSuceess)
            return;
        //adjustZGrid();
        m_isFirstRange = false;
    }
    if(RangeChange::Change == xChange)
    {
        m_xRange._x = _x._x;
        m_xRange._y = _x._y;
        //qDebug() << "[" << _x._x <<", "<< _x._y << "]";
        isSuceess = adjustGridNumber(Range::xRange, m_xGridNumPerAnix);
        if(!isSuceess)
            return;
        //adjustXGrid();
    }
    if(RangeChange::Change == yChange)
    {
        m_yRange._x = _y._x;
        m_yRange._y = _y._y;
        isSuceess = adjustGridNumber(Range::yRange, m_yGridNumPerAnix);
        if(!isSuceess)
            return;
        //adjustYGrid();
    }
    if(RangeChange::Change == zChange)
    {
        m_zRange._x = _z._x;
        m_zRange._y = _z._y;
        isSuceess = adjustGridNumber(Range::zRange, m_zGridNumPerAnix);
        if(!isSuceess)
            return;
        //adjustZGrid();
    }
    //qDebug() << "yRange: " << "[" << _y._x <<", "<< _y._y << "]";
    createAndUpdateGrid();
    createAndUpdateAnixnumberWithPnt();
}

void CGrid::setCopyFromOtherOpenWidgetMark(GLboolean _b)
{
    m_notCopyFromOtherOpenGLWidget = _b;
}

GLuint CGrid::getVaoGridAnixnumDgt()
{
    return m_vaoGridAnixnumDgt;
}

GLuint CGrid::getVboGridAnixnumDgt()
{
    return m_vboGridAnixnumDgt;
}

GLuint CGrid::getVboGridAnixnumDgtTexture()
{
    return m_vboGridAnixnumDgtTexture;
}

GLuint CGrid::getVaoGridAnixnumPnt()
{
    return m_vaoGridAnixnumPnt;
}

GLuint CGrid::getVboGridAnixnumPnt()
{
    return m_vboGridAnixnumPnt;
}

GLuint CGrid::getVboGridAnixnumPntTexture()
{
    return m_vboGridAnixnumPntTexture;
}

GLuint CGrid::getVaoGridAnixnumMinus()
{
    return m_vaoGridAnixnumMinus;
}

GLuint CGrid::getVboGridAnixnumMinus()
{
    return m_vboGridAnixnumMinus;
}

GLuint CGrid::getVboGridAnixnumMinusTexture()
{
    return m_vboGridAnixnumMinusTexture;
}

GLuint CGrid::getVaoGridAnixnumLabel()
{
    return m_vaoGridAnixnumLabel;
}

GLuint CGrid::getVboGridAnixnumLabel()
{
    return m_vboGridAnixnumLabel;
}

GLuint CGrid::getVboGridAnixnumLabelTexture()
{
    return m_vboGridAnixnumLabelTexture;
}

void CGrid::setVaoGridAnixnumDgt(GLuint _gridVaoGridAnixnumDgt)
{
    m_vaoGridAnixnumDgt = _gridVaoGridAnixnumDgt;
}

void CGrid::setVboGridAnixnumDgt(GLuint _gridVboGridAnixnumDgt)
{
    m_vboGridAnixnumDgt = _gridVboGridAnixnumDgt;
}

void CGrid::setVboGridAnixnumDgtTexture(GLuint _gridVboGridAnixnumDgtTexture)
{
    m_vboGridAnixnumDgtTexture = _gridVboGridAnixnumDgtTexture;
}

void CGrid::setVaoGridAnixnumPnt(GLuint _gridVaoGridAnixnumPnt)
{
    m_vaoGridAnixnumPnt = _gridVaoGridAnixnumPnt;
}

void CGrid::setVboGridAnixnumPnt(GLuint _gridVboGridAnixnumPnt)
{
    m_vboGridAnixnumPnt = _gridVboGridAnixnumPnt;
}

void CGrid::setVboGridAnixnumPntTexture(GLuint _gridVboGridAnixnumPntTexture)
{
    m_vboGridAnixnumPntTexture = _gridVboGridAnixnumPntTexture;
}

void CGrid::setVaoGridAnixnumMinus(GLuint _gridVaoGridAnixnumMinus)
{
    m_vaoGridAnixnumMinus = _gridVaoGridAnixnumMinus;
}

void CGrid::setVboGridAnixnumMinus(GLuint _gridVboGridAnixnumMinus)
{
    m_vboGridAnixnumMinus = _gridVboGridAnixnumMinus;
}

void CGrid::setVboGridAnixnumMinusTexture(GLuint _gridVboGridAnixnumMinusTexture)
{
    m_vboGridAnixnumMinusTexture = _gridVboGridAnixnumMinusTexture;
}

void CGrid::setVaoGridAnixnumLabel(GLuint _gridVaoGridAnixnumLabel)
{
    m_vaoGridAnixnumLabel = _gridVaoGridAnixnumLabel;
}

void CGrid::setVboGridAnixnumLabel(GLuint _gridVboGridAnixnumLabel)
{
    m_vboGridAnixnumLabel = _gridVboGridAnixnumLabel;
}

void CGrid::setVboGridAnixnumLabelTexture(GLuint _gridVboGridAnixnumLabelTexture)
{
    m_vboGridAnixnumLabelTexture = _gridVboGridAnixnumLabelTexture;
}

void CGrid::createGrid()
{
    m_context->makeCurrent(m_context->surface()); //ll 20170711
    //x轴的网格处理
    GLfloat xMaxf = m_xRange._x;
    GLfloat xMinf = m_xRange._y;
    GLfloat xMaxLower;
    GLfloat xMinUpper;
    GLfloat xfront;
    GLfloat xback;
    GLfloat xScale;
    bool isSuccess = getAnxiIntRange(xMaxf, xMinf, xMaxLower, xMinUpper, xfront, xback);
    if(!isSuccess)
        return;
    xfront *= m_viewScale * m_x._y;
    xback  *= m_viewScale * m_x._y;
    xScale = xback - xfront;
    //y轴的网格处理
    GLfloat yMaxf = m_yRange._x;
    GLfloat yMinf = m_yRange._y;
    GLfloat yMaxLower;
    GLfloat yMinUpper;
    GLfloat yfront;
    GLfloat yback;
    GLfloat yScale;
    isSuccess = getAnxiIntRange(yMaxf, yMinf, yMaxLower, yMinUpper, yfront, yback);
    if(!isSuccess)
        return;
    yfront *= m_viewScale * m_y._y;
    yback  *= m_viewScale * m_y._y;
    yScale = (yback - yfront);
    //qDebug() << "y: [" <<yfront << yback << "] [" << yMinf << yMaxf << "]" << yScale;
    //z轴的网格处理
    GLfloat zMaxf = m_zRange._x;
    GLfloat zMinf = m_zRange._y;
    GLfloat zMaxLower;
    GLfloat zMinUpper;
    GLfloat zfront;
    GLfloat zback;
    GLfloat zScale;
    isSuccess = getAnxiIntRange(zMaxf, zMinf, zMaxLower, zMinUpper, zfront, zback);
    if(!isSuccess)
        return;

    zfront *= m_viewScale * m_z._y;
    zback  *= m_viewScale * m_z._y;

    zScale = (zback - zfront);
    //qDebug() << "z: [" <<zfront << zback << "] [" << zMinf << zMaxf << "]" << zScale;

    //xy
    //qDebug() << "xNumber" <<  m_xGridNumPerAnix << "yNumber" <<  m_yGridNumPerAnix << "zNumber" <<  m_zGridNumPerAnix;
    //上述步骤是为了构建整数坐标系的
    m_gridPositions.clear();
    for(int i(0); i <= m_xGridNumPerAnix; ++i)
    {
        m_gridPositions.push_back(xfront + (GLfloat)(i)* xScale / (GLfloat)m_xGridNumPerAnix);
        m_gridPositions.push_back(0);
        m_gridPositions.push_back(0);
        //qDebug() << "x " << xfront + (GLfloat)(i)* xScale / (GLfloat)m_xGridNumPerAnix << " [" << xfront << ", "<< xback << "]";
        m_gridPositions.push_back(m_gridColor._r); m_gridPositions.push_back(m_gridColor._g);
        m_gridPositions.push_back(m_gridColor._b); m_gridPositions.push_back(m_gridColor._a);

        m_gridPositions.push_back(xfront + (GLfloat)(i)* xScale / (GLfloat)m_xGridNumPerAnix);
        m_gridPositions.push_back(yback);
        m_gridPositions.push_back(0);

        m_gridPositions.push_back(m_gridColor._r); m_gridPositions.push_back(m_gridColor._g);
        m_gridPositions.push_back(m_gridColor._b); m_gridPositions.push_back(m_gridColor._a);
    }
    for(int i(0); i <= m_yGridNumPerAnix; ++i)
    {
        m_gridPositions.push_back(0);
        m_gridPositions.push_back(yfront + (GLfloat)(i)* yScale / (GLfloat)m_yGridNumPerAnix);
        m_gridPositions.push_back(0);
        //qDebug() << "y " << yfront + (GLfloat)(i)* yScale / (GLfloat)m_yGridNumPerAnix << " [" << yfront << ", "<< yback << "]";
        m_gridPositions.push_back(m_gridColor._r); m_gridPositions.push_back(m_gridColor._g);
        m_gridPositions.push_back(m_gridColor._b); m_gridPositions.push_back(m_gridColor._a);

        m_gridPositions.push_back(xback);
        m_gridPositions.push_back(yfront + (GLfloat)(i)* yScale / (GLfloat)m_yGridNumPerAnix);
        m_gridPositions.push_back(0);

        m_gridPositions.push_back(m_gridColor._r); m_gridPositions.push_back(m_gridColor._g);
        m_gridPositions.push_back(m_gridColor._b); m_gridPositions.push_back(m_gridColor._a);
    }
    //yz
    for(int i(0); i <= m_yGridNumPerAnix; ++i)
    {
        m_gridPositions.push_back(0);
        m_gridPositions.push_back(yfront + (GLfloat)(i)* yScale / (GLfloat)m_yGridNumPerAnix);
        m_gridPositions.push_back(0);


        m_gridPositions.push_back(m_gridColor._r); m_gridPositions.push_back(m_gridColor._g);
        m_gridPositions.push_back(m_gridColor._b); m_gridPositions.push_back(m_gridColor._a);

        m_gridPositions.push_back(0);
        m_gridPositions.push_back(yfront + (GLfloat)(i)* yScale / (GLfloat)m_yGridNumPerAnix);
        m_gridPositions.push_back(zback);

        m_gridPositions.push_back(m_gridColor._r); m_gridPositions.push_back(m_gridColor._g);
        m_gridPositions.push_back(m_gridColor._b); m_gridPositions.push_back(m_gridColor._a);
    }
    for(int i(0); i <= m_zGridNumPerAnix; ++i)
    {
        m_gridPositions.push_back(0);
        m_gridPositions.push_back(0);
        m_gridPositions.push_back(zfront + (GLfloat)(i)* zScale / (GLfloat)m_zGridNumPerAnix);

        m_gridPositions.push_back(m_gridColor._r); m_gridPositions.push_back(m_gridColor._g);
        m_gridPositions.push_back(m_gridColor._b); m_gridPositions.push_back(m_gridColor._a);

        m_gridPositions.push_back(0);
        m_gridPositions.push_back(yback);
        m_gridPositions.push_back(zfront + (GLfloat)(i)* zScale / (GLfloat)m_zGridNumPerAnix);


        m_gridPositions.push_back(m_gridColor._r); m_gridPositions.push_back(m_gridColor._g);
        m_gridPositions.push_back(m_gridColor._b); m_gridPositions.push_back(m_gridColor._a);
    }
    //xz
    for(int i(0); i <= m_xGridNumPerAnix; ++i)
    {
        m_gridPositions.push_back(xfront + (GLfloat)(i)* xScale / (GLfloat)m_xGridNumPerAnix);
        m_gridPositions.push_back(0);
        m_gridPositions.push_back(0);


        m_gridPositions.push_back(m_gridColor._r); m_gridPositions.push_back(m_gridColor._g);
        m_gridPositions.push_back(m_gridColor._b); m_gridPositions.push_back(m_gridColor._a);

        m_gridPositions.push_back(xfront + (GLfloat)(i)* xScale / (GLfloat)m_xGridNumPerAnix);
        m_gridPositions.push_back(0);
        m_gridPositions.push_back(zback);

        m_gridPositions.push_back(m_gridColor._r); m_gridPositions.push_back(m_gridColor._g);
        m_gridPositions.push_back(m_gridColor._b); m_gridPositions.push_back(m_gridColor._a);
    }
    for(int i(0); i <= m_zGridNumPerAnix; ++i)
    {
        m_gridPositions.push_back(0);
        m_gridPositions.push_back(0);
        m_gridPositions.push_back(zfront + (GLfloat)(i)* zScale / (GLfloat)m_zGridNumPerAnix);


        m_gridPositions.push_back(m_gridColor._r); m_gridPositions.push_back(m_gridColor._g);
        m_gridPositions.push_back(m_gridColor._b); m_gridPositions.push_back(m_gridColor._a);

        m_gridPositions.push_back(xback);
        m_gridPositions.push_back(0);
        m_gridPositions.push_back(zfront + (GLfloat)(i)* zScale / (GLfloat)m_zGridNumPerAnix);


        m_gridPositions.push_back(m_gridColor._r); m_gridPositions.push_back(m_gridColor._g);
        m_gridPositions.push_back(m_gridColor._b); m_gridPositions.push_back(m_gridColor._a);
    }
}

void CGrid::updataGrid()
{
    glBindVertexArray(m_vaoGrid);
    GLuint  _persize = sizeof(vector3f) + sizeof(vector4f);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboGrid);
    glBufferData(GL_ARRAY_BUFFER,
        m_gridPositions.size() * sizeof(GLfloat),
        m_gridPositions.data(),
        GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
        3,
        GL_FLOAT,
        GL_FALSE,
        _persize,
        0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
        4,
        GL_FLOAT,
        GL_FALSE,
        _persize,
        (char*)(sizeof(vector3f)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

bool CGrid::adjustGridNumber(CGrid::Range range, int& anixGridNumberPerAnix)
{
    GLfloat rangef;
    switch(range)
    {
    case Range::xRange:
    {
        rangef = m_xRange._x - m_xRange._y;
        break;
    }
    case Range::yRange:
    {
        rangef = m_yRange._x - m_yRange._y;
        break;
    }
    case Range::zRange:
    {
        rangef = m_zRange._x - m_zRange._y;
        break;
    }
    default:
        return false;
        break; //useless
    }
    GLint rangei = -1;
    if(rangef > 10)
    {
        rangei = (GLint) rangef;
        while(rangei >= 10)
        {
            rangei /= 10;
            if(rangei < 0)
            {
                QMessageBox::information(NULL, "Error",
                    "1!",
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::Yes);
                return false;
            }
        }
    }
    else if(rangef < 0)
    {
        while(rangef > 1)
        {
            rangef *= 10;
        }
        rangei = (GLint)rangef;
    }
    else {
        rangei = (GLint)rangef;
    }
    //qDebug() << "xrangef:" << xrangef << "xrangei:" << xrangei;
    GLfloat gridNumberPerAnix = 1; //防止为0
    switch (rangei) {
    case 0:
    case 9:
    case 8:
    case 7:
    case 6:
        gridNumberPerAnix = defaultMaxGridNumber;
        break;
//    case 9:
//        gridNumberPerAnix = defaultMaxGridNumber * 0.9;
//        break;
//    case 8:
//        gridNumberPerAnix = defaultMaxGridNumber * 0.8;
//        break;
//    case 7:
//        gridNumberPerAnix = defaultMaxGridNumber * 0.7;
//        break;
//    case 6:
//        gridNumberPerAnix = defaultMaxGridNumber * 0.6;
//        break;
    case 5:
//        gridNumberPerAnix = defaultMaxGridNumber * 0.5;
//        break;
    case 4:
//        gridNumberPerAnix = defaultMaxGridNumber * 0.4;
//        break;
    case 3:
//        gridNumberPerAnix = defaultMaxGridNumber * 0.3;
//        break;
    case 2:
//        gridNumberPerAnix = defaultMaxGridNumber * 0.2;
//        break;
    case 1:
        gridNumberPerAnix = defaultMaxGridNumber * 0.5;
        break;
    default:
        break;
    }
    anixGridNumberPerAnix = (GLint)gridNumberPerAnix;
    return true;
    //qDebug() << "xnumber:" << m_xGridNumPerAnix;
}

void CGrid::createAndUpdateAnixnumberWithPnt()
{
    m_context->makeCurrent(m_context->surface()); //ll 20170711
    if(m_anixLabelOn)
    {
        createAnixnumber();
        updateAnixnumber();
        updateAnixDecimalPnt();
        updateAnixMinus();
        updateAnixLabel();
        m_anixLabelNotEmpty = true;
    }
    else if(m_anixLabelNotEmpty)
    {
        clearGridVector();
        updateAnixnumber();
        updateAnixDecimalPnt();
        updateAnixMinus();
        updateAnixLabel();
        m_anixLabelNotEmpty = false;
    }
    else
    {

    }
}

void CGrid::setDrawType(int drawtype)
{
    m_drawtype = CGrid::DRAWTYPE(drawtype);
}

void CGrid::createAnixnumber()
{
    //m_context->makeCurrent(m_context->surface()); //ll 20170711
    clearGridVector();
    bool isSuccess = true;
    GLfloat anixOffset = 0.0f;
    GLboolean isAxisX = true;
    GLboolean isAxisY = true;
    GLboolean isAxisZ = true;
    GLint startPostionX = 1;
    GLint startPostionY = 1;
    GLint startPostionZ = 1;
    switch (m_drawtype) {
    case DRAWTYPE::XZY:
        isAxisX = true;
        isAxisY = true;
        isAxisZ = true;
        startPostionX = 0;
        startPostionY = 0;
        startPostionZ = 1;

        break;
    case DRAWTYPE::XY:
        isAxisX = true;
        isAxisY = true;
        isAxisZ = false;
        startPostionX = 0;
        startPostionY = 0;
        startPostionZ = 0;
        break;
    case DRAWTYPE::XZ:
        isAxisX = true;
        isAxisY = false;
        isAxisZ = true;
        startPostionX = 0;
        startPostionY = 0;
        startPostionZ = 0;
        break;
    case DRAWTYPE::YZ:
        isAxisX = false;
        isAxisY = true;
        isAxisZ = true;
        startPostionX = 0;
        startPostionY = 0;
        startPostionZ = 0;
        break;
    default:
        break;
    }
    if(isAxisX)
    {
        //x轴的网格处理
        GLfloat xMaxf = m_xRange._x;
        GLfloat xMinf = m_xRange._y;
        GLfloat xMaxLower;
        GLfloat xMinUpper;
        GLfloat xAnix;
        isSuccess = getAnxiIntRange(xMaxf, xMinf, xMaxLower, xMinUpper);
        if(!isSuccess)
            return;
    //    xMaxLower *= m_viewScale * (m_x._y - m_x._x);
    //    xMinUpper *= m_viewScale * (m_x._y - m_x._x);
        //确定字体大小
        m_xSize = m_fontSize * (1.0 - m_xGridNumPerAnix / defaultMaxGridNumber);
        if(m_xSize < 0.0275)
        {
            m_xSize = 0.0275;
        }
        for(int i(startPostionX); i <= m_xGridNumPerAnix; ++i)
        {
            GLint xAnixValue; GLfloat xAnixOffset; GLboolean greaterThanOne = true;
            xAnix = xMinUpper + (GLfloat)i * (xMaxLower - xMinUpper) / (GLfloat) m_xGridNumPerAnix;
            if(getIntvaluePointposOffsetForXAnix(xAnix, xAnixValue, xAnixOffset, greaterThanOne))
            {
                xAnixOffset *= (m_x._y - m_x._x);
                addAnxisNumber(xAnix, Range::xRange, xAnixOffset, m_xSize);
            }
        }
        anixOffset = 1.2f * (m_x._y - m_x._x);
        addAxisLabel(Range::xRange, anixOffset, m_xSize);
    }
    if(isAxisY)
    {
        //y轴的网格处理
        GLfloat yMaxf = m_yRange._x;
        GLfloat yMinf = m_yRange._y;
        GLfloat yMaxLower;
        GLfloat yMinUpper;
        GLfloat yAnix;
        isSuccess = getAnxiIntRange(yMaxf, yMinf, yMaxLower, yMinUpper);
        if(!isSuccess)
            return;
    //    yMaxLower *= m_viewScale * (m_y._y - m_y._x);
    //    yMinUpper *= m_viewScale * (m_y._y - m_y._x);
        //确定字体大小
        m_ySize = m_fontSize * (1.0 - m_xGridNumPerAnix / defaultMaxGridNumber);
        if(m_ySize < 0.0275)
        {
            m_ySize = 0.0275;
        }
        for(int i(startPostionY); i <= m_yGridNumPerAnix; ++i)
        {
            GLint yAnixValue; GLfloat yAnixOffset; GLboolean greaterThanOne = true;
            yAnix = yMinUpper + (GLfloat)i * (yMaxLower - yMinUpper) / (GLfloat) m_yGridNumPerAnix;
            if(getIntvaluePointposOffsetForYAnix(yAnix, yAnixValue, yAnixOffset, greaterThanOne))
            {
                yAnixOffset *= (m_y._y - m_y._x);
                addAnxisNumber(yAnix, Range::yRange, yAnixOffset, m_ySize);
            }
        }
        anixOffset = 1.2f * (m_y._y - m_y._x);
        addAxisLabel(Range::yRange, anixOffset, m_ySize);
    }
    if(isAxisZ)
    {
        //z轴的网格处理
        GLfloat zMaxf = m_zRange._x;
        GLfloat zMinf = m_zRange._y;
        GLfloat zMaxLower;
        GLfloat zMinUpper;
        GLfloat zAnix;
        isSuccess = getAnxiIntRange(zMaxf, zMinf, zMaxLower, zMinUpper);
        if(!isSuccess)
            return;
    //    zMaxLower *= m_viewScale * (m_z._y - m_z._x);
    //    zMinUpper *= m_viewScale * (m_z._y - m_z._x);
        //确定字体大小
        m_zSize = m_fontSize * (1.0 - m_xGridNumPerAnix / defaultMaxGridNumber);
        if(m_zSize < 0.0275)
        {
            m_zSize = 0.0275;
        }
        for(int i(startPostionZ); i <= m_zGridNumPerAnix; ++i)
        {
            GLint zAnixValue; GLfloat zAnixOffset; GLboolean greaterThanOne = true;
            zAnix = zMinUpper + (GLfloat)i * (zMaxLower - zMinUpper) / (GLfloat) m_zGridNumPerAnix;
            if(getIntvaluePointposOffsetForZAnix(zAnix, zAnixValue, zAnixOffset, greaterThanOne))
            {
                zAnixOffset *= (m_z._y - m_z._x);
                addAnxisNumber(zAnix, Range::zRange, zAnixOffset, m_zSize);
            }
        }
        anixOffset = 1.2f * (m_z._y - m_z._x);
        addAxisLabel(Range::zRange, anixOffset, m_zSize);
    }
}

void CGrid::updateAnixnumber()
{
    if((GLint)m_gridAnixnumDgtPositions.size() > m_DigPositionSize)
    {
        //bind VAO
        glBindVertexArray(m_vaoGridAnixnumDgt);
        //Position
        glBindBuffer(GL_ARRAY_BUFFER, m_vboGridAnixnumDgt);

        glBufferData(GL_ARRAY_BUFFER, m_gridAnixnumDgtPositions.size() * sizeof(GLfloat),
                     m_gridAnixnumDgtPositions.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //Texture
        glBindBuffer(GL_ARRAY_BUFFER, m_vboGridAnixnumDgtTexture);

        glBufferData(GL_ARRAY_BUFFER,  m_gridAnixnumDgtTexture.size() * sizeof(GLfloat),
                     m_gridAnixnumDgtTexture.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //unbind VAO
        glBindVertexArray(0);

        m_DigPositionSize = m_gridAnixnumDgtPositions.size();
    }
    else {
        //bind VAO
        glBindVertexArray(m_vaoGridAnixnumDgt);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboGridAnixnumDgt);
//        glEnableVertexAttribArray(0);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_gridAnixnumDgtPositions.size() * sizeof(GLfloat),
                        m_gridAnixnumDgtPositions.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboGridAnixnumDgtTexture);
//        glEnableVertexAttribArray(1);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_gridAnixnumDgtTexture.size() * sizeof(GLfloat),
                        m_gridAnixnumDgtTexture.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }
    return;
}

void CGrid::updateAnixDecimalPnt()
{
    if((GLint)m_gridAnixnumPntPositions.size() > m_PntPositionSize)
    {
        //bind VAO
        glBindVertexArray(m_vaoGridAnixnumPnt);
        //Position
        glBindBuffer(GL_ARRAY_BUFFER, m_vboGridAnixnumPnt);

        glBufferData(GL_ARRAY_BUFFER, m_gridAnixnumPntPositions.size() * sizeof(GLfloat),
                     m_gridAnixnumPntPositions.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //Texture
        glBindBuffer(GL_ARRAY_BUFFER, m_vboGridAnixnumPntTexture);

        glBufferData(GL_ARRAY_BUFFER,  m_gridAnixnumPntTexture.size() * sizeof(GLfloat),
                     m_gridAnixnumPntTexture.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //unbind VAO
        glBindVertexArray(0);
        m_PntPositionSize = m_gridAnixnumPntPositions.size();
    }
    else {
        //bind VAO
        glBindVertexArray(m_vaoGridAnixnumPnt);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboGridAnixnumPnt);
//        glEnableVertexAttribArray(0);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_gridAnixnumPntPositions.size() * sizeof(GLfloat),
                        m_gridAnixnumPntPositions.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboGridAnixnumPntTexture);
//        glEnableVertexAttribArray(1);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_gridAnixnumPntTexture.size() * sizeof(GLfloat),
                        m_gridAnixnumPntTexture.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }
    return;
}

void CGrid::updateAnixMinus()
{
    if((GLint)m_gridAnixnumMinusPositions.size() > m_MinusPositionSize)
    {
        //bind VAO
        glBindVertexArray(m_vaoGridAnixnumMinus);
        //Position
        glBindBuffer(GL_ARRAY_BUFFER, m_vboGridAnixnumMinus);

        glBufferData(GL_ARRAY_BUFFER, m_gridAnixnumMinusPositions.size() * sizeof(GLfloat),
                     m_gridAnixnumMinusPositions.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //Texture
        glBindBuffer(GL_ARRAY_BUFFER, m_vboGridAnixnumMinusTexture);

        glBufferData(GL_ARRAY_BUFFER,  m_gridAnixnumMinusTexture.size() * sizeof(GLfloat),
                     m_gridAnixnumMinusTexture.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //unbind VAO
        glBindVertexArray(0);

        m_MinusPositionSize = m_gridAnixnumMinusPositions.size();
    }

    else {
        //bind VAO
        glBindVertexArray(m_vaoGridAnixnumMinus);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboGridAnixnumMinus);
//        glEnableVertexAttribArray(0);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_gridAnixnumMinusPositions.size() * sizeof(GLfloat),
                        m_gridAnixnumMinusPositions.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboGridAnixnumMinusTexture);
//        glEnableVertexAttribArray(1);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_gridAnixnumMinusTexture.size() * sizeof(GLfloat),
                        m_gridAnixnumMinusTexture.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }
    return;
}

void CGrid::updateAnixLabel()
{
    if((GLint)m_gridAnixnumLabelPositions.size() > m_LabelPositionSize)
    {
        //bind VAO
        glBindVertexArray(m_vaoGridAnixnumLabel);
        //Position
        glBindBuffer(GL_ARRAY_BUFFER, m_vboGridAnixnumLabel);

        glBufferData(GL_ARRAY_BUFFER, m_gridAnixnumLabelPositions.size() * sizeof(GLfloat),
                     m_gridAnixnumLabelPositions.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //Texture
        glBindBuffer(GL_ARRAY_BUFFER, m_vboGridAnixnumLabelTexture);

        glBufferData(GL_ARRAY_BUFFER,  m_gridAnixnumLabelTexture.size() * sizeof(GLfloat),
                     m_gridAnixnumLabelTexture.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //unbind VAO
        glBindVertexArray(0);

        m_LabelPositionSize = m_gridAnixnumLabelPositions.size();
    }

    else {
        //bind VAO
        glBindVertexArray(m_vaoGridAnixnumLabel);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboGridAnixnumLabel);
//        glEnableVertexAttribArray(0);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_gridAnixnumLabelPositions.size() * sizeof(GLfloat),
                        m_gridAnixnumLabelPositions.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboGridAnixnumLabelTexture);
//        glEnableVertexAttribArray(1);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_gridAnixnumLabelTexture.size() * sizeof(GLfloat),
                        m_gridAnixnumLabelTexture.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }
    return;
}

GLboolean CGrid::getIntvaluePointposOffsetForXAnix(GLfloat xAnix, GLint &xAnixValue, GLfloat &xAnixOffset, GLboolean &greaterThanOneMark)
{
    xAnixOffset = (xAnix - m_xRange._y) / (m_xRange._x - m_xRange._y);
    if(fabs(xAnix) >= 1)
    {
        xAnix *= pow(10, m_precision);
        greaterThanOneMark = true;
    }
    else
    {
        xAnix *= pow(10, m_precision);
        greaterThanOneMark = false;
    }
    xAnixValue = (GLint)(xAnix+0.5);
    return true;
}

GLboolean CGrid::getIntvaluePointposOffsetForYAnix(GLfloat yAnix, GLint &yAnixValue, GLfloat &yAnixOffset, GLboolean &greaterThanOneMark)
{
    yAnixOffset = (yAnix - m_yRange._y) / (m_yRange._x - m_yRange._y);
    if(fabs(yAnix) >= 1)
    {
        yAnix *= pow(10, m_precision);
        greaterThanOneMark = true;
    }
    else
    {
        yAnix *= pow(10, m_precision);
        greaterThanOneMark = false;
    }
    yAnixValue = (GLint)(yAnix+0.5);
    return true;
}

GLboolean CGrid::getIntvaluePointposOffsetForZAnix(GLfloat zAnix, GLint &zAnixValue, GLfloat &zAnixOffset, GLboolean &greaterThanOneMark)
{
    zAnixOffset = (zAnix - m_zRange._y) / (m_zRange._x - m_zRange._y);
    if(fabs(zAnix) >= 1)
    {
        zAnix *= pow(10, m_precision);
        greaterThanOneMark = true;
    }
    else
    {
        zAnix *= pow(10, m_precision);
        greaterThanOneMark = false;
    }
    zAnixValue = (GLint)(zAnix+0.5);
    return true;
}

void CGrid::addAnxisNumber(GLfloat anixNumber, CGrid::Range range, GLfloat anixOffset, GLfloat size)
{
    GLint isPositive = true; //是否为正数
    if(anixNumber > 0)
        isPositive = true;
    else
        isPositive =false;

    anixNumber = fabs(anixNumber);
    anixNumber = anixNumber / pow(10, m_tensNumber);
    GLint anixNumberI = (GLint)(anixNumber * pow(10, m_precision) + 0.5);
    GLuint bit = 0;
    m_anixNumber.clear();
    for(int i(0); i < m_precision + 1; ++i)
    {
        bit = (GLuint)(anixNumberI / (GLint)(pow(10, m_precision - i)));
        anixNumberI = anixNumberI % (GLint)(pow(10, m_precision - i));
        m_anixNumber.push_back(bit);
    }

    QMatrix4x4 _rot;
//    /*首先在物体坐标系下进行旋转，保证其转向0*/
    _rot = QMatrix4x4(
               1.0f, 0.0f, 0.0f, 0.0f,
               0.0f, 1.0f, 0.0f, 0.0f,
               0.0f, 0.0f, 1.0f, 0.0f,
               0.0f,0.0f,0.0f,1.0f
           );      //无旋转
    _rot = m_rot * _rot; //抵消外部旋转

    QVector4D _tmpPos;
    QVector4D _rotedPos;
    GLfloat xzOffsetForDrawType = 0.0f;
    GLfloat yzOffsetForDrawType = 0.0f;

    vector3f leftUp = {-0.5f * size, 1.f * size, 0.f /** size*/} ;
    vector3f leftDown = {-0.5f * size, -1.f * size, 0.f /** size*/};
    vector3f rightDown = {0.5f * size, -1.f * size, 0.f /** size*/};
    vector3f rightUp = {0.5f * size, 1.f * size, 0.f /** size*/};

    GLfloat eachNumberXOffset = rightUp._x - leftUp._x;
    GLfloat eachNumberYOffset = rightUp._y - rightDown._y;
    GLfloat numberXOffset = - eachNumberXOffset * (m_precision + 1 + xPointScale) / 2; //数字内部的相对位移
    GLfloat yOffset = -(rightUp._y - rightDown._y) * 2.0f;
    GLfloat pointOffset = 0.0f;
    switch (m_drawtype) {
    case DRAWTYPE::XZY:
        xzOffsetForDrawType = 0.0f;
        yzOffsetForDrawType = 0.0f;
        break;
    case DRAWTYPE::XY:
        xzOffsetForDrawType = 0.0f;
        yzOffsetForDrawType = 0.0f;
        break;
    case DRAWTYPE::XZ:
        xzOffsetForDrawType = numberXOffset;
        yzOffsetForDrawType = 0.0f;
        break;
    case DRAWTYPE::YZ:
        xzOffsetForDrawType = 0.0f;
        yzOffsetForDrawType = numberXOffset;
        break;
    default:
        break;
    }
    for(int llI(0); llI < (m_precision + 1); ++llI )
    {
        leftUp      = {-0.5f * size/* + numberXOffset*/,  1.f * size, 0.f /** size*/};
        leftDown    = {-0.5f * size/* + numberXOffset*/, -1.f * size, 0.f /** size*/};
        rightDown   = { 0.5f * size/* + numberXOffset*/, -1.f * size, 0.f /** size*/};
        rightUp     = { 0.5f * size/* + numberXOffset*/,  1.f * size, 0.f /** size*/};

        leftUp._x += llI*eachNumberXOffset + numberXOffset + pointOffset;
        leftDown._x += llI*eachNumberXOffset + numberXOffset + pointOffset;
        rightDown._x += llI*eachNumberXOffset + numberXOffset + pointOffset;
        rightUp._x += llI*eachNumberXOffset + numberXOffset + pointOffset;

        if(0 == llI)
        {
            pointOffset = xPointScale * eachNumberXOffset;
            GLfloat leftP = rightUp._x;
            GLfloat rightP = leftP + xPointScale * eachNumberXOffset;// + 1.0f / 6.0f * bitOffset;

            GLfloat bottomP = leftDown._y;
            GLfloat topP = bottomP + yPointScale * eachNumberYOffset;
            //qDebug() << "leftP" << leftP<< "rightP" << rightP << "bottomP" << bottomP << "topP" << topP;

            _tmpPos = QVector4D(leftP, topP, 0.f, 1.0f);
            _rotedPos = _rot * _tmpPos;
            vector3f leftUpP = {_rotedPos.x(), _rotedPos.y(), _rotedPos.z()};

            _tmpPos = QVector4D(leftP, bottomP, 0.f, 1.0f);
            _rotedPos = _rot * _tmpPos;
            vector3f leftDownP = {_rotedPos.x(), _rotedPos.y(), _rotedPos.z()};

            _tmpPos = QVector4D(rightP, bottomP, 0.f, 1.0f);
            _rotedPos = _rot * _tmpPos;
            vector3f rightDownP = {_rotedPos.x(), _rotedPos.y(), _rotedPos.z()};

            _tmpPos = QVector4D(rightP, topP, 0.f, 1.0f);
            _rotedPos = _rot * _tmpPos;
            vector3f rightUpP = {_rotedPos.x(), _rotedPos.y(), _rotedPos.z()};

            switch (range) {
            case Range::xRange:
            {
                leftUpP._x += anixOffset;
                leftDownP._x += anixOffset;
                rightDownP._x += anixOffset;
                rightUpP._x += anixOffset;

                leftUpP._y += (yOffset * xAnixOffsetRate);
                leftDownP._y += (yOffset * xAnixOffsetRate);
                rightDownP._y += (yOffset * xAnixOffsetRate);
                rightUpP._y += (yOffset * xAnixOffsetRate);

                leftUpP._z += (xzOffsetForDrawType + yzOffsetForDrawType);
                leftDownP._z += (xzOffsetForDrawType + yzOffsetForDrawType);
                rightDownP._z += (xzOffsetForDrawType + yzOffsetForDrawType);
                rightUpP._z += (xzOffsetForDrawType + yzOffsetForDrawType);
                break;
            }
            case Range::yRange:
            {
                leftUpP._y += anixOffset;
                leftDownP._y += anixOffset;
                rightDownP._y += anixOffset;
                rightUpP._y += anixOffset;

                leftUpP._x += (numberXOffset * yAnixOffsetRate);
                leftDownP._x += (numberXOffset * yAnixOffsetRate);
                rightDownP._x += (numberXOffset * yAnixOffsetRate);
                rightUpP._x += (numberXOffset * yAnixOffsetRate);

                leftUpP._z += (xzOffsetForDrawType + yzOffsetForDrawType);
                leftDownP._z += (xzOffsetForDrawType + yzOffsetForDrawType);
                rightDownP._z += (xzOffsetForDrawType + yzOffsetForDrawType);
                rightUpP._z += (xzOffsetForDrawType + yzOffsetForDrawType);
                break;
            }
            case Range::zRange:
            {
                leftUpP._z += anixOffset;
                leftDownP._z += anixOffset;
                rightDownP._z += anixOffset;
                rightUpP._z += anixOffset;

                leftUpP._x += (numberXOffset * zAnixOffsetRate);
                leftDownP._x += (numberXOffset * zAnixOffsetRate);
                rightDownP._x += (numberXOffset * zAnixOffsetRate);
                rightUpP._x += (numberXOffset * zAnixOffsetRate);

                leftUpP._y += (yOffset * xAnixOffsetRate);
                leftDownP._y += (yOffset * xAnixOffsetRate);
                rightDownP._y += (yOffset * xAnixOffsetRate);
                rightUpP._y += (yOffset * xAnixOffsetRate);
                break;
            }
            default:
                break;
            }

            m_gridAnixnumPntPositions.push_back(leftUpP._x);
            m_gridAnixnumPntPositions.push_back(leftUpP._y);
            m_gridAnixnumPntPositions.push_back(leftUpP._z);

            m_gridAnixnumPntPositions.push_back(leftDownP._x);
            m_gridAnixnumPntPositions.push_back(leftDownP._y);
            m_gridAnixnumPntPositions.push_back(leftDownP._z);

            m_gridAnixnumPntPositions.push_back(rightDownP._x);
            m_gridAnixnumPntPositions.push_back(rightDownP._y);
            m_gridAnixnumPntPositions.push_back(rightDownP._z);

            m_gridAnixnumPntPositions.push_back(rightUpP._x);
            m_gridAnixnumPntPositions.push_back(rightUpP._y);
            m_gridAnixnumPntPositions.push_back(rightUpP._z);
            for(int llJ(0); llJ < 8; ++llJ)
                m_gridAnixnumPntTexture.push_back(pointIndex[llJ]);
            //qDebug() << m_gridAnixnumPntPositions.size() << ", " << m_gridAnixnumPntTexture.size();
        }

        _tmpPos = QVector4D(leftUp._x, leftUp._y, leftUp._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        leftUp._x = _rotedPos.x(); leftUp._y = _rotedPos.y(); leftUp._z = _rotedPos.z();

        _tmpPos = QVector4D(leftDown._x, leftDown._y, leftDown._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        leftDown._x = _rotedPos.x(); leftDown._y = _rotedPos.y(); leftDown._z = _rotedPos.z();

        _tmpPos = QVector4D(rightDown._x, rightDown._y, rightDown._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        rightDown._x = _rotedPos.x(); rightDown._y = _rotedPos.y(); rightDown._z = _rotedPos.z();

        _tmpPos = QVector4D(rightUp._x, rightUp._y, rightUp._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        rightUp._x = _rotedPos.x(); rightUp._y = _rotedPos.y(); rightUp._z = _rotedPos.z();

        switch (range) {
        case Range::xRange:
        {
            leftUp._x += anixOffset;
            leftDown._x += anixOffset;
            rightDown._x += anixOffset;
            rightUp._x += anixOffset;

            leftUp._y += (yOffset * xAnixOffsetRate);
            leftDown._y += (yOffset * xAnixOffsetRate);
            rightDown._y += (yOffset * xAnixOffsetRate);
            rightUp._y += (yOffset * xAnixOffsetRate);

            leftUp._z += (xzOffsetForDrawType + yzOffsetForDrawType);
            leftDown._z += (xzOffsetForDrawType + yzOffsetForDrawType);
            rightDown._z += (xzOffsetForDrawType + yzOffsetForDrawType);
            rightUp._z += (xzOffsetForDrawType + yzOffsetForDrawType);
            break;
        }
        case Range::yRange:
        {
            leftUp._y += anixOffset;
            leftDown._y += anixOffset;
            rightDown._y += anixOffset;
            rightUp._y += anixOffset;

            leftUp._x += (numberXOffset * yAnixOffsetRate);
            leftDown._x += (numberXOffset * yAnixOffsetRate);
            rightDown._x += (numberXOffset * yAnixOffsetRate);
            rightUp._x += (numberXOffset * yAnixOffsetRate);

            leftUp._z += (xzOffsetForDrawType + yzOffsetForDrawType);
            leftDown._z += (xzOffsetForDrawType + yzOffsetForDrawType);
            rightDown._z += (xzOffsetForDrawType + yzOffsetForDrawType);
            rightUp._z += (xzOffsetForDrawType + yzOffsetForDrawType);
            break;
        }
        case Range::zRange:
        {
            leftUp._z += anixOffset;
            leftDown._z += anixOffset;
            rightDown._z += anixOffset;
            rightUp._z += anixOffset;

            leftUp._x += (numberXOffset * zAnixOffsetRate);
            leftDown._x += (numberXOffset * zAnixOffsetRate);
            rightDown._x += (numberXOffset * zAnixOffsetRate);
            rightUp._x += (numberXOffset * zAnixOffsetRate);

            leftUp._y += (yOffset * xAnixOffsetRate);
            leftDown._y += (yOffset * xAnixOffsetRate);
            rightDown._y += (yOffset * xAnixOffsetRate);
            rightUp._y += (yOffset * xAnixOffsetRate);
            break;
        }
        default:
            break;
        }

        m_gridAnixnumDgtPositions.push_back(leftUp._x);
        m_gridAnixnumDgtPositions.push_back(leftUp._y);
        m_gridAnixnumDgtPositions.push_back(leftUp._z);

        m_gridAnixnumDgtPositions.push_back(leftDown._x);
        m_gridAnixnumDgtPositions.push_back(leftDown._y);
        m_gridAnixnumDgtPositions.push_back(leftDown._z);

        m_gridAnixnumDgtPositions.push_back(rightDown._x);
        m_gridAnixnumDgtPositions.push_back(rightDown._y);
        m_gridAnixnumDgtPositions.push_back(rightDown._z);

        m_gridAnixnumDgtPositions.push_back(rightUp._x);
        m_gridAnixnumDgtPositions.push_back(rightUp._y);
        m_gridAnixnumDgtPositions.push_back(rightUp._z);

        for(int llJ(0); llJ < 8; ++llJ)
        {
            //qDebug() << GLint(m_anixNumber.at(llI));
            m_gridAnixnumDgtTexture.push_back(digitsIndex[GLint(m_anixNumber.at(llI)) * 8 + llJ]);
        }

    }

    if(!isPositive)
    {
        leftUp      = {-0.5f *  m_ySize,  1.f *  m_ySize, 0.f /**  m_ySize*/};
        leftDown    = {-0.5f *  m_ySize, -1.f *  m_ySize, 0.f /**  m_ySize*/};
        rightDown   = { 0.5f *  m_ySize, -1.f *  m_ySize, 0.f /**  m_ySize*/};
        rightUp     = { 0.5f *  m_ySize,  1.f *  m_ySize, 0.f /**  m_ySize*/};

        leftUp._x = leftUp._x - eachNumberXOffset + numberXOffset;
        leftDown._x = leftDown._x - eachNumberXOffset + numberXOffset;
        rightDown._x = rightDown._x - eachNumberXOffset + numberXOffset;
        rightUp._x = rightUp._x - eachNumberXOffset + numberXOffset;

        _tmpPos = QVector4D(leftUp._x, leftUp._y, leftUp._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        leftUp._x = _rotedPos.x(); leftUp._y = _rotedPos.y(); leftUp._z = _rotedPos.z();

        _tmpPos = QVector4D(leftDown._x, leftDown._y, leftDown._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        leftDown._x = _rotedPos.x(); leftDown._y = _rotedPos.y(); leftDown._z = _rotedPos.z();

        _tmpPos = QVector4D(rightDown._x, rightDown._y, rightDown._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        rightDown._x = _rotedPos.x(); rightDown._y = _rotedPos.y(); rightDown._z = _rotedPos.z();

        _tmpPos = QVector4D(rightUp._x, rightUp._y, rightUp._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        rightUp._x = _rotedPos.x(); rightUp._y = _rotedPos.y(); rightUp._z = _rotedPos.z();

        switch (range) {
        case Range::xRange:
        {
            leftUp._x += anixOffset;
            leftDown._x += anixOffset;
            rightDown._x += anixOffset;
            rightUp._x += anixOffset;

            leftUp._y += (yOffset * xAnixOffsetRate);
            leftDown._y += (yOffset * xAnixOffsetRate);
            rightDown._y += (yOffset * xAnixOffsetRate);
            rightUp._y += (yOffset * xAnixOffsetRate);

            leftUp._z += (xzOffsetForDrawType + yzOffsetForDrawType);
            leftDown._z += (xzOffsetForDrawType + yzOffsetForDrawType);
            rightDown._z += (xzOffsetForDrawType + yzOffsetForDrawType);
            rightUp._z += (xzOffsetForDrawType + yzOffsetForDrawType);
            break;
        }
        case Range::yRange:
        {
            leftUp._y += anixOffset;
            leftDown._y += anixOffset;
            rightDown._y += anixOffset;
            rightUp._y += anixOffset;

            leftUp._x += (numberXOffset * yAnixOffsetRate);
            leftDown._x += (numberXOffset * yAnixOffsetRate);
            rightDown._x += (numberXOffset * yAnixOffsetRate);
            rightUp._x += (numberXOffset * yAnixOffsetRate);

            leftUp._z += (xzOffsetForDrawType + yzOffsetForDrawType);
            leftDown._z += (xzOffsetForDrawType + yzOffsetForDrawType);
            rightDown._z += (xzOffsetForDrawType + yzOffsetForDrawType);
            rightUp._z += (xzOffsetForDrawType + yzOffsetForDrawType);
            break;
        }
        case Range::zRange:
        {
            leftUp._z += anixOffset;
            leftDown._z += anixOffset;
            rightDown._z += anixOffset;
            rightUp._z += anixOffset;

            leftUp._x += (numberXOffset * zAnixOffsetRate);
            leftDown._x += (numberXOffset * zAnixOffsetRate);
            rightDown._x += (numberXOffset * zAnixOffsetRate);
            rightUp._x += (numberXOffset * zAnixOffsetRate);

            leftUp._y += (yOffset * xAnixOffsetRate);
            leftDown._y += (yOffset * xAnixOffsetRate);
            rightDown._y += (yOffset * xAnixOffsetRate);
            rightUp._y += (yOffset * xAnixOffsetRate);
            break;
        }
        default:
            break;
        }
        m_gridAnixnumMinusPositions.push_back(leftUp._x);
        m_gridAnixnumMinusPositions.push_back(leftUp._y);
        m_gridAnixnumMinusPositions.push_back(leftUp._z);

        m_gridAnixnumMinusPositions.push_back(leftDown._x);
        m_gridAnixnumMinusPositions.push_back(leftDown._y);
        m_gridAnixnumMinusPositions.push_back(leftDown._z);

        m_gridAnixnumMinusPositions.push_back(rightDown._x);
        m_gridAnixnumMinusPositions.push_back(rightDown._y);
        m_gridAnixnumMinusPositions.push_back(rightDown._z);

        m_gridAnixnumMinusPositions.push_back(rightUp._x);
        m_gridAnixnumMinusPositions.push_back(rightUp._y);
        m_gridAnixnumMinusPositions.push_back(rightUp._z);

        for(int llJ(0); llJ < 8; ++llJ)
            m_gridAnixnumMinusTexture.push_back(pointIndex[llJ]);
    }
    //return;
}

void CGrid::addAxisLabel(CGrid::Range range, GLfloat axisOffset, GLfloat size)
{
    GLfloat xLabelOffset = 5.0f;
    GLint tensNumberCopy = abs(m_tensNumber);
    GLboolean isPositive = true;
    if(m_tensNumber < 0)
    {
        isPositive = false;
        xLabelOffset += 1.0;
    }

    while(tensNumberCopy > 1)
    {
        xLabelOffset ++;
        tensNumberCopy /= 10;
    }
    QMatrix4x4 _rot = QMatrix4x4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f,0.0f,0.0f,1.0f
            );      //无旋转;
    _rot = m_rot * _rot;
    QVector4D _tmpPos;
    QVector4D _rotedPos;

    size = 1.2 * size;
    //x, y, z
    vector3f leftUp = {-0.5f * size, 1.f * size, 0.f /** size*/} ;
    vector3f leftDown = {-0.5f * size, -1.f * size, 0.f /** size*/};
    vector3f rightDown = {0.5f * size, -1.f * size, 0.f /** size*/};
    vector3f rightUp = {0.5f * size, 1.f * size, 0.f /** size*/};
    GLfloat eachLabelXOffset = rightUp._x - leftUp._x;
    GLfloat eachLabelYOffset = -(rightUp._y - rightDown._y);
    GLfloat labelOffset = - xLabelOffset * eachLabelXOffset / 2.0;

    leftUp._x = leftUp._x + 0.0 * eachLabelXOffset  + labelOffset;
    leftDown._x = leftDown._x + 0.0 * eachLabelXOffset  + labelOffset;
    rightDown._x = rightDown._x + 0.0 * eachLabelXOffset  + labelOffset;
    rightUp._x = rightUp._x + 0.0 * eachLabelXOffset  + labelOffset;

    _tmpPos = QVector4D(leftUp._x, leftUp._y, leftUp._z, 1.0f);
    _rotedPos = _rot * _tmpPos;
    leftUp._x = _rotedPos.x(); leftUp._y = _rotedPos.y(); leftUp._z = _rotedPos.z();

    _tmpPos = QVector4D(leftDown._x, leftDown._y, leftDown._z, 1.0f);
    _rotedPos = _rot * _tmpPos;
    leftDown._x = _rotedPos.x(); leftDown._y = _rotedPos.y(); leftDown._z = _rotedPos.z();

    _tmpPos = QVector4D(rightDown._x, rightDown._y, rightDown._z, 1.0f);
    _rotedPos = _rot * _tmpPos;
    rightDown._x = _rotedPos.x(); rightDown._y = _rotedPos.y(); rightDown._z = _rotedPos.z();

    _tmpPos = QVector4D(rightUp._x, rightUp._y, rightUp._z, 1.0f);
    _rotedPos = _rot * _tmpPos;
    rightUp._x = _rotedPos.x(); rightUp._y = _rotedPos.y(); rightUp._z = _rotedPos.z();

    switch (range) {
    case Range::xRange:
    {
        leftUp._x += axisOffset;
        leftDown._x += axisOffset;
        rightDown._x += axisOffset;
        rightUp._x += axisOffset;

        leftUp._y += (eachLabelYOffset * xAnixOffsetRate);
        leftDown._y += (eachLabelYOffset * xAnixOffsetRate);
        rightDown._y += (eachLabelYOffset * xAnixOffsetRate);
        rightUp._y += (eachLabelYOffset * xAnixOffsetRate);

        for(int llJ(0); llJ < 8; ++llJ)
            m_gridAnixnumLabelTexture.push_back(labelIndex[llJ]);
        break;
    }
    case Range::yRange:
    {
        leftUp._y += axisOffset;
        leftDown._y += axisOffset;
        rightDown._y += axisOffset;
        rightUp._y += axisOffset;

        leftUp._x += (labelOffset * yAnixOffsetRate);
        leftDown._x += (labelOffset * yAnixOffsetRate);
        rightDown._x += (labelOffset * yAnixOffsetRate);
        rightUp._x += (labelOffset * yAnixOffsetRate);
        for(int llJ(0); llJ < 8; ++llJ)
            m_gridAnixnumLabelTexture.push_back(labelIndex[8+llJ]);
        break;
    }
    case Range::zRange:
    {
        leftUp._z += axisOffset;
        leftDown._z += axisOffset;
        rightDown._z += axisOffset;
        rightUp._z += axisOffset;

        leftUp._x += (labelOffset * zAnixOffsetRate);
        leftDown._x += (labelOffset * zAnixOffsetRate);
        rightDown._x += (labelOffset * zAnixOffsetRate);
        rightUp._x += (labelOffset * zAnixOffsetRate);

        leftUp._y += (eachLabelYOffset * xAnixOffsetRate);
        leftDown._y += (eachLabelYOffset * xAnixOffsetRate);
        rightDown._y += (eachLabelYOffset * xAnixOffsetRate);
        rightUp._y += (eachLabelYOffset * xAnixOffsetRate);
        for(int llJ(0); llJ < 8; ++llJ)
            m_gridAnixnumLabelTexture.push_back(labelIndex[16+llJ]);
        break;
    }
    default:
        break;
    }
    m_gridAnixnumLabelPositions.push_back(leftUp._x);
    m_gridAnixnumLabelPositions.push_back(leftUp._y);
    m_gridAnixnumLabelPositions.push_back(leftUp._z);

    m_gridAnixnumLabelPositions.push_back(leftDown._x);
    m_gridAnixnumLabelPositions.push_back(leftDown._y);
    m_gridAnixnumLabelPositions.push_back(leftDown._z);

    m_gridAnixnumLabelPositions.push_back(rightDown._x);
    m_gridAnixnumLabelPositions.push_back(rightDown._y);
    m_gridAnixnumLabelPositions.push_back(rightDown._z);

    m_gridAnixnumLabelPositions.push_back(rightUp._x);
    m_gridAnixnumLabelPositions.push_back(rightUp._y);
    m_gridAnixnumLabelPositions.push_back(rightUp._z);
    //"/"
    leftUp = {-0.5f * size, 1.f * size, 0.f /** size*/} ;
    leftDown = {-0.5f * size, -1.f * size, 0.f /** size*/};
    rightDown = {0.5f * size, -1.f * size, 0.f /** size*/};
    rightUp = {0.5f * size, 1.f * size, 0.f /** size*/};

    leftUp._x = leftUp._x + 1.0 * eachLabelXOffset  + labelOffset;
    leftDown._x = leftDown._x + 1.0 * eachLabelXOffset  + labelOffset;
    rightDown._x = rightDown._x + 1.0 * eachLabelXOffset  + labelOffset;
    rightUp._x = rightUp._x + 1.0 * eachLabelXOffset  + labelOffset;

    _tmpPos = QVector4D(leftUp._x, leftUp._y, leftUp._z, 1.0f);
    _rotedPos = _rot * _tmpPos;
    leftUp._x = _rotedPos.x(); leftUp._y = _rotedPos.y(); leftUp._z = _rotedPos.z();

    _tmpPos = QVector4D(leftDown._x, leftDown._y, leftDown._z, 1.0f);
    _rotedPos = _rot * _tmpPos;
    leftDown._x = _rotedPos.x(); leftDown._y = _rotedPos.y(); leftDown._z = _rotedPos.z();

    _tmpPos = QVector4D(rightDown._x, rightDown._y, rightDown._z, 1.0f);
    _rotedPos = _rot * _tmpPos;
    rightDown._x = _rotedPos.x(); rightDown._y = _rotedPos.y(); rightDown._z = _rotedPos.z();

    _tmpPos = QVector4D(rightUp._x, rightUp._y, rightUp._z, 1.0f);
    _rotedPos = _rot * _tmpPos;
    rightUp._x = _rotedPos.x(); rightUp._y = _rotedPos.y(); rightUp._z = _rotedPos.z();

    switch (range) {
    case Range::xRange:
    {
        leftUp._x += axisOffset;
        leftDown._x += axisOffset;
        rightDown._x += axisOffset;
        rightUp._x += axisOffset;

        leftUp._y += (eachLabelYOffset * xAnixOffsetRate);
        leftDown._y += (eachLabelYOffset * xAnixOffsetRate);
        rightDown._y += (eachLabelYOffset * xAnixOffsetRate);
        rightUp._y += (eachLabelYOffset * xAnixOffsetRate);
        break;
    }
    case Range::yRange:
    {
        leftUp._y += axisOffset;
        leftDown._y += axisOffset;
        rightDown._y += axisOffset;
        rightUp._y += axisOffset;

        leftUp._x += (labelOffset * yAnixOffsetRate);
        leftDown._x += (labelOffset * yAnixOffsetRate);
        rightDown._x += (labelOffset * yAnixOffsetRate);
        rightUp._x += (labelOffset * yAnixOffsetRate);
        break;
    }
    case Range::zRange:
    {
        leftUp._z += axisOffset;
        leftDown._z += axisOffset;
        rightDown._z += axisOffset;
        rightUp._z += axisOffset;

        leftUp._x += (labelOffset * zAnixOffsetRate);
        leftDown._x += (labelOffset * zAnixOffsetRate);
        rightDown._x += (labelOffset * zAnixOffsetRate);
        rightUp._x += (labelOffset * zAnixOffsetRate);

        leftUp._y += (eachLabelYOffset * xAnixOffsetRate);
        leftDown._y += (eachLabelYOffset * xAnixOffsetRate);
        rightDown._y += (eachLabelYOffset * xAnixOffsetRate);
        rightUp._y += (eachLabelYOffset * xAnixOffsetRate);
        break;
    }
    default:
        break;
    }
    m_gridAnixnumLabelPositions.push_back(leftUp._x);
    m_gridAnixnumLabelPositions.push_back(leftUp._y);
    m_gridAnixnumLabelPositions.push_back(leftUp._z);

    m_gridAnixnumLabelPositions.push_back(leftDown._x);
    m_gridAnixnumLabelPositions.push_back(leftDown._y);
    m_gridAnixnumLabelPositions.push_back(leftDown._z);

    m_gridAnixnumLabelPositions.push_back(rightDown._x);
    m_gridAnixnumLabelPositions.push_back(rightDown._y);
    m_gridAnixnumLabelPositions.push_back(rightDown._z);

    m_gridAnixnumLabelPositions.push_back(rightUp._x);
    m_gridAnixnumLabelPositions.push_back(rightUp._y);
    m_gridAnixnumLabelPositions.push_back(rightUp._z);
    for(int llJ(0); llJ < 8; ++llJ)
        m_gridAnixnumLabelTexture.push_back(labelIndex[24+llJ]);
    //"10"
    m_anixNumber.clear();
    m_anixNumber.push_back((GLuint)1); m_anixNumber.push_back((GLuint)0);
    for(int llI(0); llI < 2; ++llI)
    {
        leftUp = {-0.5f * size, 1.f * size, 0.f /** size*/} ;
        leftDown = {-0.5f * size, -1.f * size, 0.f /** size*/};
        rightDown = {0.5f * size, -1.f * size, 0.f /** size*/};
        rightUp = {0.5f * size, 1.f * size, 0.f /** size*/};

        leftUp._x = leftUp._x + (2.0+llI) * eachLabelXOffset  + labelOffset;
        leftDown._x = leftDown._x + (2.0+llI) * eachLabelXOffset  + labelOffset;
        rightDown._x = rightDown._x + (2.0+llI) * eachLabelXOffset  + labelOffset;
        rightUp._x = rightUp._x + (2.0+llI) * eachLabelXOffset  + labelOffset;

        _tmpPos = QVector4D(leftUp._x, leftUp._y, leftUp._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        leftUp._x = _rotedPos.x(); leftUp._y = _rotedPos.y(); leftUp._z = _rotedPos.z();

        _tmpPos = QVector4D(leftDown._x, leftDown._y, leftDown._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        leftDown._x = _rotedPos.x(); leftDown._y = _rotedPos.y(); leftDown._z = _rotedPos.z();

        _tmpPos = QVector4D(rightDown._x, rightDown._y, rightDown._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        rightDown._x = _rotedPos.x(); rightDown._y = _rotedPos.y(); rightDown._z = _rotedPos.z();

        _tmpPos = QVector4D(rightUp._x, rightUp._y, rightUp._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        rightUp._x = _rotedPos.x(); rightUp._y = _rotedPos.y(); rightUp._z = _rotedPos.z();

        switch (range) {
        case Range::xRange:
        {
            leftUp._x += axisOffset;
            leftDown._x += axisOffset;
            rightDown._x += axisOffset;
            rightUp._x += axisOffset;

            leftUp._y += (eachLabelYOffset * xAnixOffsetRate);
            leftDown._y += (eachLabelYOffset * xAnixOffsetRate);
            rightDown._y += (eachLabelYOffset * xAnixOffsetRate);
            rightUp._y += (eachLabelYOffset * xAnixOffsetRate);
            break;
        }
        case Range::yRange:
        {
            leftUp._y += axisOffset;
            leftDown._y += axisOffset;
            rightDown._y += axisOffset;
            rightUp._y += axisOffset;

            leftUp._x += (labelOffset * yAnixOffsetRate);
            leftDown._x += (labelOffset * yAnixOffsetRate);
            rightDown._x += (labelOffset * yAnixOffsetRate);
            rightUp._x += (labelOffset * yAnixOffsetRate);
            break;
        }
        case Range::zRange:
        {
            leftUp._z += axisOffset;
            leftDown._z += axisOffset;
            rightDown._z += axisOffset;
            rightUp._z += axisOffset;

            leftUp._x += (labelOffset * zAnixOffsetRate);
            leftDown._x += (labelOffset * zAnixOffsetRate);
            rightDown._x += (labelOffset * zAnixOffsetRate);
            rightUp._x += (labelOffset * zAnixOffsetRate);

            leftUp._y += (eachLabelYOffset * xAnixOffsetRate);
            leftDown._y += (eachLabelYOffset * xAnixOffsetRate);
            rightDown._y += (eachLabelYOffset * xAnixOffsetRate);
            rightUp._y += (eachLabelYOffset * xAnixOffsetRate);
            break;
        }
        default:
            break;
        }
        m_gridAnixnumDgtPositions.push_back(leftUp._x);
        m_gridAnixnumDgtPositions.push_back(leftUp._y);
        m_gridAnixnumDgtPositions.push_back(leftUp._z);

        m_gridAnixnumDgtPositions.push_back(leftDown._x);
        m_gridAnixnumDgtPositions.push_back(leftDown._y);
        m_gridAnixnumDgtPositions.push_back(leftDown._z);

        m_gridAnixnumDgtPositions.push_back(rightDown._x);
        m_gridAnixnumDgtPositions.push_back(rightDown._y);
        m_gridAnixnumDgtPositions.push_back(rightDown._z);

        m_gridAnixnumDgtPositions.push_back(rightUp._x);
        m_gridAnixnumDgtPositions.push_back(rightUp._y);
        m_gridAnixnumDgtPositions.push_back(rightUp._z);
        for(int llJ(0); llJ < 8; ++llJ)
            m_gridAnixnumDgtTexture.push_back(digitsIndex[GLint(m_anixNumber.at(llI)) * 8 + llJ]);
    }
    //m_tensNumber
    GLfloat tensXOffset = 0.0f;
    GLfloat minus = 0.0f;
    if(!isPositive)
    {
        leftUp      = {-0.5f * 0.9f * size,  1.f * 0.9f * size, 0.f /**  m_ySize*/};
        leftDown    = {-0.5f * 0.9f * size,  -1.f * 0.9f * size, 0.f /**  m_ySize*/};
        rightDown   = { 0.5f * 0.9f * size,  -1.f * 0.9f * size, 0.f /**  m_ySize*/};
        rightUp     = { 0.5f * 0.9f * size,  1.f * 0.9f * size, 0.f /**  m_ySize*/};

        tensXOffset = rightUp._x - leftUp._x;
        minus = 1.0f;

        leftUp._x = leftUp._x + 4.0f * eachLabelXOffset  + labelOffset;
        leftDown._x = leftDown._x + 4.0f * eachLabelXOffset  + labelOffset;
        rightDown._x = rightDown._x + 4.0f * eachLabelXOffset  + labelOffset;
        rightUp._x = rightUp._x + 4.0f * eachLabelXOffset  + labelOffset;

        leftUp._y = leftUp._y - 2.f / 3.f * eachLabelYOffset;
        leftDown._y = leftDown._y - 2.f / 3.f * eachLabelYOffset;
        rightDown._y = rightDown._y - 2.f / 3.f * eachLabelYOffset;
        rightUp._y = rightUp._y - 2.f / 3.f * eachLabelYOffset;

        _tmpPos = QVector4D(leftUp._x, leftUp._y, leftUp._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        leftUp._x = _rotedPos.x(); leftUp._y = _rotedPos.y(); leftUp._z = _rotedPos.z();

        _tmpPos = QVector4D(leftDown._x, leftDown._y, leftDown._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        leftDown._x = _rotedPos.x(); leftDown._y = _rotedPos.y(); leftDown._z = _rotedPos.z();

        _tmpPos = QVector4D(rightDown._x, rightDown._y, rightDown._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        rightDown._x = _rotedPos.x(); rightDown._y = _rotedPos.y(); rightDown._z = _rotedPos.z();

        _tmpPos = QVector4D(rightUp._x, rightUp._y, rightUp._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        rightUp._x = _rotedPos.x(); rightUp._y = _rotedPos.y(); rightUp._z = _rotedPos.z();

        switch (range) {
        case Range::xRange:
        {
            leftUp._x += axisOffset;
            leftDown._x += axisOffset;
            rightDown._x += axisOffset;
            rightUp._x += axisOffset;

            leftUp._y += (eachLabelYOffset * xAnixOffsetRate);
            leftDown._y += (eachLabelYOffset * xAnixOffsetRate);
            rightDown._y += (eachLabelYOffset * xAnixOffsetRate);
            rightUp._y += (eachLabelYOffset * xAnixOffsetRate);
            break;
        }
        case Range::yRange:
        {
            leftUp._y += axisOffset;
            leftDown._y += axisOffset;
            rightDown._y += axisOffset;
            rightUp._y += axisOffset;

            leftUp._x += (labelOffset * yAnixOffsetRate);
            leftDown._x += (labelOffset * yAnixOffsetRate);
            rightDown._x += (labelOffset * yAnixOffsetRate);
            rightUp._x += (labelOffset * yAnixOffsetRate);
            break;
        }
        case Range::zRange:
        {
            leftUp._z += axisOffset;
            leftDown._z += axisOffset;
            rightDown._z += axisOffset;
            rightUp._z += axisOffset;

            leftUp._x += (labelOffset * zAnixOffsetRate);
            leftDown._x += (labelOffset * zAnixOffsetRate);
            rightDown._x += (labelOffset * zAnixOffsetRate);
            rightUp._x += (labelOffset * zAnixOffsetRate);

            leftUp._y += (eachLabelYOffset * xAnixOffsetRate);
            leftDown._y += (eachLabelYOffset * xAnixOffsetRate);
            rightDown._y += (eachLabelYOffset * xAnixOffsetRate);
            rightUp._y += (eachLabelYOffset * xAnixOffsetRate);
            break;
        }
        default:
            break;
        }
        m_gridAnixnumMinusPositions.push_back(leftUp._x);
        m_gridAnixnumMinusPositions.push_back(leftUp._y);
        m_gridAnixnumMinusPositions.push_back(leftUp._z);

        m_gridAnixnumMinusPositions.push_back(leftDown._x);
        m_gridAnixnumMinusPositions.push_back(leftDown._y);
        m_gridAnixnumMinusPositions.push_back(leftDown._z);

        m_gridAnixnumMinusPositions.push_back(rightDown._x);
        m_gridAnixnumMinusPositions.push_back(rightDown._y);
        m_gridAnixnumMinusPositions.push_back(rightDown._z);

        m_gridAnixnumMinusPositions.push_back(rightUp._x);
        m_gridAnixnumMinusPositions.push_back(rightUp._y);
        m_gridAnixnumMinusPositions.push_back(rightUp._z);

        for(int llJ(0); llJ < 8; ++llJ)
            m_gridAnixnumMinusTexture.push_back(pointIndex[llJ]);
    }
    GLint tensNumber = abs(m_tensNumber);
    GLfloat _count = 0.0f;
    GLint x = 0;
    tensNumberCopy = abs(m_tensNumber);
    GLfloat bitsCount = 0.0f;
    while(tensNumberCopy >= 10)
    {
        bitsCount ++;
        tensNumberCopy /= 10;
    }
    while(tensNumber >= 0){
        x = (tensNumber / (GLint)(pow(10, bitsCount)));
        if(tensNumber >= 10)
            tensNumber = tensNumber % (GLint)(pow(10, bitsCount));
        else
            tensNumber = -1;

        leftUp      = {-0.5f * 0.9f * size,  1.f * 0.9f * size, 0.f /**  m_ySize*/};
        leftDown    = {-0.5f * 0.9f * size,  -1.f * 0.9f * size, 0.f /**  m_ySize*/};
        rightDown   = { 0.5f * 0.9f * size,  -1.f * 0.9f * size, 0.f /**  m_ySize*/};
        rightUp     = { 0.5f * 0.9f * size,  1.f * 0.9f * size, 0.f /**  m_ySize*/};

        leftUp._x = leftUp._x + 4.0f * eachLabelXOffset + (minus + _count) * tensXOffset + labelOffset;
        leftDown._x = leftDown._x + 4.0f * eachLabelXOffset + (minus + _count) * tensXOffset + labelOffset;
        rightDown._x = rightDown._x + 4.0f * eachLabelXOffset + (minus + _count) * tensXOffset + labelOffset;
        rightUp._x = rightUp._x + 4.0f * eachLabelXOffset + (minus + _count) * tensXOffset + labelOffset;

        leftUp._y = leftUp._y - 2.f / 3.f * eachLabelYOffset;
        leftDown._y = leftDown._y - 2.f / 3.f * eachLabelYOffset;
        rightDown._y = rightDown._y - 2.f / 3.f * eachLabelYOffset;
        rightUp._y = rightUp._y - 2.f / 3.f * eachLabelYOffset;

        _tmpPos = QVector4D(leftUp._x, leftUp._y, leftUp._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        leftUp._x = _rotedPos.x(); leftUp._y = _rotedPos.y(); leftUp._z = _rotedPos.z();

        _tmpPos = QVector4D(leftDown._x, leftDown._y, leftDown._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        leftDown._x = _rotedPos.x(); leftDown._y = _rotedPos.y(); leftDown._z = _rotedPos.z();

        _tmpPos = QVector4D(rightDown._x, rightDown._y, rightDown._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        rightDown._x = _rotedPos.x(); rightDown._y = _rotedPos.y(); rightDown._z = _rotedPos.z();

        _tmpPos = QVector4D(rightUp._x, rightUp._y, rightUp._z, 1.0f);
        _rotedPos = _rot * _tmpPos;
        rightUp._x = _rotedPos.x(); rightUp._y = _rotedPos.y(); rightUp._z = _rotedPos.z();

        switch (range) {
        case Range::xRange:
        {
            leftUp._x += axisOffset;
            leftDown._x += axisOffset;
            rightDown._x += axisOffset;
            rightUp._x += axisOffset;

            leftUp._y += (eachLabelYOffset * xAnixOffsetRate);
            leftDown._y += (eachLabelYOffset * xAnixOffsetRate);
            rightDown._y += (eachLabelYOffset * xAnixOffsetRate);
            rightUp._y += (eachLabelYOffset * xAnixOffsetRate);
            break;
        }
        case Range::yRange:
        {
            leftUp._y += axisOffset;
            leftDown._y += axisOffset;
            rightDown._y += axisOffset;
            rightUp._y += axisOffset;

            leftUp._x += (labelOffset * yAnixOffsetRate);
            leftDown._x += (labelOffset * yAnixOffsetRate);
            rightDown._x += (labelOffset * yAnixOffsetRate);
            rightUp._x += (labelOffset * yAnixOffsetRate);
            break;
        }
        case Range::zRange:
        {
            leftUp._z += axisOffset;
            leftDown._z += axisOffset;
            rightDown._z += axisOffset;
            rightUp._z += axisOffset;

            leftUp._x += (labelOffset * zAnixOffsetRate);
            leftDown._x += (labelOffset * zAnixOffsetRate);
            rightDown._x += (labelOffset * zAnixOffsetRate);
            rightUp._x += (labelOffset * zAnixOffsetRate);

            leftUp._y += (eachLabelYOffset * xAnixOffsetRate);
            leftDown._y += (eachLabelYOffset * xAnixOffsetRate);
            rightDown._y += (eachLabelYOffset * xAnixOffsetRate);
            rightUp._y += (eachLabelYOffset * xAnixOffsetRate);
            break;
        }
        default:
            break;
        }
        m_gridAnixnumDgtPositions.push_back(leftUp._x);
        m_gridAnixnumDgtPositions.push_back(leftUp._y);
        m_gridAnixnumDgtPositions.push_back(leftUp._z);

        m_gridAnixnumDgtPositions.push_back(leftDown._x);
        m_gridAnixnumDgtPositions.push_back(leftDown._y);
        m_gridAnixnumDgtPositions.push_back(leftDown._z);

        m_gridAnixnumDgtPositions.push_back(rightDown._x);
        m_gridAnixnumDgtPositions.push_back(rightDown._y);
        m_gridAnixnumDgtPositions.push_back(rightDown._z);

        m_gridAnixnumDgtPositions.push_back(rightUp._x);
        m_gridAnixnumDgtPositions.push_back(rightUp._y);
        m_gridAnixnumDgtPositions.push_back(rightUp._z);

        for(int llJ(0); llJ < 8; ++llJ)
            m_gridAnixnumDgtTexture.push_back(digitsIndex[x * 8 + llJ]);
        _count ++;
        bitsCount --;
    }
}

void CGrid::clearGridVector()
{
    m_gridAnixnumDgtPositions.clear();
    m_gridAnixnumDgtTexture.clear();
    m_gridAnixnumPntPositions.clear();
    m_gridAnixnumPntTexture.clear();
    m_gridAnixnumMinusPositions.clear();
    m_gridAnixnumMinusTexture.clear();
    m_gridAnixnumLabelPositions.clear();
    m_gridAnixnumLabelTexture.clear();
}

bool CGrid::getAnxiIntRange(GLfloat Maxf, GLfloat Minf, GLfloat &MaxLower, GLfloat &MinUpper, GLfloat &front, GLfloat &back)
{
    if(fabs(Maxf - Minf) < zeroGate) //当距离过小时会直接跳过,不更改原始的数据
        return false;

//    MaxLower = Maxf;
//    MinUpper = Minf;
//    front = 0.0f;
//    back = 1.0f;


    if(fabs(Maxf) > 1 && fabs(Minf) > 1)
    {
        MaxLower = floor(Maxf);
        MinUpper = ceil(Minf);

        MaxLower = Maxf;  // !!
        MinUpper = Minf;  // !!

        //qDebug() << "MaxLower向下取整， MinUpper向上取整:" << MaxLower << " " << MinUpper << " Maxf and Minf: " << Maxf << " " << Minf;
    }
    else if(fabs(Maxf) < zeroGate)
    {
        while(fabs(Minf) < 1)
        {
            Minf *= 10;
        }
        MaxLower = floor(Maxf);
        MinUpper = ceil(Minf);

        MaxLower = Maxf;  // !!
        MinUpper = Minf;  // !!

        //qDebug() << "MaxLower向下取整， MinUpper向上取整:" << MaxLower << " " << MinUpper << " Maxf and Minf: " << Maxf << " " << Minf;
    }
    else if(fabs(Minf) < zeroGate)
    {
        while(fabs(Maxf) < 1)
        {
            Maxf *= 10;
        }
        MaxLower = floor(Maxf);
        MinUpper = ceil(Minf);

        MaxLower = Maxf;  // !!
        MinUpper = Minf;  // !!

        //qDebug() << "MaxLower向下取整， MinUpper向上取整:" << MaxLower << " " << MinUpper << " Maxf and Minf: " << Maxf << " " << Minf;
    }
    else
    {
        while(fabs(Minf) < 1 || fabs(Maxf) < 1)
        {
            Minf *= 10;
            Maxf *= 10;
        }
        MaxLower = floor(Maxf);
        MinUpper = ceil(Minf);

        MaxLower = Maxf;  // !!
        MinUpper = Minf;  // !!

        //qDebug() << "MaxLower向下取整， MinUpper向上取整:" << MaxLower << " " << MinUpper << " Maxf and Minf: " << Maxf << " " << Minf;
    }


    front = (MinUpper - Minf) / (Maxf - Minf);
    //back  = 1.0f - (Maxf - MaxLower) / (Maxf - Minf);
    back  = (MaxLower - Minf) / (Maxf - Minf);
    return true;
}

bool CGrid::getAnxiIntRange(GLfloat Maxf, GLfloat Minf, GLfloat &MaxLower, GLfloat &MinUpper)
{
    GLfloat MaxfCopy = Maxf; // !!
    GLfloat MinfCopy = Minf; // !!

    if(fabs(Maxf - Minf) < zeroGate) //当距离过小时会直接跳过,不更改原始的数据
        return false;
    GLint count = 0;
    if(fabs(Maxf) > 1 && fabs(Minf) > 1)
    {
        MaxLower = floor(Maxf);
        MinUpper = ceil(Minf);
    }
    else if(fabs(Maxf) < zeroGate)
    {
        count = 0;
        while(fabs(Minf) < 1)
        {
            count ++;
            Minf *= 10;
        }
        MinUpper = ceil(Minf);
        MinUpper = MinUpper / pow(10, count);
        MaxLower = floor(Maxf);

    }
    else if(fabs(Minf) < zeroGate)
    {
        count = 0;
        while(fabs(Maxf) < 1)
        {
            count ++;
            Maxf *= 10;
        }
        MaxLower = floor(Maxf);
        MaxLower = MaxLower / pow(10, count);
        MinUpper = ceil(Minf);
    }
    else {
        count = 0;
        while(fabs(Minf) < 1 || fabs(Maxf) < 1)
        {
            count ++;
            Minf *= 10;
            Maxf *= 10;
        }
        MaxLower = floor(Maxf);
        MinUpper = ceil(Minf);
        MaxLower = MaxLower / pow(10, count);
        MinUpper = MinUpper / pow(10, count);
    }
    //得到科学计数法的10的倍数
    GLfloat rangeMax;

    if(fabs(MaxLower) > fabs(MinUpper))
        rangeMax = fabs(MaxLower);
    else
        rangeMax = fabs(MinUpper);
    if(rangeMax >= 1)
    {
        m_tensNumber = 0;
        while(rangeMax >= 10)
        {
            m_tensNumber ++;
            rangeMax /= 10;
        }
    }
    else if(rangeMax < 1)
    {
        m_tensNumber = 0;
        while(rangeMax < 1)
        {
            m_tensNumber --;
            rangeMax *=10;
        }
    }

    MaxLower = MaxfCopy; // !!
    MinUpper = MinfCopy; // !!

    return true;
}

void CGrid::createAndUpdateGrid()
{
    if(m_gridOn)
    {
        createGrid();
        updataGrid();
    }
    else
    {
        m_gridPositions.clear();
    }

}
