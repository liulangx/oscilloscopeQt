#include "MyOpenglWidget/Chart.h"

const float pi = 3.1415926;
const size_t linesLength = 24*3600*50;
void CChart::setRot(float _x, float _y)
{
    m_rotation = QMatrix4x4(1.0, 0.0, 0.0, 0.0,
					0.0, cos(_x), -sin(_x), 0.0,
					0.0, sin(_x), cos(_x), 0.0,
						0.0, 0.0, 0.0, 1.0)*
		QMatrix4x4(cos(_y), 0.0, sin(_y), 0.0,
			0.0, 1.0, 0.0, 0.0,
			-sin(_y), 0.0, cos(_y), 0.0,
			0.0, 0.0, 0.0, 1.0)/**
				QMatrix4x4(cos(_z), -sin(_z), 0.0, 0.0,
					sin(_z), cos(_z), 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
                    0.0, 0.0, 0.0, 1.0)*/*m_rotation;
    m_grid.setRot(m_rotation);
    m_grid.createAndUpdateAnixnumberWithPnt();
    m_stataChange = true;
}

void CChart::setRotAlone(float _x, float _y, float _z)
{
    m_rotation = QMatrix4x4(1.0, 0.0, 0.0, 0.0,
                    0.0, cos(_x), -sin(_x), 0.0,
                    0.0, sin(_x), cos(_x), 0.0,
                        0.0, 0.0, 0.0, 1.0)*
        QMatrix4x4(cos(_y), 0.0, sin(_y), 0.0,
            0.0, 1.0, 0.0, 0.0,
            -sin(_y), 0.0, cos(_y), 0.0,
            0.0, 0.0, 0.0, 1.0)/**
                QMatrix4x4(cos(_z), -sin(_z), 0.0, 0.0,
                    sin(_z), cos(_z), 0.0, 0.0,
                    0.0, 0.0, 1.0, 0.0,
                    0.0, 0.0, 0.0, 1.0)*/*m_rotation;
    m_grid.setRot(m_rotation);
    m_stataChange = true;
}

//void CChart::setXrange(float _max, float _min)
//{
//	m_xRange._x = _max;
//	m_xRange._y = _min;
//	m_stataChange = true;
//}

//void CChart::setYrange(float _max, float _min)
//{
//	m_yRange._x = _max;
//	m_yRange._y = _min;
//	m_stataChange = true;
//}

//void CChart::setZrange(float _max, float _min)
//{
//	m_zRange._x = _max;
//	m_zRange._y = _min;
//	m_stataChange = true;
//}

void CChart::setZoom(float _zoom)
{
	m_zoom = _zoom;
    //qDebug() << m_zoom;
    //通过鼠标中键放大图像时，本质上放大的上界减去下界的值，所以下界不变，上界为m_xRange._y + (m_xRange._x - m_xRange._y) / m_zoom
    vector2f xrange = {m_xRange._y + (m_xRange._x - m_xRange._y) / m_zoom, m_xRange._y};
    vector2f yrange = {m_yRange._y + (m_yRange._x - m_yRange._y) / m_zoom, m_yRange._y};
    vector2f zrange = {m_zRange._y + (m_zRange._x - m_zRange._y) / m_zoom, m_zRange._y};
    //更新网格
    m_grid.adjustGridNumber(xrange, yrange, zrange, CGrid::RangeChange::Change, CGrid::RangeChange::Change, CGrid::RangeChange::Change);

	m_stataChange = true;
}

void CChart::setMove(float _x, float _y)
{
    m_move = QMatrix4x4(1.0, 0.0, 0.0, _x,
                              0.0, 1.0, 0.0, _y,
                              0.0, 0.0, 1.0, 0.0,
                              0.0, 0.0, 0.0, 1.0) * m_move;

	m_stataChange = true;
}

void CChart::setDrawType(DRAWTYPE _type)
{
	m_drawtype = _type;
    m_grid.setDrawType(m_drawtype);
    switch (m_drawtype)
	{
		case XZY:
		{
			m_anix.setCordi({ 0,1 }, { 0,1 }, {0,1});
            m_grid.setCordi({ 0,1 }, { 0,1 }, {0,1});
			m_rotation = { 1,0,0,0,
						0,1,0,0,
						0,0,1,0,
						0,0,0,1 };
            m_move = { 1,0,0,0,
                       0,1,0,0,
                       0,0,1,0,
                       0,0,0,1 };
            setRot(0, 0);
			setMove(0, 0);
            m_context->makeCurrent(m_context->surface()); //ll 20170711
            m_therender.bindShader();			//绑定shader
            m_therender.getShader()->setUniformValue(m_uniformIndexRotPntAnixGrid, m_rotation);
            m_therender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGrid, m_move);
            m_therender.getShader()->setUniformValue(m_uniformIndexDrawtypePntAnixGrid, m_drawtype);
			m_therender.releaseShader();

            m_gridRender.bindShader();
            m_gridRender.getShader()->setUniformValue(m_uniformIndexRotAnixtexture, m_rotation);
            m_gridRender.getShader()->setUniformValue(m_uniformIndexMoveAnixtexture, m_move);
            m_gridRender.getShader()->setUniformValue(m_uniformIndexDrawtypeAnixtexture, m_drawtype);
            m_gridRender.releaseShader();

            m_linesRender.bindShader();			//绑定shader
            m_linesRender.getShader()->setUniformValue(m_uniformIndexRotPntAnixGridForLines, m_rotation);
            m_linesRender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGridForLines, m_move);
            m_linesRender.getShader()->setUniformValue(m_uniformIndexDrawtypePntAnixGridForLines, m_drawtype);
            m_linesRender.releaseShader();



			break;
		}
		case XY:
		{
			m_anix.setCordi({ -0.9f,0.9f }, { -0.9f,0.9f }, { 0,1 });
            m_grid.setCordi({ -0.9f,0.9f }, { -0.9f,0.9f }, { 0,1 });
			m_rotation = { 1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1 };
            m_move = { 1,0,0,0,
                       0,1,0,0,
                       0,0,1,0,
                       0,0,0,1 };
            setRot(0, 0);
			setMove(-0.9, -0.9);
            m_context->makeCurrent(m_context->surface()); //ll 20170711
            m_therender.bindShader();			//绑定shader
            m_therender.getShader()->setUniformValue(m_uniformIndexRotPntAnixGrid, m_rotation);
            m_therender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGrid, m_move);
            m_therender.getShader()->setUniformValue(m_uniformIndexDrawtypePntAnixGrid, m_drawtype);
			m_therender.releaseShader();

            m_gridRender.bindShader();
            m_gridRender.getShader()->setUniformValue(m_uniformIndexRotAnixtexture, m_rotation);
            m_gridRender.getShader()->setUniformValue(m_uniformIndexMoveAnixtexture, m_move);
            m_gridRender.getShader()->setUniformValue(m_uniformIndexDrawtypeAnixtexture, m_drawtype);
            m_gridRender.releaseShader();

            m_linesRender.bindShader();			//绑定shader
            m_linesRender.getShader()->setUniformValue(m_uniformIndexRotPntAnixGridForLines, m_rotation);
            m_linesRender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGridForLines, m_move);
            m_linesRender.getShader()->setUniformValue(m_uniformIndexDrawtypePntAnixGridForLines, m_drawtype);
            m_linesRender.releaseShader();
			break;
		}
		case XZ:
		{
			m_anix.setCordi({ -0.9f,0.9f }, { 0,1 }, { -0.9f,0.9f });
            m_grid.setCordi({ -0.9f,0.9f }, { 0,1 }, { -0.9f,0.9f });
			m_rotation = { 1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1 };
            m_move = { 1,0,0,0,
                       0,1,0,0,
                       0,0,1,0,
                       0,0,0,1 };
            setRot(-pi/2, 0);
			setMove(-0.9, -0.9);
            m_context->makeCurrent(m_context->surface()); //ll 20170711
            m_therender.bindShader();			//绑定shader
            m_therender.getShader()->setUniformValue(m_uniformIndexRotPntAnixGrid, m_rotation);
            m_therender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGrid, m_move);
            m_therender.getShader()->setUniformValue(m_uniformIndexDrawtypePntAnixGrid, m_drawtype);
            m_therender.releaseShader();

            m_gridRender.bindShader();
            m_gridRender.getShader()->setUniformValue(m_uniformIndexRotAnixtexture, m_rotation);
            m_gridRender.getShader()->setUniformValue(m_uniformIndexMoveAnixtexture, m_move);
            m_gridRender.getShader()->setUniformValue(m_uniformIndexDrawtypeAnixtexture, m_drawtype);
            m_gridRender.releaseShader();

            m_linesRender.bindShader();			//绑定shader
            m_linesRender.getShader()->setUniformValue(m_uniformIndexRotPntAnixGridForLines, m_rotation);
            m_linesRender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGridForLines, m_move);
            m_linesRender.getShader()->setUniformValue(m_uniformIndexDrawtypePntAnixGridForLines, m_drawtype);
            m_linesRender.releaseShader();
			break;
		}
		case YZ:
		{
			m_anix.setCordi({ 0,1 }, { -0.9f,0.9f }, { -0.9f,0.9f });
            m_grid.setCordi({ 0,1 }, { -0.9f,0.9f }, { -0.9f,0.9f });
			m_rotation = { 1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1 };
            m_move = { 1,0,0,0,
                       0,1,0,0,
                       0,0,1,0,
                       0,0,0,1 };
            setRot(0, pi/2);
			setMove(-0.9, -0.9);
            m_context->makeCurrent(m_context->surface()); //ll 20170711
            m_therender.bindShader();			//绑定shader
            m_therender.getShader()->setUniformValue(m_uniformIndexRotPntAnixGrid, m_rotation);
            m_therender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGrid, m_move);
            m_therender.getShader()->setUniformValue(m_uniformIndexDrawtypePntAnixGrid, m_drawtype);
            m_therender.releaseShader();

            m_gridRender.bindShader();
            m_gridRender.getShader()->setUniformValue(m_uniformIndexRotAnixtexture, m_rotation);
            m_gridRender.getShader()->setUniformValue(m_uniformIndexMoveAnixtexture, m_move);
            m_gridRender.getShader()->setUniformValue(m_uniformIndexDrawtypeAnixtexture, m_drawtype);
            m_gridRender.releaseShader();

            m_linesRender.bindShader();			//绑定shader
            m_linesRender.getShader()->setUniformValue(m_uniformIndexRotPntAnixGridForLines, m_rotation);
            m_linesRender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGridForLines, m_move);
            m_linesRender.getShader()->setUniformValue(m_uniformIndexDrawtypePntAnixGridForLines, m_drawtype);
            m_linesRender.releaseShader();
			break;
		}
	}

}

