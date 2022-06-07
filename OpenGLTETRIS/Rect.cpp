#include "Rect.h"

#include <string.h>
#include <gl/glut.h>

using glm::vec2;

static constexpr glm::vec2 defalutTexCoord[4] =
{
	{0, 0},
	{0, 1},
	{1, 1},
	{1, 0}
};

Rect::Rect()
	:m_position(0,0)
	,m_size(0,0)
{
	memset(m_vertex, 0, sizeof m_vertex);
	memcpy(m_texCoord, defalutTexCoord, sizeof defalutTexCoord);
}

Rect::Rect(vec2 _size)
	:m_position(0,0)
	,m_size(_size)
{
	memset(m_vertex, 0, sizeof m_vertex);
	memcpy(m_texCoord, defalutTexCoord, sizeof defalutTexCoord);
}

Rect::Rect(vec2 _size, vec2 _position)
	:m_position(_position)
	,m_size(_size)
{
	m_vertex[0] = _position;
	m_vertex[1] = _position + vec2(0, _size.y);
	m_vertex[2] = _position + _size;
	m_vertex[3] = _position + vec2(_size.x, 0);

	memcpy(m_texCoord, defalutTexCoord, sizeof defalutTexCoord);
}

void Rect::setSize(const vec2& _size)
{
	m_size = _size;
}

void Rect::setPosition(const vec2& _position)
{
	m_vertex[0] = _position;
	m_vertex[1] = _position + vec2(0, m_size.y);
	m_vertex[2] = _position + m_size;
	m_vertex[3] = _position + vec2(m_size.x, 0);
}

void Rect::setTexCoord(const glm::vec2* _texCoord[4])
{
	memcpy(m_texCoord, _texCoord, sizeof _texCoord);
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

	glTexCoordPointer(
		2,			// GLint size
		GL_FLOAT,	// GLenum type
		0,			// GLsizei stride
		m_texCoord);	// const GLvoid * pointer

	glDrawElements(
		GL_TRIANGLES,		// GLenum mode
		6,					// GLsizei count
		GL_UNSIGNED_BYTE,	// GLenum type
		indices);			// const GLvoid * indices
}