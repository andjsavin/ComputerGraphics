#include "cube.h"
#include <time.h>
#include <stdlib.h>

#define CUBE_SIZE 13
#define TIMER 30
//                  (up,       down,     front,    back,     left,     right)
unsigned int c[6] = {0xFFFFFF, 0xFFFF00, 0x0000FF, 0x00FF00, 0xFF0000, 0xFF00FF};
GLfloat lightSource[] = { 0, 100, 200, 0 };
int xAngle = 24, yAngle = 34, zAngle = 0;
double translateZ = -35.0;
Cube cube;
int autoRotate = 0;

void display()
{
	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 0, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightSource);
	glTranslatef(0, 0, translateZ);
	glRotatef(xAngle, 1, 0, 0);
	glRotatef(yAngle, 0, 1, 0);
	glTranslatef(CUBE_SIZE / -2.0, CUBE_SIZE / -2.0, CUBE_SIZE / -2.0);
	cube.draw();
	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat fAspect = (GLfloat)w / (GLfloat)h;
	gluPerspective(60, fAspect, 1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	srand(time(0));
	float mat_specular[] = { 0.3, 0.3, 0.3, 0 };
	float diffuseLight[] = { 0.2, 0.2, 0.2, 1 };
	float ambientLight[] = { 0.9, 0.9, 0.9, 1.0 };
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	cube.cube_init(CUBE_SIZE, c);
}

void specialKeys(int key, int, int)
{
	if (key == GLUT_KEY_DOWN)
	{
		xAngle += 3;
		if (xAngle >= 360)
			xAngle -= 360;
		glutPostRedisplay();
	}

	if (key == GLUT_KEY_UP)
	{
		xAngle -= 3;
		if (xAngle < 0)
			xAngle += 360;
		glutPostRedisplay();
	}

	if (key == GLUT_KEY_RIGHT)
	{
		yAngle += 3;
		if (yAngle >= 360)
			yAngle -= 360;
		glutPostRedisplay();
	}

	if (key == GLUT_KEY_LEFT)
	{
		yAngle -= 3;
		if (yAngle < 0)
			yAngle += 360;
		glutPostRedisplay();
	}

	if (key == GLUT_KEY_F2)
	{
		translateZ += 5;
		glutPostRedisplay();
	}

	if (key == GLUT_KEY_F3)
	{
		translateZ -= 5;
		glutPostRedisplay();
	}

	if (key == GLUT_KEY_F1)
	{
		cube.cube_init(CUBE_SIZE, c);
		glutPostRedisplay();
	}
}

void keys(unsigned char key, int, int)
{
	// если нажали клавишу от 0 до 5 - начинаем поворот на 3 градуса
	if (cube.active == -1 && key >= '0' && key < '6')
	{
		cube.Rotate(key - '0', 3);
		display();
	}
}

void mouse(int key, int state, int, int)
{
	if (key == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		autoRotate = 1 - autoRotate;
	}
}

void timer(int)
{
	glutTimerFunc(TIMER, timer, 0);
	if (autoRotate)
	{
		if (cube.active == -1)
			keys(rand() % 6 + '0', 0, 0);
		else
			cube.Rotate(cube.active, 3);
	}
	else
	{
		if (cube.active != -1)
			cube.Rotate(cube.active, 3);
	}
	display();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 700);
	glutInitWindowPosition(1, 1);
	glutCreateWindow("Cube");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keys);
	glutMouseFunc(mouse);
	glutTimerFunc(TIMER, timer, 0);
	glutSpecialFunc(specialKeys);
	glutMainLoop();
	return 0;
}