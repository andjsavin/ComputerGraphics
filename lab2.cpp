#include "stdafx.h"
#include "windows.h""
#include <glaux.h>
#include <glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

static float Xangle = 90.0, Yangle = -180.0, Zangle = 0.0;
const float pi = 3.14159;
float height = 0.5;
GLfloat range = 100.0f;
GLfloat zoom = 1.5f;
GLUquadricObj* quadric;

GLuint texture[3];

//Top parallelepiped coordinates
static GLfloat v_cord[6][4][2] = {
	{ { 0, 1 },{ 1, 1 },{ 1, 0 },{ 0, 0 } },
{ { 0, 0 },{ 1, 0 },{ 1, 1 },{ 0, 1 } },

{ { 1, 0 },{ 1, 1 },{ 0, 1 },{ 0, 0 } },
{ { 1, 0 },{ 1, 1 },{ 0, 1 },{ 0, 0 } },

{ { 0, 0 },{ 1, 0 },{ 1, 1 },{ 0, 1 } },
{ { 1, 0 },{ 1, 1 },{ 0, 1 },{ 0, 0 } }
};

static GLfloat v_vertex[6][4][3] = {
	{ { 10.0, -2.0, -1.0 },{ 10.0, 10.0, -1.0 },{ 10.0, 10.0, 1.0 },{ 10.0, -2.0, 1.0 } },
{ { -2.0, -2.0, -1.0 },{ -2.0, -2.0, 1.0 },{ -2.0, 10.0, 1.0 },{ -2.0, 10.0, -1.0 } },

{ { -2.0, 10.0, -1.0 },{ -2.0, 10.0, 1.0 },{ 10.0, 10.0, 1.0 },{ 10.0, 10.0, -1.0 } },
{ { -2.0, -2.0, -1.0 },{ 10.0, -2.0, -1.0 },{ 10.0, -2.0, 1.0 },{ -2.0, -2.0, 1.0 } },

{ { -2.0, -2.0, 1.0 },{ 10.0, -2.0, 1.0 },{ 10.0, 10.0, 1.0 },{ -2.0, 10.0, 1.0 } },
{ { -2.0, -2.0, -1.0 },{ -2.0, 10.0, -1.0 },{ 10.0, 10.0, -1.0 },{ 10.0, -2.0, -1.0 } }
};

static GLfloat v_cordpin1[6][4][2] = {
	{ { 0, 1 },{ 1, 1 },{ 1, 0 },{ 0, 0 } },
{ { 0, 0 },{ 1, 0 },{ 1, 1 },{ 0, 1 } },

{ { 1, 0 },{ 1, 1 },{ 0, 1 },{ 0, 0 } },
{ { 1, 0 },{ 1, 1 },{ 0, 1 },{ 0, 0 } },

{ { 0, 0 },{ 1, 0 },{ 1, 1 },{ 0, 1 } },
{ { 1, 0 },{ 1, 1 },{ 0, 1 },{ 0, 0 } }
};

static GLfloat v_vertexpin1[6][4][3] = {
	{ { 7.5, 0.0, -1.0 },{ 7.5, 2.0, -1.0 },{ 7.5, 2.0, 6.0 },{ 7.5, 0.0, 6.0 } },
{ { 5.5, 0.0, -1.0 },{ 5.5, 0.0, 6.0 },{ 5.5, 2.0, 6.0 },{ 5.5, 2.0, -1.0 } },

{ { 5.5, 2.0, -1.0 },{ 5.5, 2.0, 6.0 },{ 7.5, 2.0, 6.0 },{ 7.5, 2.0, -1.0 } },
{ { 5.5, 0.0, -1.0 },{ 7.5, 0.0, -1.0 },{ 7.5, 0.0, 6.0 },{ 5.5, 0.0, 6.0 } },

{ { 5.5, 0.0, 6.0 },{ 7.5, 0.0, 6.0 },{ 7.5, 2.0, 6.0 },{ 5.5, 2.0, 6.0 } },
{ { 5.5, 0.0, -1.0 },{ 5.5, 2.0, -1.0 },{ 7.5, 2.0, -1.0 },{ 7.5, 0.0, -1.0 } }
};