void CChart::getDrawType(CChart::DRAWTYPE &_type)
{
    _type = m_drawtype;
}

void CChart::getGridNumer(int &_xGridNumPerAnix, int &_yGridNumPerAnix, int &_zGridNumPerAnix)
{
    m_grid.getGridNumber(_xGridNumPerAnix, _yGridNumPerAnix, _zGridNumPerAnix);
}

void CChart::setGridNumer(int _xGridNumPerAnix, int _yGridNumPerAnix, int _zGridNumPerAnix)
{
    m_grid.setGridNumber(_xGridNumPerAnix, _yGridNumPerAnix, _zGridNumPerAnix);
}

void CChart::setLinesVboBufferData(list<vector<GLfloat> > &_linesBuffer, vector<size_t> & _linesAmount)
{
    for(auto obj : m_lines)
    {
        delete obj.second;
    }
    m_lines.clear();
    int count = 0;
    Clines * tmpLine = nullptr;
    for(list<vector<GLfloat>>::iterator it = _linesBuffer.begin(); it != _linesBuffer.end(); ++it)
    {
        addLine(count, linesLength); //必须修改为对应的线段编号
        //Clines * tmpLine = m_lines.at(count);
        tmpLine = m_lines.at(count);
        tmpLine->setLinesVboBufferData((*it), _linesAmount.at(count));
        count ++;
    }
}

map<u_short, vector4f> CChart::getLineColor()
{
    map<u_short, vector4f> lineColors;
    vector4f tmpColor;
    for(auto obj : m_lines )
    {
        tmpColor = obj.second->getColor();
        lineColors.insert(std::pair<u_short, vector4f>(obj.first, tmpColor));
    }
    return lineColors;
}

void CChart::setLineColor(map<u_short, vector4f> &_lineColors)
{
//    map<u_short, vector4f>::iterator it = _lineColors.begin();
    for(auto obj : m_lines )
    {
        obj.second->setColor(_lineColors.at(obj.first));
//        ++it;
    }
}

vector<size_t> CChart::getLinesPointsAmount()
{
    vector<size_t> tmp;
    size_t tmpAmount = 0;
    for(map<size_t, Clines*>::iterator it = m_lines.begin(); it != m_lines.end(); ++it)
    {
        tmpAmount = /*(*it)*/it->second->getPointsAmout();
        tmp.push_back(tmpAmount);
    }
    return tmp;
}

void CChart::updateLines()
{
    for(auto obj : m_lines )
    {
        obj.second->updateLines();
    }
}

bool CChart::checkRangeChange(vector2f& _xRange, vector2f& _yRange, vector2f& _zRange)
{
    if(m_RangeChange)
    {
        _xRange._x = m_xRange._x;
        _xRange._y = m_xRange._y;

        _yRange._x = m_yRange._x;
        _yRange._y = m_yRange._y;

        _zRange._x = m_zRange._x;
        _zRange._y = m_zRange._y;
        m_RangeChange = false;
        return true;
    }
    else
    {
        return false;
    }
}

void CChart::setLinesEnabled(size_t _lineIndex, bool _b)
{
    m_linesEnabled.at(_lineIndex) = _b;
}

void CChart::setDrawTypeAlone(CChart::DRAWTYPE &_type)
{
   m_drawtype = _type;
   m_grid.setDrawType(_type);
   switch (_type)
   {
       case XZY:
       {
           m_anix.setCordiAlone({ 0,1 }, { 0,1 }, {0,1});
           m_grid.setCordiAlone({ 0,1 }, { 0,1 }, {0,1});
           m_rotation = { 1,0,0,0,
               0,1,0,0,
               0,0,1,0,
               0,0,0,1 };
           m_move = { 1,0,0,0,
                      0,1,0,0,
                      0,0,1,0,
                      0,0,0,1 };
//           setRotAlone(0, 0, 0);
//           setMove(0, 0);
           break;
       }
       case XY:
       {
           m_anix.setCordiAlone({ -0.9f,0.9f }, { -0.9f,0.9f }, { 0,1 });
           m_grid.setCordiAlone({ -0.9f,0.9f }, { -0.9f,0.9f }, { 0,1 });
           m_rotation = { 1,0,0,0,
               0,1,0,0,
               0,0,1,0,
               0,0,0,1 };
           m_move = { 1,0,0,0,
                      0,1,0,0,
                      0,0,1,0,
                      0,0,0,1 };
           setRotAlone(0, 0, 0);
           setMove(-0.9, -0.9);
           break;
       }
       case XZ:
       {
           m_anix.setCordiAlone({ -0.9f,0.9f }, { 0,1 }, { -0.9f,0.9f });
           m_grid.setCordiAlone({ -0.9f,0.9f }, { 0,1 }, { -0.9f,0.9f });
           m_rotation = { 1,0,0,0,
               0,1,0,0,
               0,0,1,0,
               0,0,0,1 };
           m_move = { 1,0,0,0,
                      0,1,0,0,
                      0,0,1,0,
                      0,0,0,1 };
//           setRotAlone(-pi/2, 0, 0);
           //setMove(-0.9, -0.9);
           break;
       }
       case YZ:
       {
           m_anix.setCordiAlone({ 0,1 }, { -0.9f,0.9f }, { -0.9f,0.9f });
           m_grid.setCordiAlone({ 0,1 }, { -0.9f,0.9f }, { -0.9f,0.9f });
           m_rotation = { 1,0,0,0,
               0,1,0,0,
               0,0,1,0,
               0,0,0,1 };
           m_move = { 1,0,0,0,
                      0,1,0,0,
                      0,0,1,0,
                      0,0,0,1 };
           //setRotAlone(0, pi/2, 0);
           //setMove(-0.9, -0.9);
           break;
       }
   }
}

