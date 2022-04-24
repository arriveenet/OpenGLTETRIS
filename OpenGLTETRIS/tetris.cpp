#include <gl/glut.h>

#include "tetris.h"
#include "field.h"
#include "Rect.h"

#include <glm/glm.hpp>

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

	glEnableClientState(GL_VERTEX_ARRAY);
	fieldDraw();


	glutSwapBuffers();
}

void idle()
{

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(720, 480);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("TETRIS");

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glClearColor(0, 0, 0, 1);

	fieldInit();

	glutMainLoop();
}