static GLfloat v_cordpin2[6][4][2] = {
	{ { 0, 1 },{ 1, 1 },{ 1, 0 },{ 0, 0 } },
{ { 0, 0 },{ 1, 0 },{ 1, 1 },{ 0, 1 } },

{ { 1, 0 },{ 1, 1 },{ 0, 1 },{ 0, 0 } },
{ { 1, 0 },{ 1, 1 },{ 0, 1 },{ 0, 0 } },

{ { 0, 0 },{ 1, 0 },{ 1, 1 },{ 0, 1 } },
{ { 1, 0 },{ 1, 1 },{ 0, 1 },{ 0, 0 } }
};

static GLfloat v_vertexpin2[6][4][3] = {
	{ { 0.0,-2.0, -10.5 },{ 0.0, 0.0, -10.5 },{ 0.0, 0.0, -4.5 },{ 0.0,-2.0, -4.5 } },
{ { -2.0,-2.0, -10.5 },{ -2.0,-2.0, -4.5 },{ -2.0, 0.0, -4.5 },{ -2.0, 0.0, -10.5 } },

{ { -2.0, 0.0, -10.5 },{ -2.0, 0.0, -4.5 },{ 0.0, 0.0, -4.5 },{ 0.0, 0.0, -10.5 } },
{ { -2.0,-2.0, -10.5 },{ 0.0,-2.0, -10.5 },{ 0.0,-2.0, -4.5 },{ -2.0,-2.0, -4.5 } },

{ { -2.0,-2.0, -4.5 },{ 0.0,-2.0, -4.5 },{ 0.0, 0.0, -4.5 },{ -2.0, 0.0, -4.5 } },
{ { -2.0,-2.0, -10.5 },{ -2.0, 0.0, -10.5 },{ 0.0, 0.0, -10.5 },{ 0.0,-2.0, -10.5 } }
};


void drawCube()
{
	glBegin(GL_QUADS);
	for (short i = 0; i < 6; i++)
	{
		for (short j = 0; j < 4; j++)
		{
			glTexCoord2fv(&v_cord[i][j][0]);
			glVertex3fv(&v_vertex[i][j][0]);
		}
	}
	glEnd();
}

void drawCubepin1()
{
	glBegin(GL_QUADS);
	for (short i = 0; i < 6; i++)
	{
		for (short j = 0; j < 4; j++)
		{
			glTexCoord2fv(&v_cordpin1[i][j][0]);
			glVertex3fv(&v_vertexpin1[i][j][0]);
		}
	}
	glEnd();
}

void drawCubepin2()
{
	glBegin(GL_QUADS);
	for (short i = 0; i < 6; i++)
	{
		for (short j = 0; j < 4; j++)
		{
			glTexCoord2fv(&v_cordpin2[i][j][0]);
			glVertex3fv(&v_vertexpin2[i][j][0]);
		}
	}
	glEnd();
}

const char* textureFiles[3] = { "spring.bmp","brick.bmp","ball.bmp" };

AUX_RGBImageRec* openFile(const char* fileName)
{
	FILE* file = nullptr;
	if (!fileName)
		return nullptr;
	file = fopen(fileName, "r");
	if (file)
	{
		fclose(file);
		return auxDIBImageLoad(fileName);
	}
	return nullptr;
}

int loadTextures(void)
{
	int status = FALSE;
	AUX_RGBImageRec* textureImage[3];

	for (int i = 0; i < 3; i++)
	{
		status = FALSE;
		if (textureImage[i] = openFile(textureFiles[i]))
		{
			status = TRUE;
			glGenTextures(i + 1, &texture[i]);
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, textureImage[i]->sizeX,
				textureImage[i]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, textureImage[i]->data);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		if (textureImage[i])
		{
			if (textureImage[i]->data)
				free(textureImage[i]->data);
			free(textureImage[i]);
		}
	}
	return status;
}

GLUquadric* sphereQuad;

