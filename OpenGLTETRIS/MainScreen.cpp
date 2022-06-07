#include "MainScreen.h"

#include "field.h"
#include "font.h"
#include "audio.h"
#include "Wave.h"
#include "Sound.h"
#include "Game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gl/glut.h>
#include <glm/glm.hpp>

#include <random>

using namespace glm;

MainScreen::MainScreen()
	: m_holdTetrimino(-1)
	, m_lines(0)
	, m_score(0)
{
	// m_bags init and shuffle
	shuffleBags();

	m_nowTetrimino = m_bags.back();
	//m_nowTetrimino = TETRIMINO_SHAPE_I;
	m_bags.pop_back();
	m_nextTetrimino = m_bags.back();
	m_bags.pop_back();

	fieldInit();

	memcpy(m_field, defaultField, sizeof m_field);

	initTetrimino();
	shadowTetrimino();
}

void MainScreen::init()
{
	tetriminoInit();

	if (Sound::init() == 0) {
		g_pSound->play(SOUND_TYPES_BGM);
		g_pSound->loop(SOUND_TYPES_BGM, true);
	}
}

void MainScreen::reset()
{
	memcpy(m_field, defaultField, sizeof m_field);
}

void MainScreen::keyboard(unsigned char _key)
{
	TETRIMINO lastTetrimino = m_tetrimino;

	switch (_key) {
	case KEY_BIND_SOFT_DROP:
		m_tetrimino.y++;
		g_pSound->play(SOUND_TYPES_SE_SOFTDROP);
		break;
	case KEY_BIND_HARD_DROP:
		hardDrop();
		g_pSound->play(SOUND_TYPES_SE_HARDDROP);
		break;
	case KEY_BIND_MOVE_LEFT:
		m_tetrimino.x--;
		g_pSound->play(SOUND_TYPES_SE_MOVE);
		break;
	case KEY_BIND_MOVE_RIGHT:
		m_tetrimino.x++;
		g_pSound->play(SOUND_TYPES_SE_MOVE);
		break;
	case KEY_BIND_HOLD:
		if (m_holdTetrimino >= 0) {
			int temp = m_holdTetrimino;
			m_holdTetrimino = m_nowTetrimino;
			m_tetrimino.type = m_nowTetrimino = temp;
			m_tetrimino.shape = tetriminoShapes[m_nowTetrimino];
		} else {
			m_holdTetrimino = m_tetrimino.type;
			m_nowTetrimino = m_nextTetrimino;
			m_nextTetrimino = m_bags.back();
			m_bags.pop_back();
			m_tetrimino.type = m_nowTetrimino;
			m_tetrimino.shape = tetriminoShapes[m_nowTetrimino];
		}
		g_pSound->play(SOUND_TYPES_SE_HOLD);
		break;
	case KEY_BIND_ROTATE_RIGHT:
		tetriminoRotate(&m_tetrimino);
		g_pSound->play(SOUND_TYPES_SE_ROTATE);
		break;
	}

	if (intersectField(m_tetrimino)) {
		m_tetrimino = lastTetrimino;
	}

	shadowTetrimino();
}

void MainScreen::tick()
{
	TETRIMINO lastTetrimino = m_tetrimino;

	m_tetrimino.y++;

	if (intersectField(m_tetrimino)) {
		m_tetrimino = lastTetrimino;

		for (int y = 0; y < TETRIMINO_HEIGHT_MAX; y++) {
			for (int x = 0; x < TETRIMINO_WIDTH_MAX; x++) {
				if (m_tetrimino.shape.pattern[y][x]) {
					m_field[m_tetrimino.y + y][m_tetrimino.x + x] = m_tetrimino.type + 2;
				}
			}
		}

		eraseLine();

		if (m_bags.empty())
			shuffleBags();

		m_nowTetrimino = m_nextTetrimino;
		m_nextTetrimino = m_bags.back();
		m_bags.pop_back();

		initTetrimino();
		shadowTetrimino();
	}
}

void MainScreen::update()
{
}

void MainScreen::draw()
{
	fieldDraw(m_field, &m_tetrimino, &m_shadowMino);

	tetriminoDraw(m_nextTetrimino, vec2(8 * 24, 8 * 7));
	if (m_holdTetrimino >= 0)
		tetriminoDraw(m_holdTetrimino, vec2(8 * 4, 8 * 7));

	fontBegin();
	{
		glColor3ub(0xff, 0xff, 0xff);
		fontPosition(8 * 3, 8 * 5);
		fontDraw("aHOLDbc\n"
				 "g     h\n"
				 "g     h\n"
				 "g     h\n"
				 "deeeeef");
		fontPosition(8 * 3, 8 * 11);
		fontDraw("aLINESc\n"
				 "g%5dh\n"
				 "deeeeef", m_lines);
		fontPosition(8 * 3, 8 * 15);
		fontDraw("aSCOREc\n"
				 "g%5dh\n"
				 "deeeeef", m_score);
		fontPosition(8 * 23, 8 * 5);
		fontDraw("aNEXTc\n"
				 "g    h\n"
				 "g    h\n"
				 "g    h\n"
				 "deeeef");
	}
	fontEnd();
}

