#pragma once
#include "Screen.h"
#include "tetrimino.h"
#include "field.h"

class MainScreen :
	public Screen
{
public:
	virtual void init();
	virtual void reset();
	virtual void keyboard(unsigned char _key);
	virtual void tick();
	virtual void update();
	virtual void draw();

protected:
	int turn;
	int field[FIELD_HEIGHT][FIELD_WIDTH];
	int tetriminos[TETRIMINO_SHAPE_MAX];
	TETRIMINO m_tetrimino;

private:
	void initTetrimino();
	void shuffleTetriminos();
	bool intersectField();
	void eraseLine();
};