void CChart::setColor(size_t _lineIndex, vector4f _color)
{
    m_lines.at(_lineIndex)->setColor(_color);
}

void CChart::setGridOnOrOff()
{
    if(m_gridOn)
    {
        m_grid.setGridOff();
        m_gridOn = false;
    }
    else
    {
        m_grid.setGridOn();
        m_gridOn = true;
    }

}

void CChart::setGridOnOrOff(bool _b)
{
    m_gridOn = _b;
    if(m_gridOn)
    {
        m_grid.setGridOn();
    }
    else
    {
        m_grid.setGridOff();
    }
}

void CChart::setAnixLabelOnOrOff()
{
    if(m_anixLabelOn)
    {
        m_grid.setAnixLabelOff();
        m_anixLabelOn = false;
    }
    else
    {
        m_grid.setAnixLabelOn();
        m_anixLabelOn = true;
    }
}

void CChart::setAxisPrecision(GLint _precision)
{
    m_grid.setPrecision(_precision);
}

void CChart::setLineXYPlaneOn()
{
    for(auto line : m_lines)
    {
        line.second->setXYPlaneOn(/*_b*/);
    }
}

void CChart::setLineXZPlaneOn(/*GLboolean _b*/)
{
    for(auto line : m_lines)
    {
        line.second->setXZPlaneOn(/*_b*/);
    }
}

void CChart::setLineYZPlaneOn(/*GLboolean _b*/)
{
    for(auto line : m_lines)
    {
        line.second->setYZPlaneOn(/*_b*/);
    }
}

void CChart::setWidthAndHeight(size_t width, size_t height)
{
    m_width = width;
    m_height = height;
}

void CChart::setFontSize(GLfloat _fontSize)
{
    m_grid.setFontSize(_fontSize);
}

void CChart::setCopyFromOtherOpenWidgetMark(GLboolean _b)
{
    m_notCopyFromOtherOpenGLWidget = _b;
    m_grid.setCopyFromOtherOpenWidgetMark(_b);
    m_anix.setCopyFromOtherOpenWidgetMark(_b);
}

GLuint CChart::getAnixVao()
{
    return m_anix.getAnixVao();
}

GLuint CChart::getAnixVbo()
{
    return m_anix.getAnixVbo();
}

GLuint CChart::getAnixVboIndex()
{
    return m_anix.getAnixVboIndex();
}

GLuint CChart::getVaoGridAnixnumDgt()
{
    return m_grid.getVaoGridAnixnumDgt();
}

GLuint CChart::getVboGridAnixnumDgt()
{
    return m_grid.getVboGridAnixnumDgt();
}

GLuint CChart::getVboGridAnixnumDgtTexture()
{
    return m_grid.getVboGridAnixnumDgtTexture();
}

GLuint CChart::getVaoGridAnixnumPnt()
{
    return m_grid.getVaoGridAnixnumPnt();
}

GLuint CChart::getVboGridAnixnumPnt()
{
    return m_grid.getVboGridAnixnumPnt();
}

GLuint CChart::getVboGridAnixnumPntTexture()
{
    return m_grid.getVboGridAnixnumPntTexture();
}

GLuint CChart::getVaoGridAnixnumMinus()
{
    return m_grid.getVaoGridAnixnumMinus();
}

GLuint CChart::getVboGridAnixnumMinus()
{
    return m_grid.getVboGridAnixnumMinus();
}

GLuint CChart::getVboGridAnixnumMinusTexture()
{
    return m_grid.getVboGridAnixnumMinusTexture();
}

GLuint CChart::getVaoGridAnixnumLabel()
{
    return m_grid.getVaoGridAnixnumLabel();
}

GLuint CChart::getVboGridAnixnumLabel()
{
    return m_grid.getVboGridAnixnumLabel();
}

GLuint CChart::getVboGridAnixnumLabelTexture()
{
    return m_grid.getVboGridAnixnumLabelTexture();
}

list<GLuint> CChart::getLinesVao()
{
    m_lineVaos.clear();
    for(auto line : m_lines)
    {
        m_lineVaos.push_back(line.second->getLinesVao());
    }
    return m_lineVaos;
}

list<GLuint> CChart::getLinesVbo()
{
    m_lineVbos.clear();
    for(auto line : m_lines)
    {
        m_lineVbos.push_back(line.second->getLinesVbo());
    }
    return m_lineVbos;
}

void CChart::setAnixVboBuffer(vector<GLfloat>& _anixVboBuffer)
{
    m_anix.setAnixVboBuffer(_anixVboBuffer);
}

void CChart::setAnixVboIndexBuffer(vector<GLuint>& _anixVboIndexBuffer)
{
    m_anix.setAnixVboIndexBuffer(_anixVboIndexBuffer);
}

void CChart::setVaoGridAnixnumDgt(GLuint _gridVaoGridAnixnumDgt)
{
    m_grid.setVaoGridAnixnumDgt(_gridVaoGridAnixnumDgt);
}

void CChart::setVboGridAnixnumDgt(GLuint _gridVboGridAnixnumDgt)
{
    m_grid.setVboGridAnixnumDgt(_gridVboGridAnixnumDgt);
}

void CChart::setVboGridAnixnumDgtTexture(GLuint _gridVboGridAnixnumDgtTexture)
{
    m_grid.setVboGridAnixnumDgtTexture(_gridVboGridAnixnumDgtTexture);
}

void CChart::setVaoGridAnixnumPnt(GLuint _gridVaoGridAnixnumPnt)
{
    m_grid.setVaoGridAnixnumPnt(_gridVaoGridAnixnumPnt);
}

void CChart::setVboGridAnixnumPnt(GLuint _gridVboGridAnixnumPnt)
{
    m_grid.setVboGridAnixnumPnt(_gridVboGridAnixnumPnt);
}

void CChart::setVboGridAnixnumPntTexture(GLuint _gridVboGridAnixnumPntTexture)
{
    m_grid.setVboGridAnixnumPntTexture(_gridVboGridAnixnumPntTexture);
}

void CChart::setVaoGridAnixnumMinus(GLuint _gridVaoGridAnixnumMinus)
{
    m_grid.setVaoGridAnixnumMinus(_gridVaoGridAnixnumMinus);
}

void CChart::setVboGridAnixnumMinus(GLuint _gridVboGridAnixnumMinus)
{
   m_grid.setVboGridAnixnumMinus(_gridVboGridAnixnumMinus);
}

void CChart::setVboGridAnixnumMinusTexture(GLuint _gridVboGridAnixnumMinusTexture)
{
    m_grid.setVboGridAnixnumMinusTexture(_gridVboGridAnixnumMinusTexture);
}

void CChart::setVaoGridAnixnumLabel(GLuint _gridVaoGridAnixnumLabel)
{
    m_grid.setVaoGridAnixnumLabel(_gridVaoGridAnixnumLabel);
}

void CChart::setVboGridAnixnumLabel(GLuint _gridVboGridAnixnumLabel)
{
    m_grid.setVboGridAnixnumLabel(_gridVboGridAnixnumLabel);
}

