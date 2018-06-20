#ifndef _CUBE_H
#define _CUBE_H

#include "small_cube.h"

class Cube
{
	Small_Cube a[3][3][3];
	int rotate[6];
	double size;
	unsigned int color[6];

public:
	int active;

	Cube()
	{
	}

	Cube(double size, unsigned int *color)
	{
		cube_init(size, color);
	}

	void cube_init(double size, unsigned int *color)
	{
		memset(rotate, 0, sizeof(rotate));
		this->size = size;
		active = -1;

		int i, j, k;
		for (i = 0; i < 6; i++)
			this->color[i] = color[i];

		//up
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				a[i][j][2].setColor(0, color[0]);

		//down
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				a[i][j][0].setColor(1, color[1]);

		//front
		for (k = 0; k < 3; k++)
			for (j = 0; j < 3; j++)
				a[j][0][k].setColor(2, color[2]);

		//back
		for (k = 0; k < 3; k++)
			for (j = 0; j < 3; j++)
				a[j][2][k].setColor(3, color[3]);

		//left
		for (i = 0; i < 3; i++)
			for (k = 0; k < 3; k++)
				a[0][k][i].setColor(4, color[4]);

		//right
		for (i = 0; i < 3; i++)
			for (k = 0; k < 3; k++)
				a[2][k][i].setColor(5, color[5]);

		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				for (k = 0; k < 3; k++)
					a[i][j][k].size = (size / 3.0) * 0.95;
	}

	void draw()
	{
		const double K = 0.65;
		glPushMatrix();
		glColor3f(0, 0, 0);
		glTranslatef(((1.0 - K) / 2)*size + K * size / 2, ((1.0 - K) / 2)*size + K * size / 2, ((1.0 - K) / 2)*size + K * size / 2);
		glutSolidCube(size * K);
		glPopMatrix();

		memset(still, true, sizeof(still));
		if (active != -1)
		{
			glPushMatrix();
			int i, j, k;

			if (active == 0 || active == 1)
			{
				k = (active & 1) * 2;
				for (i = 0; i < 3; i++)
					for (j = 0; j < 3; j++)
						still[i][j][k] = false;

				
				glTranslated(size / 2, size / 2, 0);   
				glRotatef(rotate[active], 0, 0, 1);   
				glTranslated(-size / 2, -size / 2, 0); 
				for (i = 0; i < 3; i++)
					for (j = 0; j < 3; j++)
						a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
			}

			else if (active == 2 || active == 3)
			{
				j = (active & 1) * 2;
				for (i = 0; i < 3; i++)
					for (k = 0; k < 3; k++)
						still[i][j][k] = false;

				glTranslated(size / 2, 0, size / 2);
				glRotatef(rotate[active], 0, 1, 0);
				glTranslated(-size / 2, 0, -size / 2);
				for (i = 0; i < 3; i++)
					for (k = 0; k < 3; k++)
						a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
			}

			else if (active == 4 || active == 5)
			{
				i = (active & 1) * 2;
				for (j = 0; j < 3; j++)
					for (k = 0; k < 3; k++)
						still[i][j][k] = false;

				glTranslated(0, size / 2, size / 2);
				glRotatef(rotate[active], 1, 0, 0);
				glTranslated(0, -size / 2, -size / 2);
				for (j = 0; j < 3; j++)
					for (k = 0; k < 3; k++)
						a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
			}
			glPopMatrix();
		}

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
					if (still[i][j][k])
						a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
	}

public:
	void rot90(int num, int direction)
	{
		int i, j, k;
		if (direction == -1)
			direction = 3;
		while (direction--)
		{
			if (num == 0 || num == 1)
			{
				//up/down
				k = (num & 1) * 2;
				for (i = 0; i < 3; i++)
					for (j = 0; j < 3; j++)
						tmp[j][2 - i] = a[i][j][k];
				for (i = 0; i < 3; i++)
					for (j = 0; j < 3; j++)
						tmp[i][j].rotZ(), a[i][j][k] = tmp[i][j];
			}
			else if (num == 2 || num == 3)
			{
				//left/right
				j = (num & 1) * 2;
				for (i = 0; i < 3; i++)
					for (k = 0; k < 3; k++)
						tmp[k][2 - i] = a[i][j][k];
				for (i = 0; i < 3; i++)
					for (k = 0; k < 3; k++)
						tmp[i][k].rotX(), a[i][j][k] = tmp[i][k];
			}
			else if (num == 4 || num == 5)
			{
				//front/back
				i = (num & 1) * 2;
				for (j = 0; j < 3; j++)
					for (k = 0; k < 3; k++)
						tmp[k][2 - j] = a[i][j][k];
				for (j = 0; j < 3; j++)
					for (k = 0; k < 3; k++)
						tmp[j][k].rotY(), a[i][j][k] = tmp[j][k];
			}
		}
	}

	void Rotate(int num, int angle)
	{
		if (active == -1 || active == num)
		{
			rotate[num] += angle;

			if (rotate[num] % 90 != 0)
			{
				active = num;
			}
			else
			{
				if ((rotate[num] < 0) ^ (active == 2 || active == 3))
					rot90(num, 1);
				else
					rot90(num, -1);
				rotate[num] = 0;
				active = -1;
			}
		}
	}
private:
	int _angle[3];
	bool still[3][3][3];
	Small_Cube tmp[3][3];
};

#endif;