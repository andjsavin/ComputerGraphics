#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <glut.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <iostream>

#define PI 3.14159265358979324
#define R 2.0
#define r 0.5

using namespace std;

// Globals.
static int p = 20; // Number of grid columns.
static int q = 12; // Number of grid rows
static float *vertices = NULL; // Vertex array of the mapped sample
static float Xangle = 150.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate

														 // Fuctions to map the grid vertex (u_i,v_j) to the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j))
float f(int i, int j)
{
	return (cos((8 * (float)j / q) * PI) * (3 + cos((2 * (float)i / p) * PI)));
}

float g(int i, int j)
{
	return (sin((8 * (float)j / q) * PI) * (3 + cos((2 * (float)i / p) * PI)));
}

float h(int i, int j)
{
	return (0.6 * ((8 * (float)j / q) * PI) + sin((2 * (float)j / q) * PI));
}

// Routine to fill the vertex array with co-ordinates of the mapped sample points.
void fillVertexArray(void)
{
	int i, j, k;

	k = 0;
	for (j = 0; j <= q; j++)
		for (i = 0; i <= p; i++)
		{
			vertices[k++] = f(i, j);
			vertices[k++] = g(i, j);
			vertices[k++] = h(i, j);
		}
}

// Initialization routine.
void setup(void)
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glClearColor(1.0, 1.0, 1.0, 0.0);
}

// Drawing routine.
void drawScene(void)
{
	int  i, j;
	vertices = new float[3 * (p + 1)*(q + 1)]; // Dynamic array allocation with new value of p and q. 

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(-30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColor3f(0.0, 0.0, 0.0);

	// Rotate scene.
	glRotatef(Zangle, 0.0, 0.0, 1.0);
	glRotatef(Yangle, 0.0, 1.0, 0.0);
	glRotatef(Xangle, 1.0, 0.0, 0.0);

	// Fill the vertex array.
	fillVertexArray();

	// Make the approximating triangular mesh.
	glTranslatef(0.0f, 10.0f, 0.0f);
	for (j = 0; j < q; j++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (i = 0; i <= p; i++)
		{
			glArrayElement((j + 1)*(p + 1) + i);
			glArrayElement(j*(p + 1) + i);
		}
		glEnd();
	}

	glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 1.0, 50.0);
	glMatrixMode(GL_MODELVIEW);

}

// Keyboard input processing routine.
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
	default:
		break;
	}
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT) if (p > 3) p -= 1;
	if (key == GLUT_KEY_RIGHT) p += 1;
	if (key == GLUT_KEY_DOWN) if (q > 3) q -= 1;
	if (key == GLUT_KEY_UP) q += 1;

	glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	cout << "Interaction:" << endl;
	cout << "Press left/right arrow keys to increase/decrease the number of grid columns." << endl
		<< "Press up/down arrow keys to increase/decrease the number of grid rows." << endl
		<< "Press x, X, y, Y, z, Z to turn the torus." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
	printInteraction();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("spring");
	setup();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glutMainLoop();

	return 0;
}