void CChart::setVboGridAnixnumLabelTexture(GLuint _gridVboGridAnixnumLabelTexture)
{
    m_grid.setVboGridAnixnumLabelTexture(_gridVboGridAnixnumLabelTexture);
}

void CChart::setLinesVao(list<GLuint> _linesVaos)
{
    size_t count1 = 0;
    for(list<GLuint>::iterator it = _linesVaos.begin(); it != _linesVaos.end(); ++it)
    {
        addLine(count1, linesLength);
        count1++;
    }
    int count = 0;
    for(list<GLuint>::iterator it = _linesVaos.begin(); it != _linesVaos.end(); ++it)
    {
        m_lines.at(count)->setLinesVao((*it));
        count ++;
    }
}

void CChart::setLinesVbo(list<GLuint> _linesVbos)
{
    int count = 0;
    for(list<GLuint>::iterator it = _linesVbos.begin(); it != _linesVbos.end(); ++it)
    {
        m_lines.at(count)->setLinesVbo((*it));
        count ++;
    }
}

void CChart::getCordRangei(vector2f &_x, vector2f &_y, vector2f &_z)
{
    m_grid.getCordRangei(_x, _y, _z);
}

void CChart::setCordRangei(vector2f &_x, vector2f &_y, vector2f &_z)
{
    m_xRange._x = _x._x;
    m_xRange._y = _x._y;
    m_yRange._x = _y._x;
    m_yRange._y = _y._y;
    m_zRange._x = _z._x;
    m_zRange._y = _z._y;


    m_grid.setCordRangei(_x, _y, _z);

}

void CChart::getRotation(QMatrix4x4 &_rotation)
{
    _rotation = m_rotation;
}

void CChart::setRotation(QMatrix4x4 &_rotation)
{
    m_rotation = _rotation * m_rotation;
    m_grid.setRot(m_rotation);
}

void CChart::getMove(QMatrix4x4 &_move)
{
    _move = m_move;
}

void CChart::setMove(QMatrix4x4 &_move)
{
    m_move = _move;
}
void CChart::setDrawAnx()
{
    m_context->makeCurrent(m_context->surface()); //ll 20170711
    m_therender.bindShader();
    m_therender.getShader()->setUniformValue(m_uniformIndexChosePntAnixGrid, 0);
    m_therender.getShader()->setUniformValue(m_uniformIndexDrawtypePntAnixGrid, m_drawtype);
	switch (m_drawtype)
	{
	case XZY:
		{
			m_stataChange = true;
			if (m_stataChange)
			{
                QMatrix4x4 tmpProjection = {
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                };
                tmpProjection.perspective(12.0, 1, 5, 20);
                //tmpProjection.ortho(-1.4f,1.4f,-1.4f,1.4f,-1.4f,1.4f);

                m_therender.getShader()->setUniformValue(m_uniformIndexRangeXPntAnixGrid, m_xRange._x, m_xRange._y);
                m_therender.getShader()->setUniformValue(m_uniformIndexRangeYPntAnixGrid, m_yRange._x, m_yRange._y);
                m_therender.getShader()->setUniformValue(m_uniformIndexRangeZPntAnixGrid, m_zRange._x, m_zRange._y);
                m_therender.getShader()->setUniformValue(m_uniformIndexRotPntAnixGrid, m_rotation);
                m_therender.getShader()->setUniformValue(m_uniformIndexScalePntAnixGrid, m_zoom);
                m_therender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGrid, m_move);
                m_therender.getShader()->setUniformValue(m_uniformIndexBasecordPntAnixGrid, m_cordOrigin._x, m_cordOrigin._y, m_cordOrigin._z);
                m_therender.getShader()->setUniformValue(m_uniformIndexProjectionPntAnixGrid, tmpProjection);
				m_stataChange = false;
			}
			break;
		}
	case XY:
		{
			m_stataChange = true;
			if (m_stataChange)
			{
                QMatrix4x4 tmpProjection = {
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                };
                //tmpProjection.perspective(12.0, 1, 5, 20);
                tmpProjection.ortho(-1.4f,1.4f,-1.4f,1.4f,-1.4f,1.4f);
                m_therender.getShader()->setUniformValue(m_uniformIndexRangeXPntAnixGrid, m_xRange._x, m_xRange._y);
                m_therender.getShader()->setUniformValue(m_uniformIndexRangeYPntAnixGrid, m_yRange._x, m_yRange._y);
                m_therender.getShader()->setUniformValue(m_uniformIndexScalePntAnixGrid, m_zoom);
                m_therender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGrid, m_move);
                m_therender.getShader()->setUniformValue(m_uniformIndexBasecordPntAnixGrid, m_cordOrigin._x, m_cordOrigin._y, m_cordOrigin._z);
                m_therender.getShader()->setUniformValue(m_uniformIndexProjectionPntAnixGrid, tmpProjection);
				m_stataChange = false;
			}
			break;
		}
	case XZ:
		{
			m_stataChange = true;
			if (m_stataChange)
			{
                QMatrix4x4 tmpProjection = {
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                };
                //tmpProjection.perspective(12.0, 1, 5, 20);
                tmpProjection.ortho(-1.4f,1.4f,-1.4f,1.4f,-1.4f,1.4f);
                m_therender.getShader()->setUniformValue(m_uniformIndexRangeXPntAnixGrid, m_xRange._x, m_xRange._y);
                m_therender.getShader()->setUniformValue(m_uniformIndexRangeZPntAnixGrid, m_zRange._x, m_zRange._y);
                m_therender.getShader()->setUniformValue(m_uniformIndexScalePntAnixGrid, m_zoom);
                m_therender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGrid, m_move);
                m_therender.getShader()->setUniformValue(m_uniformIndexBasecordPntAnixGrid, m_cordOrigin._x, m_cordOrigin._y, m_cordOrigin._z);
                m_therender.getShader()->setUniformValue(m_uniformIndexProjectionPntAnixGrid, tmpProjection);
				m_stataChange = false;
			}
			break;
		}
	case YZ:
		{
			m_stataChange = true;
			if (m_stataChange)
			{
                QMatrix4x4 tmpProjection = {
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                };
                //tmpProjection.perspective(12.0, 1, 5, 20);
                tmpProjection.ortho(-1.4f,1.4f,-1.4f,1.4f,-1.4f,1.4f);
                m_therender.getShader()->setUniformValue(m_uniformIndexRangeYPntAnixGrid, m_yRange._x, m_yRange._y);
                m_therender.getShader()->setUniformValue(m_uniformIndexRangeZPntAnixGrid, m_zRange._x, m_zRange._y);
                m_therender.getShader()->setUniformValue(m_uniformIndexScalePntAnixGrid, m_zoom);
                m_therender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGrid, m_move);
                m_therender.getShader()->setUniformValue(m_uniformIndexBasecordPntAnixGrid, m_cordOrigin._x, m_cordOrigin._y, m_cordOrigin._z);
                m_therender.getShader()->setUniformValue(m_uniformIndexProjectionPntAnixGrid, tmpProjection);
				m_stataChange = false;
			}
			break;
		}
	}
    m_therender.releaseShader();

    m_linesRender.bindShader();
    m_linesRender.getShader()->setUniformValue(m_uniformIndexChosePntAnixGridForLines, 0);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexDrawtypePntAnixGridForLines, m_drawtype);
    switch (m_drawtype)
    {
    case XZY:
        {
            m_stataChange = true;
            if (m_stataChange)
            {
                QMatrix4x4 tmpProjection = {
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                };
                tmpProjection.perspective(12.0, 1, 5, 20);
                //tmpProjection.ortho(-1.4f,1.4f,-1.4f,1.4f,-1.4f,1.4f);


                m_linesRender.getShader()->setUniformValue(m_uniformIndexRangeXPntAnixGridForLines, m_xRange._x, m_xRange._y);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexRangeYPntAnixGridForLines, m_yRange._x, m_yRange._y);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexRangeZPntAnixGridForLines, m_zRange._x, m_zRange._y);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexRotPntAnixGridForLines, m_rotation);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexScalePntAnixGridForLines, m_zoom);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGridForLines, m_move);
                //qDebug() << "m_move: "<< m_move(0,3) << m_move(1, 3) << m_move(2, 3);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexBasecordPntAnixGridForLines, m_cordOrigin._x, m_cordOrigin._y, m_cordOrigin._z);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexProjectionPntAnixGridForLines, tmpProjection);
                m_stataChange = false;
            }
            break;
        }
    case XY:
        {
            m_stataChange = true;
            if (m_stataChange)
            {
                QMatrix4x4 tmpProjection = {
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                };
                //tmpProjection.perspective(12.0, 1, 5, 20);
                tmpProjection.ortho(-1.4f,1.4f,-1.4f,1.4f,-1.4f,1.4f);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexRangeXPntAnixGridForLines, m_xRange._x, m_xRange._y);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexRangeYPntAnixGridForLines, m_yRange._x, m_yRange._y);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexScalePntAnixGridForLines, m_zoom);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGridForLines, m_move);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexBasecordPntAnixGridForLines, m_cordOrigin._x, m_cordOrigin._y, m_cordOrigin._z);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexProjectionPntAnixGridForLines, tmpProjection);
                m_stataChange = false;
            }
            break;
        }
    case XZ:
        {
            m_stataChange = true;
            if (m_stataChange)
            {
                QMatrix4x4 tmpProjection = {
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                };
                //tmpProjection.perspective(12.0, 1, 5, 20);
                tmpProjection.ortho(-1.4f,1.4f,-1.4f,1.4f,-1.4f,1.4f);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexRangeXPntAnixGridForLines, m_xRange._x, m_xRange._y);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexRangeZPntAnixGridForLines, m_zRange._x, m_zRange._y);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexScalePntAnixGridForLines, m_zoom);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGridForLines, m_move);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexBasecordPntAnixGridForLines, m_cordOrigin._x, m_cordOrigin._y, m_cordOrigin._z);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexProjectionPntAnixGridForLines, tmpProjection);
                m_stataChange = false;
            }
            break;
        }
    case YZ:
        {
            m_stataChange = true;
            if (m_stataChange)
            {
                QMatrix4x4 tmpProjection = {
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                };
                //tmpProjection.perspective(12.0, 1, 5, 20);
                tmpProjection.ortho(-1.4f,1.4f,-1.4f,1.4f,-1.4f,1.4f);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexRangeYPntAnixGridForLines, m_yRange._x, m_yRange._y);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexRangeZPntAnixGridForLines, m_zRange._x, m_zRange._y);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexScalePntAnixGridForLines, m_zoom);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGridForLines, m_move);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexBasecordPntAnixGridForLines, m_cordOrigin._x, m_cordOrigin._y, m_cordOrigin._z);
                m_linesRender.getShader()->setUniformValue(m_uniformIndexProjectionPntAnixGridForLines, tmpProjection);
                m_stataChange = false;
            }
            break;
        }
    }
    m_linesRender.releaseShader();

    m_gridRender.bindShader();