void MainScreen::initTetrimino()
{
	//printf("now=%d\n", nowTetrimino);
	//printf("next=%d\n", nextTetrimino);

	m_tetrimino.type = m_nowTetrimino;
	m_tetrimino.shape = tetriminoShapes[m_nowTetrimino];
	m_tetrimino.x = FIELD_WIDTH / 2 - m_tetrimino.shape.size / 2;
	m_tetrimino.y = 0;
}

void MainScreen::shuffleBags()
{
	// init m_bugs
	for (int i = 0; i < TETRIMINO_SHAPE_MAX; i++)
		m_bags.push_back(i);
	// shuffle m_bugs
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(m_bags.begin(), m_bags.end(), engine);
}

bool MainScreen::intersectField(const TETRIMINO& _tetrimino)
{
	for (int y = 0; y < _tetrimino.shape.size; y++) {
		for (int x = 0; x < _tetrimino.shape.size; x++) {
			if (_tetrimino.shape.pattern[y][x]) {
				int globalX = _tetrimino.x + x;
				int globalY = _tetrimino.y + y;
				if ((globalX < 0)
					|| (globalX >= FIELD_WIDTH)
					|| (globalY < 0)
					|| (globalY >= FIELD_HEIGHT)
					|| (m_field[globalY][globalX] != BLOCK_NONE)
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
	int lines = 0;
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		bool completed = true;
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (m_field[y][x] == BLOCK_NONE
				|| (y >= FIELD_HEIGHT - 1)) {
				completed = false;
				break;
			}
		}
		if (completed) {
			for (int x = 0; x < FIELD_WIDTH; x++) {
				if ((m_field[y][x] >= BLOCK_SOFT_I)
					&& (m_field[y][x] <= BLOCK_SOFT_T)
					) {
					m_field[y][x] = BLOCK_NONE;
				}
			}

			for (int x = 0; x < FIELD_WIDTH; x++) {
				for (int y2 = y; y2 >= 0; y2--) {
					if (m_field[y2][x] == BLOCK_HARD)
						break;

					if (y2 == 0) {
						m_field[y2][x] = BLOCK_NONE;
					} else {
						if (m_field[y2 - 1][x] != BLOCK_HARD) {
							m_field[y2][x] = m_field[y2 - 1][x];
						}
					}
				}
			}
			m_lines++;
			lines++;
		}
	}

	if (lines == 1) {
		g_pSound->play(SOUND_TYPES_SE_SINGLE);
		m_score += 100;
		printf("single\n");
	} else if (lines == 2) {
		g_pSound->play(SOUND_TYPES_SE_DOUBLE);
		printf("double\n");
		m_score += 300;
	} else if (lines == 3) {
		g_pSound->play(SOUND_TYPES_SE_TRIPLE);
		printf("triple\n");
		m_score += 500;
	} else if(lines > 3) {
		g_pSound->play(SOUND_TYPES_SE_TETRIS);
		printf("tetris\n");
		m_score += 800;
	}
}

void MainScreen::hardDrop()
{

	ivec2 fallPos = getFallPosition();
	//printf("fallPos: %d, %d\n", fallPos.x, fallPos.y);
	m_tetrimino.x = fallPos.x;
	m_tetrimino.y = fallPos.y;

	for (int y = 0; y < TETRIMINO_HEIGHT_MAX; y++) {
		for (int x = 0; x < TETRIMINO_WIDTH_MAX; x++) {
			if (m_tetrimino.shape.pattern[y][x]) {
				m_field[m_tetrimino.y + y][m_tetrimino.x + x] = m_tetrimino.type + 2;
			}
		}
	}

	eraseLine();

	if (m_bags.empty())
		shuffleBags();

	m_nowTetrimino = m_nextTetrimino;
	m_nextTetrimino = m_bags.back();
	m_bags.pop_back();

	initTetrimino();
}

void MainScreen::shadowTetrimino()
{
	ivec2 fallPos = getFallPosition();
	m_shadowMino = m_tetrimino;
	m_shadowMino.x = fallPos.x;
	m_shadowMino.y = fallPos.y;
}

ivec2 MainScreen::getFallPosition()
{
	TETRIMINO tetrimino = m_tetrimino, lastTetrimino;
	int y = 0;
	ivec2 result = ivec2(0);

	while (true)
	{
		lastTetrimino = tetrimino;
		tetrimino.y++;

		if (intersectField(tetrimino)) {

			result.x = lastTetrimino.x;
			result.y = lastTetrimino.y;
			break;
		}
	}

	return result;
}
