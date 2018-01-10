#include "Renders/CLinesRender.h"

CLinesRender::CLinesRender()
{
}


CLinesRender::~CLinesRender()
{
}

void CLinesRender::prepare()
{
    initializeOpenGLFunctions();
    QOpenGLFunctions_4_3_Core::glClearColor(1,1,1,0.2);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    initialShader();
}

void CLinesRender::bindShader()
{
    m_shaderprogram.bind();
}

void CLinesRender::releaseShader()
{
    m_shaderprogram.release();
}

GLuint CLinesRender::getUniformDataIndex(const char * _name)
{
    return GLuint(m_shaderprogram.uniformLocation(_name));
}

QGLShaderProgram * CLinesRender::getShader()
{
    return &m_shaderprogram;
}

void CLinesRender::initialShader()
{
    //shader程序
    if (!m_shaderprogram.addShaderFromSourceFile(QGLShader::Vertex, "Shaders/lines.vert"))
    {
        QMessageBox::information(NULL, "vert",
            m_shaderprogram.log(),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    }
    if (!m_shaderprogram.addShaderFromSourceFile(QGLShader::Geometry, "Shaders/lines.geo"))
    {
        QMessageBox::information(NULL, "geo",
            m_shaderprogram.log(),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    }
    if (!m_shaderprogram.addShaderFromSourceFile(QGLShader::Fragment, "Shaders/lines.Frag"))
    {
        QMessageBox::information(NULL, "Frag",
            m_shaderprogram.log(),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    }
    if (!m_shaderprogram.link())
    {
        QMessageBox::information(NULL, "Link",
            m_shaderprogram.log(),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    }
}