//    m_therender.getShader()->setUniformValue(m_uniformIndexChosePntAnixGrid, 0);
    //m_therender.getShader()->setUniformValue(m_uniformIndexDrawtypePntAnixGrid, m_drawtype);
    switch (m_drawtype)
    {
    case XZY:
        {
            m_stataChange = true;
            if (m_stataChange)
            {
                QMatrix4x4 tmpProjection = {
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                };
                tmpProjection.perspective(12.0, 1, 5, 20);
                //tmpProjection.ortho(-1.4f,1.4f,-1.4f,1.4f,-1.4f,1.4f);

                m_gridRender.getShader()->setUniformValue(m_uniformIndexRangeXAnixtexture, m_xRange._x, m_xRange._y);
                m_gridRender.getShader()->setUniformValue(m_uniformIndexRangeYAnixtexture, m_yRange._x, m_yRange._y);
                m_gridRender.getShader()->setUniformValue(m_uniformIndexRangeZAnixtexture, m_zRange._x, m_zRange._y);
                m_gridRender.getShader()->setUniformValue(m_uniformIndexRotAnixtexture, m_rotation);
                m_gridRender.getShader()->setUniformValue(m_uniformIndexMoveAnixtexture, m_move);
                m_gridRender.getShader()->setUniformValue(m_uniformIndexBasecordAnixtexture, m_cordOrigin._x, m_cordOrigin._y, m_cordOrigin._z);
                m_therender.getShader()->setUniformValue(m_uniformIndexProjectionAnixtexture, tmpProjection);
                m_stataChange = false;
            }
            break;
        }
    case XY:
        {
            m_stataChange = true;
            if (m_stataChange)
            {
                QMatrix4x4 tmpProjection = {
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                };
                //tmpProjection.perspective(12.0, 1, 5, 20);
                tmpProjection.ortho(-1.4f,1.4f,-1.4f,1.4f,-1.4f,1.4f);
                m_gridRender.getShader()->setUniformValue(m_uniformIndexRangeXAnixtexture, m_xRange._x, m_xRange._y);
                m_gridRender.getShader()->setUniformValue(m_uniformIndexRangeYAnixtexture, m_yRange._x, m_yRange._y);
                m_gridRender.getShader()->setUniformValue(m_uniformIndexMoveAnixtexture, m_move);
                m_gridRender.getShader()->setUniformValue(m_uniformIndexBasecordAnixtexture, m_cordOrigin._x, m_cordOrigin._y, m_cordOrigin._z);
                m_therender.getShader()->setUniformValue(m_uniformIndexProjectionAnixtexture, tmpProjection);
                m_stataChange = false;
            }
            break;
        }
    case XZ:
        {
            m_stataChange = true;
            if (m_stataChange)
            {
                QMatrix4x4 tmpProjection = {
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                };
                //tmpProjection.perspective(12.0, 1, 5, 20);
                tmpProjection.ortho(-1.4f,1.4f,-1.4f,1.4f,-1.4f,1.4f);
                m_gridRender.getShader()->setUniformValue(m_uniformIndexRangeXAnixtexture, m_xRange._x, m_xRange._y);
                m_gridRender.getShader()->setUniformValue(m_uniformIndexRangeZAnixtexture, m_zRange._x, m_zRange._y);
                m_gridRender.getShader()->setUniformValue(m_uniformIndexMoveAnixtexture, m_move);
                m_gridRender.getShader()->setUniformValue(m_uniformIndexBasecordAnixtexture, m_cordOrigin._x, m_cordOrigin._y, m_cordOrigin._z);
                m_therender.getShader()->setUniformValue(m_uniformIndexProjectionAnixtexture, tmpProjection);
                m_stataChange = false;
            }
            break;
        }
    case YZ:
        {
            m_stataChange = true;
            if (m_stataChange)
            {
                QMatrix4x4 tmpProjection = {
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                };
                //tmpProjection.perspective(12.0, 1, 5, 20);
                tmpProjection.ortho(-1.4f,1.4f,-1.4f,1.4f,-1.4f,1.4f);
                m_gridRender.getShader()->setUniformValue(m_uniformIndexRangeYAnixtexture, m_yRange._x, m_yRange._y);
                m_gridRender.getShader()->setUniformValue(m_uniformIndexRangeZAnixtexture, m_zRange._x, m_zRange._y);
                m_gridRender.getShader()->setUniformValue(m_uniformIndexMoveAnixtexture, m_move);
                m_gridRender.getShader()->setUniformValue(m_uniformIndexBasecordAnixtexture, m_cordOrigin._x, m_cordOrigin._y, m_cordOrigin._z);
                m_therender.getShader()->setUniformValue(m_uniformIndexProjectionAnixtexture, tmpProjection);
                m_stataChange = false;
            }
            break;
        }
    }
    m_gridRender.releaseShader();
}

