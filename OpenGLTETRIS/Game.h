#pragma once
#include "Screen.h"

enum
{
	GAME_SCREEN_MAIN,
	GAME_SCREEN_MAX
};

class Game
{
	int m_screen;
	Screen* m_pScreens[GAME_SCREEN_MAX];

public:
	Screen* m_pCurrentScreen;

	Game();

	void tick();
	void update();
	void setScreen(int _screen);
};

extern Game g_game;