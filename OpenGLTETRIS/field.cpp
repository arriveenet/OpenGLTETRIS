#include "field.h"
#include "Rect.h"

#include <stdio.h>
#include <string.h>

#include <gl/glut.h>
#include <glm/glm.hpp>

using namespace glm;

static int m_field[FIELD_HEIGHT][FIELD_WIDTH];
const int lineVertexCount = FIELD_HEIGHT * FIELD_WIDTH * 2;
vec2 m_lineVertex[lineVertexCount];
vec2 m_position = { 8,8 };

int fieldInit()
{
	m_field[9][5] = BLOCK_FALL;

	int i = 0;
	for (int x = 0; x <= FIELD_WIDTH; x++) {
		m_lineVertex[i] = vec2(m_position.x + x * BLOCK_SIZE, m_position.y);
		m_lineVertex[i+1] = vec2(m_position.x + x * BLOCK_SIZE, m_position.y + FIELD_HEIGHT * BLOCK_SIZE);
		i += 2;
	}
	for (int y = 0; y <= FIELD_HEIGHT; y++) {
		m_lineVertex[i] = vec2(m_position.x + 0, m_position.y + y * BLOCK_SIZE);
		m_lineVertex[i + 1] = vec2(m_position.x + FIELD_WIDTH * BLOCK_SIZE, m_position.y + y * BLOCK_SIZE);
		i += 2;
	}

	return 0;
}

void fieldUpdate()
{

}

void fieldDraw()
{
	glVertexPointer(
		2,				// GLint size
		GL_FLOAT,		// GLenum type
		0,				// GLsizei stride
		m_lineVertex);	// const GLvoid * pointer
	glDrawArrays(
		GL_LINES,// GLenum mode
		0,// GLint first
		lineVertexCount);// GLsizei count

	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			//printf("%d", m_field[y][x]);
			switch (m_field[y][x]) {
			case BLOCK_NONE:
				break;
			case BLOCK_FALL:
				Rect(vec2(BLOCK_SIZE, BLOCK_SIZE), vec2(x* BLOCK_SIZE + m_position.x, y* BLOCK_SIZE + m_position.y)).draw();
				break;
			}
		}
		//putchar('\n');
	}

}