void CChart::setDrawLine(vector4f& _lineColor)
{
    m_context->makeCurrent(m_context->surface()); //ll 20170711
    m_therender.bindShader();
    m_therender.getShader()->setUniformValue(m_uniformIndexChosePntAnixGrid, 1);
    m_therender.getShader()->setUniformValue(m_uniformIndexDrawtypePntAnixGrid, m_drawtype);
    m_therender.getShader()->setUniformValue(m_uniformIndexLineColorPntAnixGrid, _lineColor._r, _lineColor._g, _lineColor._b, _lineColor._a);
    m_therender.releaseShader();

    m_linesRender.bindShader();
    m_linesRender.getShader()->setUniformValue(m_uniformIndexChosePntAnixGridForLines, 1);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexDrawtypePntAnixGridForLines, m_drawtype);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexLineColorPntAnixGridForLines, _lineColor._r, _lineColor._g, _lineColor._b, _lineColor._a);
    m_linesRender.releaseShader();

    m_gridRender.bindShader();
    m_gridRender.getShader()->setUniformValue(m_uniformIndexDrawtypeAnixtexture, m_drawtype);
    m_gridRender.releaseShader();
}

CChart::CChart()
{
    m_isfirstdata = true;
}


CChart::~CChart()
{
    //delete m_isFirstPoint;
    this->clearUp();
}

void CChart::initial(vector3f _location, QOpenGLContext* _context,QSurface* _surface)
{
   initializeOpenGLFunctions();

   qreal aspect = 1;
   const qreal zNear = 5, zFar = 20, fov = 12.0;
   m_projection.perspective(fov, aspect, zNear, zFar);
    //m_projection.ortho(-1.4f,1.4f,-1.4f,1.4f,-1.4f,1.4f);


    m_context = _context;
    m_surface = _surface;
    m_context->makeCurrent(m_surface);

    m_therender.prepare();
    m_gridRender.prepare();
    m_linesRender.prepare();
    //qDebug() << m_allocVBOandVAO;
    m_allocVBOandVAO->prepare(/*m_context*/);

    m_anix.setAlloc(m_allocVBOandVAO);
    m_anix.initial(&m_therender, m_context, m_surface);
    m_grid.setAlloc(m_allocVBOandVAO);
    m_grid.initial(&m_therender, &m_gridRender, m_context, m_surface);

    m_uniformIndexChosePntAnixGrid =  m_therender.getShader()->uniformLocation("chose");
    m_uniformIndexRangeXPntAnixGrid= m_therender.getShader()->uniformLocation("rangeX");
    m_uniformIndexRangeYPntAnixGrid= m_therender.getShader()->uniformLocation("rangeY");
    m_uniformIndexRangeZPntAnixGrid= m_therender.getShader()->uniformLocation("rangeZ");
    m_uniformIndexScalePntAnixGrid= m_therender.getShader()->uniformLocation("scale");
    m_uniformIndexRotPntAnixGrid= m_therender.getShader()->uniformLocation("rot");
    m_uniformIndexMovePntAnixGrid = m_therender.getShader()->uniformLocation("move");
    m_uniformIndexBasecordPntAnixGrid = m_therender.getShader()->uniformLocation("basecord");
    m_uniformIndexDrawtypePntAnixGrid = m_therender.getShader()->uniformLocation("drawtype");
    m_uniformIndexProjectionPntAnixGrid = m_therender.getShader()->uniformLocation("projection");
    m_uniformIndexViewLinefactorPntAnixGrid = m_therender.getShader()->uniformLocation("viewScale");
    m_uniformIndexLineColorPntAnixGrid = m_therender.getShader()->uniformLocation("lineColor");
	//setRot(0.2, 0.2, 0.2);
	m_therender.bindShader();			//����shader
    m_therender.getShader()->setUniformValue(m_uniformIndexRangeXPntAnixGrid, m_xRange._x, m_xRange._y);
    m_therender.getShader()->setUniformValue(m_uniformIndexRangeYPntAnixGrid, m_yRange._x, m_yRange._y);
    m_therender.getShader()->setUniformValue(m_uniformIndexRangeZPntAnixGrid, m_zRange._x, m_zRange._y);
    m_therender.getShader()->setUniformValue(m_uniformIndexRotPntAnixGrid, m_rotation);
    m_therender.getShader()->setUniformValue(m_uniformIndexScalePntAnixGrid, m_zoom);
    m_therender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGrid, m_move);
    m_therender.getShader()->setUniformValue(m_uniformIndexBasecordPntAnixGrid, m_cordOrigin._x,m_cordOrigin._y,m_cordOrigin._z);
    m_therender.getShader()->setUniformValue(m_uniformIndexDrawtypePntAnixGrid,0);
    m_therender.getShader()->setUniformValue(m_uniformIndexProjectionPntAnixGrid, m_projection);
    m_therender.getShader()->setUniformValue(m_uniformIndexViewLinefactorPntAnixGrid, m_viewLineFactor);
    m_therender.getShader()->setUniformValue(m_uniformIndexLineColorPntAnixGrid, m_lineColor._r, m_lineColor._g, m_lineColor._b, m_lineColor._a);
	m_therender.releaseShader();

    m_uniformIndexChosePntAnixGridForLines =  m_linesRender.getShader()->uniformLocation("chose");
    m_uniformIndexRangeXPntAnixGridForLines = m_linesRender.getShader()->uniformLocation("rangeX");
    m_uniformIndexRangeYPntAnixGridForLines = m_linesRender.getShader()->uniformLocation("rangeY");
    m_uniformIndexRangeZPntAnixGridForLines = m_linesRender.getShader()->uniformLocation("rangeZ");
    m_uniformIndexScalePntAnixGridForLines = m_linesRender.getShader()->uniformLocation("scale");
    m_uniformIndexRotPntAnixGridForLines = m_linesRender.getShader()->uniformLocation("rot");
    m_uniformIndexMovePntAnixGridForLines = m_linesRender.getShader()->uniformLocation("move");
    m_uniformIndexBasecordPntAnixGridForLines = m_linesRender.getShader()->uniformLocation("basecord");
    m_uniformIndexDrawtypePntAnixGridForLines = m_linesRender.getShader()->uniformLocation("drawtype");
    m_uniformIndexProjectionPntAnixGridForLines = m_linesRender.getShader()->uniformLocation("projection");
    m_uniformIndexViewLinefactorPntAnixGridForLines = m_linesRender.getShader()->uniformLocation("viewScale");
    m_uniformIndexLineColorPntAnixGridForLines = m_linesRender.getShader()->uniformLocation("lineColor");
    m_uniformIndexXyPlaneMarkForLines = m_linesRender.getShader()->uniformLocation("xyPlaneMark");
    m_uniformIndexXzPlaneMarkForLines = m_linesRender.getShader()->uniformLocation("xzPlaneMark");
    m_uniformIndexYzPlaneMarkForLines = m_linesRender.getShader()->uniformLocation("yzPlaneMark");
    m_uniformIndexXLowerForLines = m_linesRender.getShader()->uniformLocation("xLower");
    m_uniformIndexYLowerForLines = m_linesRender.getShader()->uniformLocation("yLower");
    m_uniformIndexZLowerForLines = m_linesRender.getShader()->uniformLocation("zLower");
    //setRot(0.2, 0.2, 0.2);
    m_linesRender.bindShader();			//����shader
    m_linesRender.getShader()->setUniformValue(m_uniformIndexRangeXPntAnixGridForLines, m_xRange._x, m_xRange._y);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexRangeYPntAnixGridForLines, m_yRange._x, m_yRange._y);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexRangeZPntAnixGridForLines, m_zRange._x, m_zRange._y);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexRotPntAnixGridForLines, m_rotation);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexScalePntAnixGridForLines, m_zoom);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexMovePntAnixGridForLines, m_move);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexBasecordPntAnixGridForLines, m_cordOrigin._x,m_cordOrigin._y,m_cordOrigin._z);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexDrawtypePntAnixGridForLines,0);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexProjectionPntAnixGridForLines, m_projection);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexViewLinefactorPntAnixGridForLines, m_viewLineFactor);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexLineColorPntAnixGridForLines, m_lineColor._r, m_lineColor._g, m_lineColor._b, m_lineColor._a);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexXyPlaneMarkForLines, false);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexXzPlaneMarkForLines, false);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexYzPlaneMarkForLines, false);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexXLowerForLines, m_xRange._y);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexYLowerForLines, m_yRange._y);
    m_linesRender.getShader()->setUniformValue(m_uniformIndexZLowerForLines, m_zRange._y);

    m_linesRender.releaseShader();


    m_uniformIndexProjectionAnixtexture = m_gridRender.getShader()->uniformLocation("_projection");
    m_uniformIndexMoveAnixtexture = m_gridRender.getShader()->uniformLocation("_move");
    m_uniformIndexRotAnixtexture = m_gridRender.getShader()->uniformLocation("_rot");
    m_uniformIndexDrawtypeAnixtexture = m_gridRender.getShader()->uniformLocation("_drawtype");
    m_uniformIndexRangeXAnixtexture = m_gridRender.getShader()->uniformLocation("_rangeX");
    m_uniformIndexRangeYAnixtexture = m_gridRender.getShader()->uniformLocation("_rangeY");
    m_uniformIndexRangeZAnixtexture = m_gridRender.getShader()->uniformLocation("_rangeZ");


    m_gridRender.bindShader();
    m_gridRender.getShader()->setUniformValue(m_uniformIndexProjectionAnixtexture, m_projection);
    m_gridRender.getShader()->setUniformValue(m_uniformIndexMoveAnixtexture, m_move);
    m_gridRender.getShader()->setUniformValue(m_uniformIndexRotAnixtexture, m_rotation);
    m_gridRender.getShader()->setUniformValue(m_uniformIndexDrawtypeAnixtexture, m_drawtype);
    m_gridRender.getShader()->setUniformValue(m_uniformIndexRangeXAnixtexture, m_xRange._x, m_xRange._y);
    m_gridRender.getShader()->setUniformValue(m_uniformIndexRangeYAnixtexture, m_yRange._x, m_yRange._y);
    m_gridRender.getShader()->setUniformValue(m_uniformIndexRangeZAnixtexture, m_zRange._x, m_zRange._y);
    m_gridRender.releaseShader();


    if(m_notCopyFromOtherOpenGLWidget)
    {
        m_anix.setCordi({ _location._x,1 }, { _location._y,1 }, { _location._z,1 });
        setMove(_location._x,_location._y);
    }
    else
    {
        m_anix.setCordiAlone();
        //m_grid.setRot(m_rotation);
    }
    if(!m_notCopyFromOtherOpenGLWidget)
    {
        for(auto obj : m_lines)
        {
            //obj.second->initial(&m_therender, linesLength, m_context);
            obj.second->initial(&m_linesRender, linesLength, m_context);

            obj.second->setRenderParam(m_uniformIndexXyPlaneMarkForLines, m_uniformIndexXzPlaneMarkForLines, m_uniformIndexYzPlaneMarkForLines, m_uniformIndexXLowerForLines, m_uniformIndexYLowerForLines, m_uniformIndexZLowerForLines);
            obj.second->setLowers(m_xRange._y, m_yRange._y, m_zRange._y);
        }
    }

}

