#include "MyOpenglWidget/QOpenglMain.h"

QOpenglMain::QOpenglMain(u_char _frameType, CAllocVBOandVAO *callocVBOandVAO, CChart::DRAWTYPE drawType, QWidget *parent)
    : QOpenGLWidget(parent),
      m_frameType(_frameType),
      m_allocVBOandVAO(callocVBOandVAO)
{
    //QOpenGLContext* tmp = context();
    m_chart.setAlloc(m_allocVBOandVAO);
    m_drawtype = drawType;
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    setMinimumSize(QSize(200, 200));
}

size_t flushtime = 0;

QOpenglMain::~QOpenglMain()
{

}

void QOpenglMain::setColor(size_t _lineIndex, vector4f _color)
{
    m_chart.setColor(_lineIndex, _color);
    this->update();
}

bool QOpenglMain::addPoint(size_t _lineindex, const vector3f & _posion)
{
    if(m_notCopyFromOtherOpenGLWidget)
        return m_chart.addPoint(_lineindex, _posion/*,  context()*/);

}

void QOpenglMain::setGridOnOrOff()
{
    m_chart.setGridOnOrOff();
    this->update();
}

void QOpenglMain::setGridOnOrOff(bool _b)
{
    m_chart.setGridOnOrOff(_b);
    this->update();
}

void QOpenglMain::setAnixLabelOnOrOff()
{
    m_chart.setAnixLabelOnOrOff();
    this->update();
}

void QOpenglMain::setAxisPrecision(GLint _precision)
{
    m_chart.setAxisPrecision(_precision);
}

void QOpenglMain::setLineXYPlaneOn(/*GLboolean _b*/)
{
    m_chart.setLineXYPlaneOn(/*_b*/);
    this->update();
}

void QOpenglMain::setLineXZPlaneOn(/*GLboolean _b*/)
{
    m_chart.setLineXZPlaneOn(/*_b*/);
    this->update();
}

void QOpenglMain::setLineYZPlaneOn(/*GLboolean _b*/)
{
    m_chart.setLineYZPlaneOn(/*_b*/);
    this->update();
}

void QOpenglMain::setLinesEnabled(size_t _lineIndex, bool _b)
{
    m_chart.setLinesEnabled(_lineIndex, _b);
    this->update();
}

void QOpenglMain::setCopyFromOtherOpenWidgetMark(GLboolean _b)
{
    m_notCopyFromOtherOpenGLWidget = _b;
    m_chart.setCopyFromOtherOpenWidgetMark(_b);
}

void QOpenglMain::getAllVaoAndVboAndVboIndex()
{
        //anix
        getAnixVao();
        getAnixVbo();
        getAnixVboIndex();
        //grid
        getVaoGridAnixnumDgt();
        getVboGridAnixnumDgt();
        getVboGridAnixnumDgtTexture();

        getVaoGridAnixnumPnt();
        getVboGridAnixnumPnt();
        getVboGridAnixnumPntTexture();

        getVaoGridAnixnumMinus();
        getVboGridAnixnumMinus();
        getVboGridAnixnumMinusTexture();

        getVaoGridAnixnumLabel();
        getVboGridAnixnumLabel();
        getVboGridAnixnumLabelTexture();
        //lines
        getLinesVao();
        getLinesVbo();
}

VaoVbos QOpenglMain::getVaoAndVbos()
{
    return m_vaoVbos;
}

void QOpenglMain::setVaoAndVbosBuffer(vector<GLfloat>& _anixVboBuffer, vector<GLuint>& _anixVboIndexBuffer)
{
    if(!m_notCopyFromOtherOpenGLWidget)
    {
        m_chart.setAnixVboBuffer(_anixVboBuffer);
        m_chart.setAnixVboIndexBuffer(_anixVboIndexBuffer);
    }
    else
    {
        QMessageBox::information(NULL, "QOpenGLMain",
            "必须先设置窗口拷贝标志m_notCopyFromOtherOpenGLWidget=false",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    }
}

vector<GLuint> &QOpenglMain::getAnixVboIndexBufferData()
{
    glBindVertexArray(m_vaoVbos.m_anixVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vaoVbos.m_anixVboIndex);
    GLint size = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    m_vboIndexData.resize(size / sizeof(GLuint));
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, size, m_vboIndexData.data());
    return m_vboIndexData;
}

void QOpenglMain::getLinesVboBufferData(list<vector<GLfloat> > & _linesBuffer, vector<size_t> & _linesPointAmout)
{
    _linesPointAmout = m_chart.getLinesPointsAmount();
    vector<GLfloat> tmpVboBuffer;
    list<GLuint>::iterator itVaos = m_vaoVbos.m_linesVaos.begin();
    //GLint size = 0;
    int count = 0;
    for(list<vector<GLfloat> >::iterator it = _linesBuffer.begin(); it != _linesBuffer.end(); ++it)
    {
        (*it).clear();
    }
    _linesBuffer.clear();
    int size = 0;
    for(list<GLuint>::iterator it = m_vaoVbos.m_linesVbos.begin(); it != m_vaoVbos.m_linesVbos.end(); ++it)
    {
        glBindVertexArray(*itVaos);
        glBindBuffer(GL_ARRAY_BUFFER, *it);
        size = _linesPointAmout.at(count);
        tmpVboBuffer.resize(size * 3);
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(vector3f), tmpVboBuffer.data());
        _linesBuffer.push_back(tmpVboBuffer);
        ++itVaos;
        count++;
    }
}

