#include  "DataManageMent/CAllocVBOandVAO.h"

CAllocVBOandVAO::CAllocVBOandVAO()
{
    //initializeOpenGLFunctions();
}

CAllocVBOandVAO::~CAllocVBOandVAO()
{
    for(vector<GLuint>::iterator it = m_vaos.begin(); it != m_vaos.end(); ++it )
    {
        glDeleteVertexArrays(1, &(*it));
    }
    for(vector<GLuint>::iterator it = m_vbos.begin(); it != m_vbos.end(); ++it)
    {
        glDeleteBuffers(1, &(*it));
    }
}

// create Vertex Array Buffer Object;
GLuint CAllocVBOandVAO::allocVAO()
{
    GLuint vao;
    glGenVertexArrays(1, &vao);

    if(vao <= 0)
        return -1;
    m_vaos.push_back(vao);
    return (GLuint)vao;
}
// create Vertex Buffer Object;
GLuint CAllocVBOandVAO::allocVBO()
{
    GLuint vbo;
    glGenBuffers(1, &vbo);

    if(vbo <= 0)
        return -1;
    m_vbos.push_back(vbo);
    return (GLuint)vbo;
}

// prepare for the initial environment;
void CAllocVBOandVAO::prepare()
{
    //_tmpContext->makeCurrent(_tmpContext->surface());
//    QOpenGLFunctions_4_3_Core* m_funcs = _tmpContext->versionFunctions<QOpenGLFunctions_4_3_Core>();
//    m_funcs->initializeOpenGLFunctions();
    initializeOpenGLFunctions();
}
