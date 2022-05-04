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

static const int lineVertexCount = FIELD_HEIGHT * FIELD_WIDTH * 2;
static vec2 lineVertex[lineVertexCount];
static vec2 position = { 80,8*5 };
static Texture texture;
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
		lineVertex[i] = vec2(position.x + x * BLOCK_SIZE, position.y);
		lineVertex[i+1] = vec2(position.x + x * BLOCK_SIZE, position.y + FIELD_HEIGHT * BLOCK_SIZE);
		i += 2;
	}
	for (int y = 0; y <= FIELD_HEIGHT; y++) {
		lineVertex[i] = vec2(position.x + 0, position.y + y * BLOCK_SIZE);
		lineVertex[i + 1] = vec2(position.x + FIELD_WIDTH * BLOCK_SIZE, position.y + y * BLOCK_SIZE);
		i += 2;
	}

	return 0;
}


void fieldUpdate()
{
}

void fieldDraw(
	const int field[FIELD_HEIGHT][FIELD_WIDTH],
	const TETRIMINO* tetrimino)
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
			if (tetrimino->shape.pattern[y][x]) {
				screen[tetrimino->y + y][tetrimino->x + x] = BLOCK_FALL;
			}
		}

	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			//printf("%d", m_field[y][x]);
			switch (screen[y][x]) {
			case BLOCK_NONE:
				break;
			case BLOCK_HARD:
				glColor3ub(0x7c, 0x7c, 0x7c);
				rect.setPosition(vec2(x * BLOCK_SIZE + position.x, y * BLOCK_SIZE + position.y));
				break;
			case BLOCK_SOFT_I:
				glColor3ub(0x00, 0xff, 0xff);
				rect.setPosition(vec2(x * BLOCK_SIZE + position.x, y * BLOCK_SIZE + position.y));
				break;
			case BLOCK_SOFT_O:
				glColor3ub(0xff, 0xff, 0x00);
				rect.setPosition(vec2(x * BLOCK_SIZE + position.x, y * BLOCK_SIZE + position.y));
				break;
			case BLOCK_SOFT_S:
				glColor3ub(0x00, 0xff, 0x00);
				rect.setPosition(vec2(x * BLOCK_SIZE + position.x, y * BLOCK_SIZE + position.y));
				break;
			case BLOCK_SOFT_Z:
				glColor3ub(0xff, 0x00, 0x00);
				rect.setPosition(vec2(x * BLOCK_SIZE + position.x, y * BLOCK_SIZE + position.y));
				break;
			case BLOCK_SOFT_J:
				glColor3ub(0x00, 0x00, 0xff);
				rect.setPosition(vec2(x * BLOCK_SIZE + position.x, y * BLOCK_SIZE + position.y));
				break;
			case BLOCK_SOFT_L:
				glColor3ub(0xff, 0x7c, 0x00);
				rect.setPosition(vec2(x * BLOCK_SIZE + position.x, y * BLOCK_SIZE + position.y));
				break;
			case BLOCK_SOFT_T:
				glColor3ub(0x7c, 0x00, 0x7c);
				rect.setPosition(vec2(x * BLOCK_SIZE + position.x, y * BLOCK_SIZE + position.y));
				break;
			case BLOCK_FALL:
				glColor3ub(
					tetrimino->shape.color.r,
					tetrimino->shape.color.g,
					tetrimino->shape.color.b);
				rect.setPosition(vec2(x * BLOCK_SIZE + position.x, y * BLOCK_SIZE + position.y));
				break;
			}
			rect.draw();
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