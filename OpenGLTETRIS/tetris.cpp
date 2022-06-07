#include "tetris.h"
#include "field.h"
#include "font.h"
#include "audio.h"
#include "Rect.h"
#include "Game.h"

#include <time.h>
#include <gl/glut.h>
#include <glm/glm.hpp>

bool g_keys[256];

using namespace glm;

void release()
{
	audioRelease();
	fontRelease();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(
		0,				// GLdouble left
		SCREEN_WIDTH,	// GLdouble right
		SCREEN_HEIGHT,	// GLdouble bottom
		0);				// GLdouble top

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	g_game.m_pCurrentScreen->draw();

	glutSwapBuffers();
}

void idle()
{
	if (g_keys[0x1b]) {
		release();
		exit(0);
	}

	g_game.m_pCurrentScreen->update();

	glutPostRedisplay();
}

void timer(int value)
{
	g_game.m_pCurrentScreen->tick();

	//glutPostRedisplay();

	glutTimerFunc(
		g_game.getTime(),	// unsigned int time
		timer,				// void (* callback)( int )
		0);					// int value
}

void keyboard(unsigned char key, int x, int y)
{
	//printf("key=%#x\n", key);
	g_keys[key] = true;
	g_game.m_pCurrentScreen->keyboard(key);
}

void keyboardUp(unsigned char key, int x, int y)
{
	//printf("key=%#x\n", key);
	g_keys[key] = false;
}

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(720, 480);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("TETRIS");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutTimerFunc(1000, timer, 0);

	glClearColor(0, 0, 0, 1);
	
	fontInit();
	audioInit();

	g_game.init();

	glutMainLoop();
}