#include "GridAndAxis/Anix.h"
//#include <windows.h>
#include <math.h>

const float pi = 3.1415926;
#define  MAX_CHAR 1024
void CAnix::initial(Crender* _render, QOpenGLContext *context, QSurface *surface)
{	
    //glClear(GL_CLEAR_BUFFER);
    initializeOpenGLFunctions();
    m_context = context;
    m_surface = surface;
	m_therender = _render;
    m_context->makeCurrent(m_context->surface());
    //m_vao = _render->allocVao();
    m_vao = m_allocVBOandVAO->allocVAO();
    //创建缓冲区VBO
    m_vbo = m_allocVBOandVAO->allocVBO();
    m_vboIndex = m_allocVBOandVAO->allocVBO();

//    if(m_notCopyFromOtherOpenGLWidget)
//    {
        genAnix();
        genCord();  //ll 20170714
        //qDebug() << "createTheForm";
//    }
//    else if(m_copyCompleted)
//    {
//        copyBufferFromOthers();
//        //qDebug() << "createfromCopy";
//    }
//    else
//    {
//        //qDebug() << "createNothing";
//    }
}

void CAnix::draw()
{
    m_context->makeCurrent(m_surface);

    m_therender->bindShader();
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
		m_vboIndex);
	glDrawElements(GL_TRIANGLES,m_arrowIndex.size()-18, GL_UNSIGNED_INT, 0);
	glLineWidth(1.5);
	glDrawElements(GL_LINES, 18, GL_UNSIGNED_INT, (char*)((m_arrowIndex.size() - 18)*4));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //ll
	glBindVertexArray(0);

    m_therender->releaseShader();
}

void CAnix::clearUp()
{

}

void CAnix::setCordi(vector2f _x, vector2f _y, vector2f _z)
{
    m_x._x = 0;
    m_x._y = _x._y - _x._x;
    m_y._x = 0;
    m_y._y = _y._y - _y._x;
    m_z._x = 0;
    m_z._y = _z._y - _z._x;
	genAnix();							//��������
    updataCord();
}

void CAnix::setCordiAlone()
{
    genAnix();							//��������
    updataCord();
}

void CAnix::setCordiAlone(vector2f _x, vector2f _y, vector2f _z)
{
    m_x._x = 0;
    m_x._y = _x._y - _x._x;
    m_y._x = 0;
    m_y._y = _y._y - _y._x;
    m_z._x = 0;
    m_z._y = _z._y - _z._x;
}

CAnix::CAnix()
{
}


CAnix::~CAnix()
{
}

void CAnix::setAlloc(CAllocVBOandVAO *allocVBOandVAO)
{
    m_allocVBOandVAO = allocVBOandVAO;
}

void CAnix::setCopyFromOtherOpenWidgetMark(GLboolean _b)
{
    m_notCopyFromOtherOpenGLWidget = _b;
}

GLuint CAnix::getAnixVao()
{
    return m_vao;
}

GLuint CAnix::getAnixVbo()
{
    return m_vbo;
}

GLuint CAnix::getAnixVboIndex()
{
    return m_vboIndex;
}


void CAnix::setAnixVboBuffer(vector<GLfloat>& _anixVboBuffer)
{
    m_vboBuffer = _anixVboBuffer;
}

void CAnix::setAnixVboIndexBuffer(vector<GLuint>& _anixVboIndexBuffer)
{
    //该函数在调用完成后将会置m_copyCompleted为true，在初始化anix过程中，会将拷贝来的数据作为缓冲区的数据
    m_vboIndexBuffer = _anixVboIndexBuffer;
    m_copyCompleted = true;
}

void CAnix::copyBufferFromOthers()
{
    m_context->makeCurrent(m_surface);
    glBindVertexArray(m_vao);
    GLuint  _persize = sizeof(vector3f) + sizeof(vector4f);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
        m_vboBuffer.size() * sizeof(GLfloat),
        m_vboBuffer.data(),
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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
        m_vboIndex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        m_vboIndexBuffer.size() * sizeof(GLuint),
        m_vboIndexBuffer.data(),
        GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void CAnix::genCord()
{
    m_context->makeCurrent(m_surface);
	glBindVertexArray(m_vao);
	GLuint  _persize = sizeof(vector3f) + sizeof(vector4f);//λ��3+��ɫ3
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER,
		m_arrowpoint.size() * sizeof(GLfloat),
		m_arrowpoint.data(),
		GL_DYNAMIC_DRAW);
//    GLint size = 0;
//    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
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


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
		m_vboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		m_arrowIndex.size() * sizeof(GLuint),
		m_arrowIndex.data(),
		GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
    //ll 2017714
//    glBindVertexArray(m_vao);
//    GLint size = 0;
//    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
////    glBindBuffer(GL_ARRAY_BUFFER, m_vboIndex);
////    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
//    GLfloat tmpData[size] = {0};
//    glGetBufferSubData(GL_ARRAY_BUFFER, 0, size, tmpData);
//    size = 793;
//    glCopyBufferSubData(m_vbo, m_vboIndex, 0, 0, size);
//    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

}