void QOpenglMain::setLinesVboBufferData(list<vector<GLfloat> > &_linesBuffer, vector<size_t> & _linesAmount)
{
    m_chart.setLinesVboBufferData(_linesBuffer, _linesAmount);

}

void QOpenglMain::getCordRangei(vector2f &_x, vector2f &_y, vector2f &_z)
{
    m_chart.getCordRangei(_x, _y, _z);
}

void QOpenglMain::setCordRangei(vector2f &_x, vector2f &_y, vector2f &_z)
{
    m_chart.setCordRangei(_x, _y, _z);
}

void QOpenglMain::getRotation(QMatrix4x4 &_rotation)
{
    m_chart.getRotation(_rotation);
}

void QOpenglMain::setRotation(QMatrix4x4 &_rotation)
{
    m_chart.setRotation(_rotation);
}

void QOpenglMain::getMove(QMatrix4x4 &_move)
{
    m_chart.getMove(_move);
}

void QOpenglMain::setMove(QMatrix4x4 &_move)
{
    m_chart.setMove(_move);
}

void QOpenglMain::setDrawType(CChart::DRAWTYPE _type)
{
    m_drawtype = _type;
    m_chart.setDrawTypeAlone(_type);
}

void QOpenglMain::getDrawType(CChart::DRAWTYPE &_type)
{
    m_chart.getDrawType(_type);
}

void QOpenglMain::getGridNumer(int &_xGridNumPerAnix, int &_yGridNumPerAnix, int &_zGridNumPerAnix)
{
    m_chart.getGridNumer(_xGridNumPerAnix, _yGridNumPerAnix, _zGridNumPerAnix);
}

void QOpenglMain::setGridNumer(int _xGridNumPerAnix, int _yGridNumPerAnix, int _zGridNumPerAnix)
{
    m_chart.setGridNumer(_xGridNumPerAnix, _yGridNumPerAnix, _zGridNumPerAnix);
}

void QOpenglMain::getLineColor(map<u_short, vector4f> &_lineColor)
{
    _lineColor = m_chart.getLineColor();
}

void QOpenglMain::setLineColor(map<u_short, vector4f> &_lineColor)
{
    m_chart.setLineColor(_lineColor);
}

vector<GLfloat> &QOpenglMain::getAnixVboBufferData()
{
    glBindVertexArray(m_vaoVbos.m_anixVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vaoVbos.m_anixVbo);
    GLint size = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    m_vboData.resize(size / sizeof(GLfloat));

    glGetBufferSubData(GL_ARRAY_BUFFER, 0, size, m_vboData.data());

    return m_vboData;
}

void QOpenglMain::getAnixVao()
{
    m_vaoVbos.m_anixVao = m_chart.getAnixVao();
}

void QOpenglMain::getAnixVbo()
{
    m_vaoVbos.m_anixVbo = m_chart.getAnixVbo();
}

void QOpenglMain::getAnixVboIndex()
{
    m_vaoVbos.m_anixVboIndex = m_chart.getAnixVboIndex();
}

void QOpenglMain::getVaoGridAnixnumDgt()
{
    m_vaoVbos.m_gridVaoGridAnixnumDgt = m_chart.getVaoGridAnixnumDgt();
}

void QOpenglMain::getVboGridAnixnumDgt()
{
    m_vaoVbos.m_gridVboGridAnixnumDgt = m_chart.getVboGridAnixnumDgt();
}

void QOpenglMain::getVboGridAnixnumDgtTexture()
{
    m_vaoVbos.m_gridVboGridAnixnumDgtTexture = m_chart.getVboGridAnixnumDgtTexture();
}

void QOpenglMain::getVaoGridAnixnumPnt()
{
    m_vaoVbos.m_gridVaoGridAnixnumPnt = m_chart.getVaoGridAnixnumPnt();
}

void QOpenglMain::getVboGridAnixnumPnt()
{
    m_vaoVbos.m_gridVboGridAnixnumPnt = m_chart.getVboGridAnixnumPnt();
}

void QOpenglMain::getVboGridAnixnumPntTexture()
{
    m_vaoVbos.m_gridVboGridAnixnumPntTexture = m_chart.getVboGridAnixnumPntTexture();
}

void QOpenglMain::getVaoGridAnixnumMinus()
{
    m_vaoVbos.m_gridVaoGridAnixnumMinus = m_chart.getVaoGridAnixnumMinus();
}

void QOpenglMain::getVboGridAnixnumMinus()
{
    m_vaoVbos.m_gridVboGridAnixnumMinus = m_chart.getVboGridAnixnumMinus();
}

