#pragma once

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

const TETRIMINOSHAPE tetriminoShapes[TETRIMINO_SHAPE_MAX] =
{
	// TETRIMINO_SHAPE_I
	{
		4,
		{
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		},
		{0x00, 0xff, 0xff}
	},

	// TETRIMINO_SHAPE_O
	{
		3,
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

void tetriminoRotate(TETRIMINO* _tetrimino);