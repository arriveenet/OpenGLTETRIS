#include "MainScreen.h"

#include "field.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void MainScreen::init()
{
	fieldInit();

	for (int j = 0; j < TETRIMINO_SHAPE_MAX; j++)
		tetriminos[j] = j;
	shuffleTetriminos();

	initTetrimino();

	memcpy(field, defaultField, sizeof field);
}

void MainScreen::reset()
{
	memcpy(field, defaultField, sizeof field);
}

void MainScreen::keyboard(unsigned char _key)
{
	TETRIMINO lastTetrimino = m_tetrimino;

	switch (_key) {
	case 'w':
		break;
	case 's':
		m_tetrimino.y++;
		break;
	case 'a':
		m_tetrimino.x--;
		break;
	case 'd':
		m_tetrimino.x++;
		break;
	case ' ':
		tetriminoRotate(&m_tetrimino);
		break;
	}

	if (intersectField()) {
		m_tetrimino = lastTetrimino;
	}
}

void MainScreen::tick()
{
	TETRIMINO lastTetrimino = m_tetrimino;

	m_tetrimino.y++;

	if (intersectField()) {
		m_tetrimino = lastTetrimino;

		for (int y = 0; y < TETRIMINO_HEIGHT_MAX; y++) {
			for (int x = 0; x < TETRIMINO_WIDTH_MAX; x++) {
				if (m_tetrimino.shape.pattern[y][x]) {
					field[m_tetrimino.y + y][m_tetrimino.x + x] = m_tetrimino.type + 2;
				}
			}
		}

		if (intersectField()) {
			printf("GAME OVER\n");
		}

		eraseLine();

		initTetrimino();
	}
}

void MainScreen::update()
{

}

void MainScreen::draw()
{
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			//printf("%d", field[y][x]);
		}
		//printf("\n");
	}
	//printf("field=%p\n", field);
	fieldDraw(field, sizeof field, &m_tetrimino);
}

void MainScreen::initTetrimino()
{
	if (turn > TETRIMINO_SHAPE_MAX) {
		turn = 0;
		shuffleTetriminos();
	}

	if (turn < TETRIMINO_SHAPE_MAX) {
		//printf("turn=%d\n", turn);
		int type = tetriminos[turn];
		m_tetrimino.type = type;
		m_tetrimino.shape = tetriminoShapes[type];
		m_tetrimino.x = FIELD_WIDTH / 2 - m_tetrimino.shape.size / 2;
		m_tetrimino.y = 0;
	}
	turn++;
}

void MainScreen::shuffleTetriminos()
{
	for (int i = 0; i < TETRIMINO_SHAPE_MAX; i++) {
		int j = rand() % TETRIMINO_SHAPE_MAX;
		int t = tetriminos[i];
		tetriminos[i] = tetriminos[j];
		tetriminos[j] = t;
	}
}

bool MainScreen::intersectField()
{
	for (int y = 0; y < m_tetrimino.shape.size; y++) {
		for (int x = 0; x < m_tetrimino.shape.size; x++) {
			if (m_tetrimino.shape.pattern[y][x]) {
				int globalX = m_tetrimino.x + x;
				int globalY = m_tetrimino.y + y;
				if ((globalX < 0)
					|| (globalX >= FIELD_WIDTH)
					|| (globalY < 0)
					|| (globalY >= FIELD_HEIGHT)
					|| (field[globalY][globalX] != BLOCK_NONE)
					) {
					return true;
				}
			}
		}
	}

	return false;
}

void MainScreen::eraseLine()
{
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		bool completed = true;
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (field[y][x] == BLOCK_NONE) {
				completed = false;
				break;
			}
		}
		if (completed) {
			for (int x = 0; x < FIELD_WIDTH; x++) {
				if ((field[y][x] >= BLOCK_SOFT_I)
					&& (field[y][x] <= BLOCK_SOFT_T)
					) {
					field[y][x] = BLOCK_NONE;
				}
			}

			for (int x = 0; x < FIELD_WIDTH; x++) {
				for (int y2 = y; y2 >= 0; y2--) {
					if (field[y2][x] == BLOCK_HARD)
						break;

					if (y2 == 0) {
						field[y2][x] = BLOCK_NONE;
					} else {
						if (field[y2 - 1][x] != BLOCK_HARD) {
							field[y2][x] = field[y2 - 1][x];
						}
					}
				}
			}

		}
	}
}