void CChart::draw()
{ 
    //ll if(!m_lines.size()) return;
    //m_therender.bindShader();			//����shader
    setDrawAnx();						//�趨��Ⱦ����Ⱦ
    m_anix.draw();
    m_grid.draw();
    if (m_scalechange)
        m_scalechange = false;
    for (auto line : m_lines)			//��������
    {
        if(m_linesEnabled.at(line.first))
        {
            vector4f _tmpColor = line.second->getColor();
            setDrawLine(_tmpColor);						//�趨��Ⱦ����Ⱦ
            line.second->draw();
        }
    }
    //m_therender.releaseShader();
}

void CChart::clearUp()
{
    for(auto line: m_lines)
    {
        delete line.second;
    }
    m_lines.clear();
}

size_t CChart::addLine(size_t _lineIndex, size_t _pointsize)
{
    Clines* lines = new Clines(m_allocVBOandVAO);
    if(m_notCopyFromOtherOpenGLWidget)
    {
        lines->initial(&m_linesRender, _pointsize, m_context);
        lines->setRenderParam(m_uniformIndexXyPlaneMarkForLines, m_uniformIndexXzPlaneMarkForLines, m_uniformIndexYzPlaneMarkForLines, m_uniformIndexXLowerForLines, m_uniformIndexYLowerForLines, m_uniformIndexZLowerForLines);
    }
    m_lines.insert(std::pair<size_t, Clines*>(_lineIndex, lines));
    m_linesEnabled.insert(std::pair<size_t, bool>(_lineIndex, true));
    return _lineIndex;
}

