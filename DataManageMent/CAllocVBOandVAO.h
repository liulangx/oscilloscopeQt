#ifndef CALLOCVBOANDVAO_H
#define CALLOCVBOANDVAO_H
/* Class Name: CAllocVBOandVAO;
 * Description:
 * This class is for creating the VAO and VBO, so the generated buffer object can be organized and deleted in the same time;
 * Function:
 * 1. create VAO by allocVAO() which returns the unused name(GLuint)
 * 2. create VBO by allocVBO() which returns the unused name(GLuint)
 * Tips: this class must be prepared by using prepare();
*/
#include <vector>
#include <QOpenGLFunctions_4_3_Core>

using namespace std;
class CAllocVBOandVAO : protected QOpenGLFunctions_4_3_Core
{
public:
    CAllocVBOandVAO();
    ~CAllocVBOandVAO();
    void prepare(/*QOpenGLContext *_tmpContext*/);
    GLuint allocVAO();
    GLuint allocVBO();
private:
    vector<GLuint> m_vaos;				//vaos
    vector<GLuint> m_vbos;              //vbos
};

#endif // CALLOCVBOANDVAO_H
