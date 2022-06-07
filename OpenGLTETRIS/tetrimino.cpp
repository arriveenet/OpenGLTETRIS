#include "tetrimino.h"
#include "Rect.h"
#include "Texture.h"

using namespace glm;

const TETRIMINOSHAPE tetriminoShapes[TETRIMINO_SHAPE_MAX] =
{
	// TETRIMINO_SHAPE_I
	{
		4,
		{
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0}
		},
		{0x00, 0xff, 0xff}
	},

	// TETRIMINO_SHAPE_O
	{
		2,
		{
			{1,1,0,0},
			{1,1,0,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{0xff, 0xff, 0x00}
	},

	// TETRIMINO_SHAPE_S
	{
		3,
		{
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{0x00, 0xff, 0x00}
	},

	// TETRIMINO_SHAPE_Z
	{
		3,
		{
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{0xff, 0x00, 0x00}
	},

	// TETRIMINO_SHAPE_J
	{
		3,
		{
			{1,0,0,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{0x00, 0x00, 0xff}
	},

	// TETRIMINO_SHAPE_L
	{
		3,
		{
			{0,0,1,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{0xff, 0x7c, 0x00}
	},

	// TETRIMINO_SHAPE_T
	{
		3,
		{
			{0,1,0,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{0x7c, 0x00, 0x7c}
	},
};

GLuint g_tetriminoTexture;

void tetriminoInit()
{
	Texture texture;
	texture.loadBitmapFile("font/tetrimino.bmp");
	glGenTextures(1, &g_tetriminoTexture);
	glBindTexture(GL_TEXTURE_2D, g_tetriminoTexture);
	glTexImage2D(
		GL_TEXTURE_2D,		// GLenum target
		0,					// GLint level
		GL_RGBA,			// GLint internalformat
		texture.getWidth(),	// GLsizei width
		texture.getHeight(),// GLsizei height
		0,					// GLint border
		GL_RGBA,			// GLenum format
		GL_UNSIGNED_BYTE,	// GLenum type
		texture.getTexImage());	 // const GLvoid * pixels
	glTexParameteri(
		GL_TEXTURE_2D,			// GLenum target
		GL_TEXTURE_MAG_FILTER,	// GLenum pname
		GL_NEAREST);			// GLint param
	glTexParameteri(
		GL_TEXTURE_2D,			// GLenum target
		GL_TEXTURE_MIN_FILTER,	// GLenum pname
		GL_NEAREST);			// GLint param
	glBindTexture(GL_TEXTURE_2D, 0);
}

void tetriminoRotate(TETRIMINO* _tetrimino)
{
	TETRIMINO rotatedTetrimino = *_tetrimino;
	for (int y = 0; y < _tetrimino->shape.size; y++)
		for (int x = 0; x < _tetrimino->shape.size; x++) {
			rotatedTetrimino.shape.pattern[_tetrimino->shape.size - 1 - x][y] =
				_tetrimino->shape.pattern[y][x];
		}
	*_tetrimino = rotatedTetrimino;
}

void tetriminoDraw(int _shape, const glm::vec2& _position, const glm::vec2& _size)
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
		GL_TEXTURE_2D,	// GLenum target
		g_tetriminoTexture);			// GLuint texture

	glColor3ub(
		tetriminoShapes[_shape].color.r,	// GLbyte red
		tetriminoShapes[_shape].color.g,	// GLbyte green
		tetriminoShapes[_shape].color.b);	// GLbyte blue

	for (int y = 0; y < TETRIMINO_HEIGHT_MAX; y++)
		for (int x = 0; x < TETRIMINO_WIDTH_MAX; x++) {
			if (tetriminoShapes[_shape].pattern[y][x]) {
				Rect(
					vec2(_size.x, _size.y),		// size
					vec2(_position.x + _size.x * x,// positionX
						_position.y + _size.y * y) // positionY
				).draw();
			}
		}

	glPopAttrib();
	glPopClientAttrib();
}

TETRIMINOCOLOR tetriminoGetColor(int _shape)
{
	return tetriminoShapes->color;
}