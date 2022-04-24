#pragma once

#define FIELD_WIDTH		(10)
#define FIELD_HEIGHT	(20)
#define BLOCK_SIZE		(8)

enum {
	BLOCK_NONE,
	BLOCK_HARD,
	BLOCK_FALL,
	BLOCK_MAX,
};

int fieldInit();
void fieldUpdate();
void fieldDraw();

