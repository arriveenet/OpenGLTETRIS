#include "tetrimino.h"

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