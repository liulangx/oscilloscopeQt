#include "Renders/Crender.h"

#define ATTRIB_VERTEX 1

Crender::Crender()
{
}


Crender::~Crender()
{
}

void Crender::prepare()
{
    initializeOpenGLFunctions();
    QOpenGLFunctions_4_3_Core::glClearColor(1,1,1,0.2);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	initialShader();
}

void Crender::bindShader()
{
	m_shaderprogram.bind();
}

void Crender::releaseShader()
{
	m_shaderprogram.release();
}

GLuint Crender::getUniformDataIndex(const char * _name)
{
	return GLuint(m_shaderprogram.uniformLocation(_name));
}

QGLShaderProgram * Crender::getShader()
{
	return &m_shaderprogram;
}

void Crender::initialShader()
{
    //shader程序
    if (!m_shaderprogram.addShaderFromSourceFile(QGLShader::Vertex, "Shaders/basic.vert"))
    {
        QMessageBox::information(NULL, "vert",
            m_shaderprogram.log(),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
    }
    if (!m_shaderprogram.addShaderFromSourceFile(QGLShader::Fragment, "Shaders/basic.Frag"))
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

