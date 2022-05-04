#include "Game.h"
#include "MainScreen.h"

#include <gl/glut.h>

Game g_game;

Game::Game()
	:m_screen(0)
	,m_pScreens()
	,m_pCurrentScreen(NULL)
{}

void Game::init()
{
	m_pScreens[GAME_SCREEN_MAIN] = new MainScreen();

	for (int i = 0; i < GAME_SCREEN_MAX; i++) {
		m_pScreens[i]->init();
	}

	m_screen = GAME_SCREEN_MAIN;
	m_pCurrentScreen = m_pScreens[m_screen];
}

void Game::tick()
{

}

void Game::update()
{

}

void Game::setScreen(int _screen)
{
	m_pCurrentScreen = m_pScreens[m_screen = _screen];
	m_pCurrentScreen->reset();
}