void QOpenglMain::getVboGridAnixnumMinusTexture()
{
    m_vaoVbos.m_gridVboGridAnixnumMinusTexture = m_chart.getVboGridAnixnumMinusTexture();
}

void QOpenglMain::getVaoGridAnixnumLabel()
{
    m_vaoVbos.m_gridVaoGridAnixnumLabel = m_chart.getVaoGridAnixnumLabel();
}

void QOpenglMain::getVboGridAnixnumLabel()
{
    m_vaoVbos.m_gridVboGridAnixnumLabel = m_chart.getVboGridAnixnumLabel();
}

void QOpenglMain::getVboGridAnixnumLabelTexture()
{
    m_vaoVbos.m_gridVboGridAnixnumLabelTexture = m_chart.getVboGridAnixnumLabelTexture();
}

void QOpenglMain::getLinesVao()
{
    m_vaoVbos.m_linesVaos.clear();
    m_vaoVbos.m_linesVaos = m_chart.getLinesVao();
}

void QOpenglMain::getLinesVbo()
{
    m_vaoVbos.m_linesVbos.clear();
    m_vaoVbos.m_linesVbos = m_chart.getLinesVbo();
}

void QOpenglMain::updateLines()
{
    m_chart.updateLines();
}

void QOpenglMain::drawOnce()
{
    if(!m_notCopyFromOtherOpenGLWidget)
    {
        m_chart.draw();
    }
}

bool QOpenglMain::checkRangeChange(vector2f& _xRange, vector2f& _yRange, vector2f& _zRange)
{

    return m_chart.checkRangeChange(_xRange, _yRange, _zRange);
}

void QOpenglMain::initializeGL()
{
    initializeOpenGLFunctions();
    m_chart.setWidthAndHeight(width(), height());
    m_chart.initial({0,0,0}, context(),context()->surface());				//��ʼ��chart

    if(m_notCopyFromOtherOpenGLWidget)
        m_chart.setDrawType(CChart::DRAWTYPE(m_drawtype));
}

void QOpenglMain::paintGL()
{
    glViewport(0, 0, width(), height());

    m_chart.draw();
}

void QOpenglMain::mouseDoubleClickEvent(QMouseEvent * event)
{
    if(0 == m_frameType)
    {
        if (event->button()==Qt::LeftButton)
        {
            m_drawtype += 1;
            if (m_drawtype > CChart::YZ)
            {
                m_drawtype = CChart::XZY;
            }
            m_chart.setDrawType(CChart::DRAWTYPE(m_drawtype));
        }
        update();
    }
}

void QOpenglMain::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_leftPushDown = true;
		m_mouseCordx = event->x();
		m_mouseCordy = event->y();
	}
	if (event->button() == Qt::RightButton)
	{
		m_rightPushDown = true;
		m_mouseCordx = event->x();
		m_mouseCordy = event->y();
    }

}

void QOpenglMain::mouseMoveEvent(QMouseEvent * event)
{
    if(0 == m_frameType)
    {
        if (m_leftPushDown)
        {
            float _roatx = m_mouseCordx - event->x();
            float _roaty = m_mouseCordy - event->y();
            m_mouseCordx = event->x();
            m_mouseCordy = event->y();
            switch (m_drawtype) {
            case CChart::DRAWTYPE::XZY:
                m_chart.setRot(_roaty / 200,  _roatx/ 200/*,0*/);
                break;
            default:
                break;
            }

        }
    }
	if (m_rightPushDown)
	{
		float _movex = ((float)m_mouseCordx - event->x())/width();
		float _movey = ((float)m_mouseCordy - event->y())/height();
		m_mouseCordx = event->x();
		m_mouseCordy = event->y();
		m_chart.setMove(-_movex, _movey);
	}
    update();
}

void QOpenglMain::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
	{
		m_leftPushDown = false;
	}
	if (event->button() == Qt::RightButton)
	{
		m_rightPushDown = false;
	}
}

void QOpenglMain::wheelEvent(QWheelEvent * event)
{
	if (event->delta()>0)
	{
		m_zoom *= 1.1;
	}
	else {
		m_zoom *= 0.9;
	}
    m_chart.setZoom(m_zoom);
    update();
}

void QOpenglMain::keyPressEvent(QKeyEvent *event)
{
    if (Qt::Key_G==event->key())
    {
        //setGridOnOrOff();
    }
    if (Qt::Key_L==event->key())
    {
        //setAnixLabelOnOrOff();
    }
}

void QOpenglMain::onSetGridState()
{
    setGridOnOrOff();
}

void QOpenglMain::onSetAnixState()
{
    setAnixLabelOnOrOff();
}

void QOpenglMain::onXYPlaneState()
{
    setLineXYPlaneOn();
}

void QOpenglMain::onXZPlaneState()
{
    setLineXZPlaneOn();
}

void QOpenglMain::onYZPlaneState()
{
    setLineYZPlaneOn();
}
