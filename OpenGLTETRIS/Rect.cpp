#include "Rect.h"

#include <gl/glut.h>

using namespace glm;

Rect::Rect()
	:m_position(0,0)
	,m_size(0,0)
{
	for (int i = 0; i < 4; i++)
		m_vertex[i] = vec2(0, 0);
}

Rect::Rect(vec2 _size, vec2 _position)
	:m_position(_position)
	,m_size(_size)
{
	m_vertex[0] = _position;
	m_vertex[1] = _position + vec2(0, _size.y);
	m_vertex[2] = _position + _size;
	m_vertex[3] = _position + vec2(_size.x, 0);
}

void Rect::draw()
{
	static GLubyte indices[] = { 0 , 1 , 2, 2, 3, 0};
	//static GLubyte lineIndices[] = { 0 , 1, 1, 2, 2, 0, 0, 3, 3, 2};
	glVertexPointer(
		2,			// GLint size
		GL_FLOAT,	// GLenum type
		0,			// GLsizei stride
		m_vertex);	// const GLvoid * pointer
	glDrawElements(
		GL_TRIANGLES,		// GLenum mode
		10,					// GLsizei count
		GL_UNSIGNED_BYTE,	// GLenum type
		indices);			// const GLvoid * indices
}