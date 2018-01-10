#ifndef CGRIDRENDER_H
#define CGRIDRENDER_H
#include "MyOpenglWidget/Clines.h"
#include <vector>
#include <QOpenGLFunctions_4_3_Core>
#include <QGLShader>
#include <QGLShaderProgram>
#include <QMessageBox>
using namespace std;
class CGridRender :protected QOpenGLFunctions_4_3_Core
{
public:
    CGridRender();
    ~CGridRender();
    void	prepare();
    void	bindShader();
    virtual void bindAttributes();
    void    setBackGround(vector4f& _color);
    void	releaseShader();
    QGLShaderProgram *getShader();
    void initialShader();
protected:

    QGLShaderProgram			m_shaderprogram;				//shader程序
    vector<GLuint>              m_vaos;				//vba
};

#endif // CGRIDRENDER_H
