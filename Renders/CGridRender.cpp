#include "Renders/CGridRender.h"


CGridRender::CGridRender()
{

}

CGridRender::~CGridRender()
{

}

void CGridRender::prepare()
{
    initializeOpenGLFunctions();
    initialShader();
}

void CGridRender::bindShader()
{
    m_shaderprogram.bind();
}

void CGridRender::bindAttributes()
{
    m_shaderprogram.bindAttributeLocation("Position", 0);
    m_shaderprogram.bindAttributeLocation("textureCoords", 1);
}

void CGridRender::setBackGround(vector4f &_color)
{
    glClearColor(_color._r, _color._g, _color._b, _color._a);
    glFlush();
}

void CGridRender::releaseShader()
{
    m_shaderprogram.release();
}

QGLShaderProgram *CGridRender::getShader()
{
    return &m_shaderprogram;
}

void CGridRender::initialShader()
{
    if (!m_shaderprogram.addShaderFromSourceFile(QGLShader::Vertex, "Shaders/coord.vert"))
    {
        QMessageBox::information(NULL, "vert",
            m_shaderprogram.log(),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    }
    if (!m_shaderprogram.addShaderFromSourceFile(QGLShader::Fragment, "Shaders/coord.frag"))
    {
        QMessageBox::information(NULL, "Frag",
            m_shaderprogram.log(),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    }
    if (!m_shaderprogram.link())
    {
        QMessageBox::information(NULL, "Frag",
            m_shaderprogram.log(),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    }
}