bool CChart::addPoint(size_t _lineindex, const vector3f & _position)
{
    bool createLineMark = false;

    if(m_notCopyFromOtherOpenGLWidget)
    {
        //当该编号的线段存在时添加

        if (m_lines.end() != m_lines.find(_lineindex))
        {
//            m_lines.at(_lineindex)->addLine(_position, m_xRange._y, m_yRange._y, m_zRange._y);

            if(m_first2Two)
            {
                float xfisrtPoint = (m_xRange._x + m_xRange._y) / 2.0f;
                float yfisrtPoint = (m_yRange._x + m_yRange._y) / 2.0f;
                float zfisrtPoint = (m_zRange._x + m_zRange._y) / 2.0f;
                if(xfisrtPoint < _position._x)
                {
                    m_xRange._x = _position._x;
                    m_xRange._y = xfisrtPoint;
                    m_RangeChange = true;
                }
                else if(xfisrtPoint > _position._x)
                {
                    m_xRange._y = _position._x;
                    m_xRange._x = xfisrtPoint;
                    m_RangeChange = true;
                }
                else {

                }

                if(yfisrtPoint < _position._y)
                {
                    m_yRange._x = _position._y;
                    m_yRange._y = yfisrtPoint;
                    m_RangeChange = true;
                }
                else if(yfisrtPoint > _position._y)
                {
                    m_yRange._y = _position._y;
                    m_yRange._x = yfisrtPoint;
                    m_RangeChange = true;
                }
                else {

                }

                if(zfisrtPoint < _position._z)
                {
                    m_zRange._x = _position._z;
                    m_zRange._y = zfisrtPoint;
                    m_RangeChange = true;
                }
                else if(zfisrtPoint > _position._z)
                {
                    m_zRange._y = _position._z;
                    m_zRange._x = zfisrtPoint;
                    m_RangeChange = true;
                }
                else {

                }
                m_first2Two = false;
                m_stataChange = true;
            }
            else
            {
                if (m_xRange._x < _position._x)
                {
                    if(_position._x >= 0)
                        m_xRange._x = _position._x*1.2;
                    else if(_position._x < 0)
                        m_xRange._x = _position._x*0.8;
                    m_stataChange = true;
                    m_xRanChange = CGrid::RangeChange::Change;
                    m_RangeChange = true;
                }
                if (m_xRange._y > _position._x)
                {
                    if(_position._x <= 0)
                        m_xRange._y = _position._x*1.2;
                    else if(_position._x > 0)
                        m_xRange._y = _position._x*0.8;
                    m_stataChange = true;
                    m_xRanChange = CGrid::RangeChange::Change;
                    m_RangeChange = true;
                }

                if (m_yRange._x < _position._y)
                {
                    if(_position._y >= 0)
                        m_yRange._x = _position._y*1.2;
                    else if(_position._y < 0)
                        m_yRange._x = _position._y*0.8;
                    m_stataChange = true;
                    m_yRanChange = CGrid::RangeChange::Change;
                    m_RangeChange = true;
                }
                if (m_yRange._y > _position._y)
                {
                    if(_position._y <= 0)
                        m_yRange._y = _position._y*1.2;
                    else if(_position._y > 0)
                        m_yRange._y = _position._y*0.8;
                    m_stataChange = true;
                    m_yRanChange = CGrid::RangeChange::Change;
                    m_RangeChange = true;
                }

                if (m_zRange._x < _position._z)
                {
                    if(_position._z >= 0)
                        m_zRange._x = _position._z*1.2;
                    else if(_position._z < 0)
                        m_zRange._x = _position._z*0.8;
                    m_stataChange = true;
                    m_zRanChange = CGrid::RangeChange::Change;
                    m_RangeChange = true;
                }
                if (m_zRange._y > _position._z)
                {
                    if(_position._z <= 0)
                        m_zRange._y = _position._z * 1.2;
                    else if(_position._z > 0)
                        m_zRange._y = _position._z * 0.8;
                    m_stataChange = true;
                    m_zRanChange = CGrid::RangeChange::Change;
                    m_RangeChange = true;
                }
                //qDebug() << "_position._z: " << _position._z;
                //qDebug() << "m_zRange._y: " << m_zRange._y;
            }
            m_lines.at(_lineindex)->addLine(_position, m_xRange._y, m_yRange._y, m_zRange._y);

        }
        //如未建立则创建后添加
        else
        {
//            m_isFirstData = true;
            if(m_lines.empty())
            {
                m_first2Two = true;

                m_xRange._x = _position._x + 0.5 * fabs(_position._x);
                m_xRange._y = _position._x - 0.5 * fabs(_position._x);

                m_yRange._x = _position._y + 0.5 * fabs(_position._y);
                m_yRange._y = _position._y - 0.5 * fabs(_position._y);

                m_zRange._x = _position._z + 0.5 * fabs(_position._z);
                m_zRange._y = _position._z - 0.5 * fabs(_position._z);

                m_xRanChange = CGrid::RangeChange::Change;
                m_yRanChange = CGrid::RangeChange::Change;
                m_zRanChange = CGrid::RangeChange::Change;

                m_stataChange = true;
                m_RangeChange = true;
            }
            else if(m_first2Two)
            {
                float xfisrtPoint = (m_xRange._x + m_xRange._y) / 2.0f;
                float yfisrtPoint = (m_yRange._x + m_yRange._y) / 2.0f;
                float zfisrtPoint = (m_zRange._x + m_zRange._y) / 2.0f;
                if(xfisrtPoint < _position._x)
                {
                    m_xRange._x = _position._x;
                    m_xRange._y = xfisrtPoint;
                    m_RangeChange = true;
                }
                else if(xfisrtPoint > _position._x)
                {
                    m_xRange._y = _position._x;
                    m_xRange._x = xfisrtPoint;
                    m_RangeChange = true;
                }
                else {

                }

                if(yfisrtPoint < _position._y)
                {
                    m_yRange._x = _position._y;
                    m_yRange._y = yfisrtPoint;
                    m_RangeChange = true;
                }
                else if(yfisrtPoint > _position._y)
                {
                    m_yRange._y = _position._y;
                    m_yRange._x = yfisrtPoint;
                    m_RangeChange = true;
                }
                else {

                }

                if(zfisrtPoint < _position._z)
                {
                    m_zRange._x = _position._z;
                    m_zRange._y = zfisrtPoint;
                    m_RangeChange = true;
                }
                else if(zfisrtPoint > _position._z)
                {
                    m_zRange._y = _position._z;
                    m_zRange._x = zfisrtPoint;
                    m_RangeChange = true;
                }
                else {

                }
                m_first2Two = false;
                m_stataChange = true;
            }
            else
            {
                if (m_xRange._x < _position._x)
                {
                    if(_position._x >= 0)
                        m_xRange._x = _position._x*1.2;
                    else if(_position._x < 0)
                        m_xRange._x = _position._x*0.8;
                    m_stataChange = true;
                    m_xRanChange = CGrid::RangeChange::Change;
                    m_RangeChange = true;
                }
                if (m_xRange._y > _position._x)
                {
                    if(_position._x <= 0)
                        m_xRange._y = _position._x*1.2;
                    else if(_position._x > 0)
                        m_xRange._y = _position._x*0.8;
                    m_stataChange = true;
                    m_xRanChange = CGrid::RangeChange::Change;
                    m_RangeChange = true;
                }

                if (m_yRange._x < _position._y)
                {
                    if(_position._y >= 0)
                        m_yRange._x = _position._y*1.2;
                    else if(_position._y < 0)
                        m_yRange._x = _position._y*0.8;
                    m_stataChange = true;
                    m_yRanChange = CGrid::RangeChange::Change;
                    m_RangeChange = true;
                }
                if (m_yRange._y > _position._y)
                {
                    if(_position._y <= 0)
                        m_yRange._y = _position._y*1.2;
                    else if(_position._y > 0)
                        m_yRange._y = _position._y*0.8;
                    m_stataChange = true;
                    m_yRanChange = CGrid::RangeChange::Change;
                    m_RangeChange = true;
                }

                if (m_zRange._x < _position._z)
                {
                    if(_position._z >= 0)
                        m_zRange._x = _position._z*1.2;
                    else if(_position._z < 0)
                        m_zRange._x = _position._z*0.8;
                    m_stataChange = true;
                    m_zRanChange = CGrid::RangeChange::Change;
                    m_RangeChange = true;
                }
                if (m_zRange._y > _position._z)
                {
                    if(_position._z <= 0)
                        m_zRange._y = _position._z * 1.2;
                    else if(_position._z > 0)
                        m_zRange._y = _position._z * 0.8;
                    m_stataChange = true;
                    m_zRanChange = CGrid::RangeChange::Change;
                    m_RangeChange = true;
                }
                //qDebug() << "_position._z: " << _position._z;
                //qDebug() << "m_zRange._y: " << m_zRange._y;
            }

            m_lines.at(addLine(_lineindex, linesLength))->addLine(_position, m_xRange._y, m_yRange._y, m_zRange._y);
            createLineMark = true;
        }

        //如果坐标系的上下界变化则更新网格
        if(m_stataChange)
        {
            //通过鼠标中键放大图像时，本质上放大的上界减去下界的值，所以下界不变，上界为m_xRange._y + (m_xRange._x - m_xRange._y) / m_zoom
            vector2f xrange = {m_xRange._y + (m_xRange._x - m_xRange._y) / m_zoom, m_xRange._y};
            vector2f yrange = {m_yRange._y + (m_yRange._x - m_yRange._y) / m_zoom, m_yRange._y};
            vector2f zrange = {m_zRange._y + (m_zRange._x - m_zRange._y) / m_zoom, m_zRange._y};
            //更新网格
            m_grid.adjustGridNumber(xrange, yrange, zrange, m_xRanChange, m_yRanChange, m_zRanChange);
            m_xRanChange = CGrid::RangeChange::NotChange;
            m_yRanChange = CGrid::RangeChange::NotChange;
            m_zRanChange = CGrid::RangeChange::NotChange;
        }
    }
    return createLineMark;
}

void CChart::updateChart()
{

}

void CChart::setAlloc(CAllocVBOandVAO *allocVBOandVAO)
{
    m_allocVBOandVAO = allocVBOandVAO;
}
