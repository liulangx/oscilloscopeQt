#include "MyOpenglWidget/Clines.h"



Clines::Clines(CAllocVBOandVAO *allocVBOandVAO)
    : m_shadermin(0),
      m_shadermax(0)
{
    m_allocVBOandVAO = allocVBOandVAO;
}


Clines::~Clines()
{
    this->clearUp();
}

void Clines::initial(CLinesRender * _render, size_t _pointsize, QOpenGLContext* _context)
{
    initializeOpenGLFunctions();

    m_linesRender = _render;
    m_pointSize = _pointsize;
    m_context = _context;
    m_context->makeCurrent(m_context->surface());
    m_persize = sizeof(vector3f);//位置3+颜色4

    //顶点坐标绑定
    m_vao = m_allocVBOandVAO->allocVAO();
    glBindVertexArray(m_vao);
    m_vbo = m_allocVBOandVAO->allocVBO();
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 m_pointSize * m_persize,			//颜色和位置
                 nullptr,
                 GL_DYNAMIC_DRAW);

    //glEnableVertexAttribArray(2);		//法相坐标
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          m_persize,
                          nullptr);
    glEnableVertexAttribArray(0);		//顶点坐标属性

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //glColor3f(1, 1, 0);

    if(!m_notCopyFromOtherOpenGLWidget)
    {    //updateLines();
        setLinesVboBufferData();
        //drawOnce();
    }

}

void Clines::setRenderParam(GLuint _uniformIndexXyPlaneMarkForLines, GLuint _uniformIndexXzPlaneMarkForLines, GLuint _uniformIndexYzPlaneMarkForLines, GLuint _uniformIndexXLowerForLines, GLuint _uniformIndexYLowerForLines, GLuint _uniformIndexZLowerForLines)
{
    m_uniformIndexXyPlaneMarkForLines = _uniformIndexXyPlaneMarkForLines;
    m_uniformIndexXzPlaneMarkForLines = _uniformIndexXzPlaneMarkForLines;
    m_uniformIndexYzPlaneMarkForLines = _uniformIndexYzPlaneMarkForLines;
    m_uniformIndexXLowerForLines = _uniformIndexXLowerForLines;
    m_uniformIndexYLowerForLines = _uniformIndexYLowerForLines;
    m_uniformIndexZLowerForLines = _uniformIndexZLowerForLines;
}


void Clines::addLine(const vector3f & _posion, GLfloat _xLower, GLfloat _yLower, GLfloat _zLower)
{
    m_xLower = _xLower;
    m_yLower = _yLower;
    m_zLower = _zLower;
    m_context->makeCurrent(m_context->surface());

	if (m_pointCount >= m_pointSize)
	{
		QMessageBox::information(NULL, "points",
			"存储点数到达最大值",
			QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
		m_pointCount = 0;
		return;
	}
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER,
		m_pointCount * m_persize,
		sizeof(vector3f),
		(GLvoid*)&_posion);
    //qDebug() << "1: " << (m_pointCount) * m_persize;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ++m_pointCount;

}

void Clines::setXYPlaneOn(/*GLboolean _b*/)
{
    if(m_XYPlaneOn)
    {
        m_XYPlaneOn = false;
    }
    else
    {
        m_XYPlaneOn = true;
    }
//    m_XYPlaneOn = _b;
}

void Clines::setXZPlaneOn(/*GLboolean _b*/)
{
    if(m_XZPlaneOn)
    {
        m_XZPlaneOn = false;
    }
    else
    {
        m_XZPlaneOn = true;
    }
    //m_XZPlaneOn = _b;
}

void Clines::setYZPlaneOn(/*GLboolean _b*/)
{
    if(m_YZPlaneOn)
    {
        m_YZPlaneOn = false;
    }
    else
    {
        m_YZPlaneOn = true;
    }
//    m_YZPlaneOn = _b;
}

void Clines::setLinesVboBufferData()
{
//    if(m_copyVboBuffer.size() != 0)
//        qDebug() << m_copyVboBuffer.at(0) << " " << m_copyVboBuffer.size();
//    qDebug() << m_context;
    m_context->makeCurrent(m_context->surface());
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER,
        0,
        m_pointCount * sizeof(vector3f),
        m_copyVboBuffer.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //qDebug() << m_pointCount;
}



GLuint Clines::getLinesVao()
{
    return m_vao;
}

