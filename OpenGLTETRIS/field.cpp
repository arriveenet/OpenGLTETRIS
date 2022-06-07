#include "field.h"
#include "Rect.h"
#include "tetris.h"
#include "Game.h"
#include "Texture.h"

#include <stdio.h>
#include <string.h>

#include <gl/glut.h>
#include <glm/glm.hpp>

using namespace glm;

static const int m_lineVertexCount = FIELD_HEIGHT * FIELD_WIDTH * 2;
static vec2 m_lineVertex[m_lineVertexCount];
static vec2 m_position = { 80,8*5 };
static Texture m_texture;
GLuint g_texId;

int defaultField[FIELD_HEIGHT][FIELD_WIDTH] =
{
	{1,1,1,0,0,0,0,0,0,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1},
};

int fieldInit()
{
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

void fieldDraw(
	const int field[FIELD_HEIGHT][FIELD_WIDTH],
	const TETRIMINO* tetrimino,
	const TETRIMINO* shadowMino)
{
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);// GLbitfield mask
	glPushAttrib(GL_ALL_ATTRIB_BITS);// GLbitfield mask

	glEnableClientState(GL_VERTEX_ARRAY);// GLenum array
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);// GLenum array

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);			// GLenum cap
	glBlendFunc(
		GL_SRC_ALPHA,			//GLenum sfactor
		GL_ONE_MINUS_SRC_ALPHA);//GLenum dfactor

	glBindTexture(
		GL_TEXTURE_2D,		// GLenum target
		g_tetriminoTexture);// GLuint texture

	int screen[FIELD_HEIGHT][FIELD_WIDTH];
	memcpy(screen, field, sizeof field);
	Rect rect(vec2(BLOCK_SIZE, BLOCK_SIZE));

	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			screen[y][x] = field[y][x];
		}
	}

	for (int y = 0; y < TETRIMINO_HEIGHT_MAX; y++)
		for (int x = 0; x < TETRIMINO_WIDTH_MAX; x++) {
			if (tetrimino->shape.pattern[y][x])
				screen[tetrimino->y + y][tetrimino->x + x] = BLOCK_FALL;

			if (shadowMino->shape.pattern[y][x])
				screen[shadowMino->y + y][shadowMino->x + x] = BLOCK_SHADOW;
		}

	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			//printf("%d", field[y][x]);
			switch (screen[y][x]) {
			case BLOCK_NONE:
				break;
			case BLOCK_HARD:
				glColor3ub(0x80, 0x80, 0x80);
				break;
			case BLOCK_SOFT_I:
				glColor3ub(0x00, 0xff, 0xff);
				break;
			case BLOCK_SOFT_O:
				glColor3ub(0xff, 0xff, 0x00);
				break;
			case BLOCK_SOFT_S:
				glColor3ub(0x00, 0xff, 0x00);
				break;
			case BLOCK_SOFT_Z:
				glColor3ub(0xff, 0x00, 0x00);
				break;
			case BLOCK_SOFT_J:
				glColor3ub(0x00, 0x00, 0xff);
				break;
			case BLOCK_SOFT_L:
				glColor3ub(0xff, 0x80, 0x00);
				break;
			case BLOCK_SOFT_T:
				glColor3ub(0x7c, 0x00, 0x80);
				break;
			case BLOCK_FALL:
				glColor3ub(
					tetrimino->shape.color.r,
					tetrimino->shape.color.g,
					tetrimino->shape.color.b);
				break;
			case BLOCK_SHADOW:
				glColor4ub(
					tetrimino->shape.color.r,
					tetrimino->shape.color.g,
					tetrimino->shape.color.b,
					0x40);
				break;
			}
			if (screen[y][x] != BLOCK_NONE) {
				rect.setPosition(
					vec2(x * BLOCK_SIZE + m_position.x,
						y * BLOCK_SIZE + m_position.y));
				rect.draw();
			}
		}
		//putchar('\n');
	}

	/*
	glColor3ub(0xff, 0xff, 0xff);
	glVertexPointer(
		2,				// GLint size
		GL_FLOAT,		// GLenum type
		0,				// GLsizei stride
		lineVertex);	// const GLvoid * pointer
	glDrawArrays(
		GL_LINES,// GLenum mode
		0,// GLint first
		lineVertexCount);// GLsizei count
	*/

	glPopAttrib();
	glPopClientAttrib();
}