void CAnix::updataCord()
{
    m_context->makeCurrent(m_surface);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    //GLboolean b = glIsVertexArray(m_vao);
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		m_arrowpoint.size() * sizeof(GLfloat),
		m_arrowpoint.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
		m_vboIndex);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
		0,
		m_arrowIndex.size() * sizeof(GLuint),
		m_arrowIndex.data());
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void CAnix::genAnix()
{
    m_context->makeCurrent(m_surface);
	//vector<GLuint> _indexarrow;
	//vector<float>  _arrordata;
	m_arrowIndex.clear();
	m_arrowpoint.clear();
	size_t anglenum = 20;			//��Ⱦ������Ҫ��������20��

	int offsite = 0;
	for (size_t index = 0; index < anglenum; ++index)		//��������
	{
		if (index < (anglenum - 1))
		{
			m_arrowIndex.push_back(0);
			m_arrowIndex.push_back(index + 1);
			m_arrowIndex.push_back(index + 2);
		}
		else {
			m_arrowIndex.push_back(0);
			m_arrowIndex.push_back(index + 1);
			m_arrowIndex.push_back(1);
		}
	}
	offsite = anglenum + 1;
	for (size_t index = 0; index < anglenum; ++index)		//��������
	{
		if (index < (anglenum - 1))
		{
			m_arrowIndex.push_back(0 + offsite);
			m_arrowIndex.push_back(index + 1 + offsite);
			m_arrowIndex.push_back(index + 2 + offsite);
		}
		else {
			m_arrowIndex.push_back(0 + offsite);
			m_arrowIndex.push_back(index + 1 + offsite);
			m_arrowIndex.push_back(1 + offsite);
		}
	}
	offsite = 2 * (anglenum + 1);
	for (size_t index = 0; index < anglenum; ++index)		//��������
	{
		if (index < (anglenum - 1))
		{
			m_arrowIndex.push_back(0 + offsite);
			m_arrowIndex.push_back(index + 1 + offsite);
			m_arrowIndex.push_back(index + 2 + offsite);
		}
		else {
			m_arrowIndex.push_back(0 + offsite);
			m_arrowIndex.push_back(index + 1 + offsite);
			m_arrowIndex.push_back(1 + offsite);
		}
	}
	offsite = 3 * (anglenum + 1);
	for (size_t index = 0; index < 6 * 3; ++index)					//����������
	{
		m_arrowIndex.push_back(offsite + index);
	}


	//X�����ͷ
	m_arrowpoint.push_back(m_x._y);			//x���
	m_arrowpoint.push_back(m_y._x);			//y��С
	m_arrowpoint.push_back(m_z._x);			//z��С
	m_arrowpoint.push_back(m_xcolor._r);//��ɫ
	m_arrowpoint.push_back(m_xcolor._g);//��ɫ
	m_arrowpoint.push_back(m_xcolor._b);//��ɫ
	m_arrowpoint.push_back(m_xcolor._a);//��ɫ
	for (size_t index = 0; index < anglenum; ++index)
	{
		m_arrowpoint.push_back(m_x._y - m_arrowbase._y);
		m_arrowpoint.push_back(m_y._x+m_arrowbase._x*cos(index * 2 * pi / anglenum));
		m_arrowpoint.push_back(m_z._x+m_arrowbase._x*sin(index * 2 * pi / anglenum));

		m_arrowpoint.push_back(m_xcolor._r);//��ɫ
		m_arrowpoint.push_back(m_xcolor._g);//��ɫ
		m_arrowpoint.push_back(m_xcolor._b);//��ɫ
		m_arrowpoint.push_back(m_xcolor._a);//��ɫ
	}

	//Y���� ��ͷ
	m_arrowpoint.push_back(m_x._x);			//x���
	m_arrowpoint.push_back(m_y._y);			//y��С
	m_arrowpoint.push_back(m_z._x);			//z��С
	m_arrowpoint.push_back(m_ycolor._r);//��ɫ
	m_arrowpoint.push_back(m_ycolor._g);//��ɫ
	m_arrowpoint.push_back(m_ycolor._b);//��ɫ
	m_arrowpoint.push_back(m_ycolor._a);//��ɫ
	for (size_t index = 0; index < anglenum; ++index)
	{
		m_arrowpoint.push_back(m_x._x + m_arrowbase._x*cos(index * 2 * pi / anglenum));
		m_arrowpoint.push_back(m_y._y - m_arrowbase._y);
		m_arrowpoint.push_back(m_z._x + m_arrowbase._x*sin(index * 2 * pi / anglenum));

		m_arrowpoint.push_back(m_ycolor._r);//��ɫ
		m_arrowpoint.push_back(m_ycolor._g);//��ɫ
		m_arrowpoint.push_back(m_ycolor._b);//��ɫ
		m_arrowpoint.push_back(m_ycolor._a);//��ɫ
	}

	//Z���� ��ͷ
	m_arrowpoint.push_back(m_x._x);
	m_arrowpoint.push_back(m_y._x);
	m_arrowpoint.push_back(m_z._y);
	m_arrowpoint.push_back(m_zcolor._r);//��ɫ
	m_arrowpoint.push_back(m_zcolor._g);//��ɫ
	m_arrowpoint.push_back(m_zcolor._b);//��ɫ
	m_arrowpoint.push_back(m_zcolor._a);//��ɫ
	for (size_t index = 0; index < anglenum; ++index)
	{
		m_arrowpoint.push_back(m_x._x + m_arrowbase._x*cos(index * 2 * pi / anglenum));
		m_arrowpoint.push_back(m_y._x - m_arrowbase._x*sin(index * 2 * pi / anglenum));
		m_arrowpoint.push_back(m_z._y - m_arrowbase._y);
		m_arrowpoint.push_back(m_zcolor._r);//��ɫ
		m_arrowpoint.push_back(m_zcolor._g);//��ɫ
		m_arrowpoint.push_back(m_zcolor._b);//��ɫ
		m_arrowpoint.push_back(m_zcolor._a);//��ɫ
	}

	//X��
	m_arrowpoint.push_back(m_x._x); m_arrowpoint.push_back(m_y._x); m_arrowpoint.push_back(m_z._x);
	m_arrowpoint.push_back(m_xcolor._r); m_arrowpoint.push_back(m_xcolor._g); m_arrowpoint.push_back(m_xcolor._b); m_arrowpoint.push_back(m_xcolor._a);

	m_arrowpoint.push_back(m_x._y); m_arrowpoint.push_back(m_y._x); m_arrowpoint.push_back(m_z._x);
	m_arrowpoint.push_back(m_xcolor._r); m_arrowpoint.push_back(m_xcolor._g); m_arrowpoint.push_back(m_xcolor._b); m_arrowpoint.push_back(m_xcolor._a);
	//Y��
	m_arrowpoint.push_back(m_x._x); m_arrowpoint.push_back(m_y._x); m_arrowpoint.push_back(m_z._x);
	m_arrowpoint.push_back(m_ycolor._r); m_arrowpoint.push_back(m_ycolor._g); m_arrowpoint.push_back(m_ycolor._b); m_arrowpoint.push_back(m_ycolor._a);

	m_arrowpoint.push_back(m_x._x); m_arrowpoint.push_back(m_y._y); m_arrowpoint.push_back(m_z._x);
	m_arrowpoint.push_back(m_ycolor._r); m_arrowpoint.push_back(m_ycolor._g); m_arrowpoint.push_back(m_ycolor._b); m_arrowpoint.push_back(m_ycolor._a);
	//Z��
	m_arrowpoint.push_back(m_x._x); m_arrowpoint.push_back(m_y._x); m_arrowpoint.push_back(m_z._x);
	m_arrowpoint.push_back(m_zcolor._r); m_arrowpoint.push_back(m_zcolor._g); m_arrowpoint.push_back(m_zcolor._b); m_arrowpoint.push_back(m_zcolor._a);

	m_arrowpoint.push_back(m_x._x); m_arrowpoint.push_back(m_y._x); m_arrowpoint.push_back(m_z._y);
	m_arrowpoint.push_back(m_zcolor._r); m_arrowpoint.push_back(m_zcolor._g); m_arrowpoint.push_back(m_zcolor._b); m_arrowpoint.push_back(m_zcolor._a);

}