GLuint Clines::getLinesVbo()
{
    return m_vbo;
}

size_t Clines::getPointsAmout() const
{
    return m_pointCount;
}

void Clines::setLinesVao(GLuint _linesVaos)
{
    m_vao = _linesVaos;
}

void Clines::setLinesVbo(GLuint _linesVbos)
{
    m_vbo = _linesVbos;
}

void Clines::setLinesVboBufferData(vector<GLfloat> &_linesBuffer, size_t _linesAmount)
{
    m_notCopyFromOtherOpenGLWidget = false;
    m_copyVboBuffer = _linesBuffer;
    m_pointCount = _linesAmount;
    //setLinesVboBufferData();
    //qDebug() << m_copyVboBuffer.size();
}

void Clines::updateLines()
{
    setLinesVboBufferData();
}

void Clines::setLowers(GLfloat _xLower, GLfloat _yLower, GLfloat _zLower)
{
    m_xLower = _xLower;
    m_yLower = _yLower;
    m_zLower = _zLower;
}

void Clines::draw()
{
    //qDebug() << "m_pointCount: " << m_pointCount;
    m_context->makeCurrent(m_context->surface());
    m_linesRender->bindShader();
    m_linesRender->getShader()->setUniformValue(m_uniformIndexXyPlaneMarkForLines, false);
    m_linesRender->getShader()->setUniformValue(m_uniformIndexXzPlaneMarkForLines, false);
    m_linesRender->getShader()->setUniformValue(m_uniformIndexYzPlaneMarkForLines, false);
    glBindVertexArray(m_vao);
    glLineWidth(1.5);
    glDrawArrays(GL_LINE_STRIP, 0, m_pointCount);
    glBindVertexArray(0);

    if(m_XYPlaneOn)
    {
        m_linesRender->getShader()->setUniformValue(m_uniformIndexXyPlaneMarkForLines, true);
        m_linesRender->getShader()->setUniformValue(m_uniformIndexXzPlaneMarkForLines, false);
        m_linesRender->getShader()->setUniformValue(m_uniformIndexYzPlaneMarkForLines, false);
        m_linesRender->getShader()->setUniformValue(m_uniformIndexZLowerForLines, m_zLower);
        glBindVertexArray(m_vao);
        glLineWidth(1);
        glDrawArrays(GL_LINE_STRIP, 0, m_pointCount);
        glBindVertexArray(0);
    }

    if(m_XZPlaneOn)
    {
        m_linesRender->getShader()->setUniformValue(m_uniformIndexXyPlaneMarkForLines, false);
        m_linesRender->getShader()->setUniformValue(m_uniformIndexXzPlaneMarkForLines, true);
        m_linesRender->getShader()->setUniformValue(m_uniformIndexYzPlaneMarkForLines, false);
        m_linesRender->getShader()->setUniformValue(m_uniformIndexYLowerForLines, m_yLower);
        glBindVertexArray(m_vao);
        glLineWidth(1);
        glDrawArrays(GL_LINE_STRIP, 0, m_pointCount);
        glBindVertexArray(0);
    }

    if(m_YZPlaneOn)
    {
        m_linesRender->getShader()->setUniformValue(m_uniformIndexXyPlaneMarkForLines, false);
        m_linesRender->getShader()->setUniformValue(m_uniformIndexXzPlaneMarkForLines, false);
        m_linesRender->getShader()->setUniformValue(m_uniformIndexYzPlaneMarkForLines, true);
        m_linesRender->getShader()->setUniformValue(m_uniformIndexXLowerForLines, m_xLower);
        glBindVertexArray(m_vao);
        glLineWidth(1);
        glDrawArrays(GL_LINE_STRIP, 0, m_pointCount);
        glBindVertexArray(0);
    }


    m_linesRender->releaseShader();
}

void Clines::drawOnce()
{
    m_context->makeCurrent(m_context->surface());
    m_linesRender->bindShader();
    glBindVertexArray(m_vao);
    glLineWidth(1.5);
    glDrawArrays(GL_LINE_STRIP, 0, m_pointCount);
    glBindVertexArray(0);
    m_linesRender->releaseShader();
}

void Clines::clearUp()
{
}

void Clines::setColor(vector4f color)
{
    m_color._r = color._r;
    m_color._g = color._g;
    m_color._b = color._b;
    m_color._a = color._a;
}

vector4f Clines::getColor() const
{
    return m_color;
}



