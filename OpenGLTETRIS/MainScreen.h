#pragma once
#include "Screen.h"
#include "tetrimino.h"
#include "field.h"

#include <vector>

#define KEY_BIND_HARD_DROP		0x20
#define KEY_BIND_MOVE_LEFT		0x61
#define KEY_BIND_HOLD			0x63
#define KEY_BIND_MOVE_RIGHT		0x64
#define KEY_BIND_SOFT_DROP		0x73
#define KEY_BIND_ROTATE_RIGHT	0x77


class MainScreen :
	public Screen
{
public:
	MainScreen();
	virtual void init();
	virtual void reset();
	virtual void keyboard(unsigned char _key);
	virtual void tick();
	virtual void update();
	virtual void draw();

protected:
	int m_nowTetrimino;
	int m_nextTetrimino;
	int m_holdTetrimino;
	int m_field[FIELD_HEIGHT][FIELD_WIDTH];
	unsigned int m_lines;
	unsigned int m_score;
	std::vector<int> m_bags;
	TETRIMINO m_tetrimino;
	TETRIMINO m_shadowMino;

private:
	void initTetrimino();
	void shuffleBags();
	bool intersectField(const TETRIMINO& _tetrimino);
	void eraseLine();
	void hardDrop();
	void shadowTetrimino();
	glm::ivec2 getFallPosition();
};

