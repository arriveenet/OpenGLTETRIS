#pragma once
#include "tetrimino.h"

#define FIELD_WIDTH		(12)
#define FIELD_HEIGHT	(22)
#define BLOCK_SIZE		(8)

enum {
	BLOCK_NONE,
	BLOCK_HARD,
	BLOCK_SOFT_I,
	BLOCK_SOFT_O,
	BLOCK_SOFT_S,
	BLOCK_SOFT_Z,
	BLOCK_SOFT_J,
	BLOCK_SOFT_L,
	BLOCK_SOFT_T,
	BLOCK_FALL,
	BLOCK_MAX,
};

int fieldInit();
void fieldUpdate();
void fieldDraw(
	const int field[FIELD_HEIGHT][FIELD_WIDTH],
	const TETRIMINO* tetrimino);

extern int defaultField[FIELD_HEIGHT][FIELD_WIDTH];