void init(void)
{
	if (!loadTextures())
	{
		perror("Missing texture files!");
		exit(1);
	}

	sphereQuad = gluNewQuadric();

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}


void display(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	float dt = 18.4 * pi / 100;
	float du = 2 * pi / 10;
	float t = 0, u = 0;


	GLfloat x = cos(t) * (3 + cos(u));
	GLfloat y = sin(t) * (3 + cos(u));
	GLfloat z = height * (t)+sin(u);

	glPushMatrix();
	glTranslatef(0.0f, 80.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glScalef(zoom, zoom, zoom);
	glRotatef(Zangle, 0.0, 0.0, 1.0);
	glRotatef(Yangle, 0.0, 1.0, 0.0);
	glRotatef(Xangle, 1.0, 0.0, 0.0);
	drawCube();
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	drawCubepin1();
	glTranslatef(0.0f, 0.0f, 5.0f);
	glTranslatef(x, y, z);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	for (t = 0; t < 18.4 * pi; t += dt)
	{
		for (u = 0; u < 2 * pi; u += du)
		{
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glBegin(GL_QUADS);

			glTexCoord2f(0.0, 0.0);
			x = cos(t) * (3 + cos(u));
			y = sin(t) * (3 + cos(u));
			z = height * (t)+sin(u);

			glVertex3f(x, y, z);

			glTexCoord2f(0.0, 1.0);
			x = cos(t + dt) * (3 + cos(u));
			y = sin(t + dt) * (3 + cos(u));
			z = height * (t + dt) + sin(u);

			glVertex3f(x, y, z);

			glTexCoord2f(1.0, 1.0);
			x = cos(t + dt) * (3 + cos(u + du));
			y = sin(t + dt) * (3 + cos(u + du));
			z = height * (t + dt) + sin(u + du);

			glVertex3f(x, y, z);

			glTexCoord2f(1.0, 0.0);
			x = cos(t) * (3 + cos(u + du));
			y = sin(t) * (3 + cos(u + du));
			z = height * (t)+sin(u + du);

			glVertex3f(x, y, z);

			glEnd();
		}
	}

	x = cos(18.4 * pi) * (3 + cos(2 * pi + du));
	y = sin(18.4 * pi) * (3 + cos(2 * pi + du));
	z = height * (18.4 * pi) + sin(2 * pi + du);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTranslatef(x, y, z + 10);
	drawCubepin2();
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	gluSphere(quadric, 7, 50.0, 50.0);

	glPopMatrix();
	glFlush();
}

void reshape(int w, int h)
{
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-range, range, -range * h / w, range * h / w, -range, range);
	else
		glOrtho(-range * w / h, range * w / h, -range, range, -range, range);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool processFlag = false;

void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'x':
		Xangle += 5.0;
		if (Xangle > 360.0) Xangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'X':
		Xangle -= 5.0;
		if (Xangle < 0.0) Xangle += 360.0;
		glutPostRedisplay();
		break;
	case 'y':
		Yangle += 5.0;
		if (Yangle > 360.0) Yangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Y':
		Yangle -= 5.0;
		if (Yangle < 0.0) Yangle += 360.0;
		glutPostRedisplay();
		break;
	case 'z':
		Zangle += 5.0;
		if (Zangle > 360.0) Zangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Z':
		Zangle -= 5.0;
		if (Zangle < 0.0) Zangle += 360.0;
		glutPostRedisplay();
		break;
	case 'f':
		zoom += 0.05;
		glutPostRedisplay();
		break;
	case 'b':
		zoom -= 0.05;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

unsigned time_step = 20;

int f = 0;
void process(int value)
{
	if (processFlag)
	{
		if (height >= 1.5 && f == 0) f = 1;
		if (height <= 0.5 && f == 1) f = 0;
		if (f == 0) {
			height += 0.01;
		}
		else {
			height -= 0.01;
		}
		glutPostRedisplay();
	}
	glutTimerFunc(time_step, process, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(50.0, 50.0);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(75, 1.68, 0.5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glutKeyboardFunc(keyInput);
	glutTimerFunc(time_step, process, 0);
	glutMainLoop();
	return 0;
}
