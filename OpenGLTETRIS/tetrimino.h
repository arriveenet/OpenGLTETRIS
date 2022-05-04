#pragma once

#include <gl/glut.h>
#include <glm/glm.hpp>

#define TETRIMINO_WIDTH_MAX		4
#define TETRIMINO_HEIGHT_MAX	4

enum {
	TETRIMINO_SHAPE_I,
	TETRIMINO_SHAPE_O,
	TETRIMINO_SHAPE_S,
	TETRIMINO_SHAPE_Z,
	TETRIMINO_SHAPE_J,
	TETRIMINO_SHAPE_L,
	TETRIMINO_SHAPE_T,
	TETRIMINO_SHAPE_MAX
};

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
}TETRIMINOCOLOR;

typedef struct {
	int size;
	bool pattern[TETRIMINO_WIDTH_MAX][TETRIMINO_HEIGHT_MAX];
	TETRIMINOCOLOR color;
}TETRIMINOSHAPE;

typedef struct {
	int type;
	int x, y;
	TETRIMINOSHAPE shape;
}TETRIMINO;

extern const TETRIMINOSHAPE tetriminoShapes[TETRIMINO_SHAPE_MAX];
extern GLuint g_tetriminoTexture;

void tetriminoInit();
void tetriminoRotate(TETRIMINO* _tetrimino);
void tetriminoDraw(int _shape,
	const glm::vec2 &_position,
	const glm::vec2 &_size = glm::vec2(8, 8));
