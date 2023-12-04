// Source file for Daniel Lopez
#include <GL/glx.h>
#include "fonts.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "pixel.h"

extern Global gl;
extern Level lev;

void display_border(int xres, int yres) 
{
	float b = 50.0f;
	glColor3f(0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(b, b);
		glVertex2f(0.0f, yres);
		glVertex2f(b, yres - b);
		glVertex2f(xres, yres);
		glVertex2f(xres - b, yres - b);
		glVertex2f(xres, 0.0f);
		glVertex2f(xres - b, b);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(b, b);
	glEnd();
	glPopMatrix();
}


float mouse_movement_distance(int x, int y, bool get) {
	static float distance;
	static int first_time = 1;
	static int savex, savey;

	if (first_time) {
		distance  = 0;
		savex = x;
		savey = y;
		first_time = 0;
	}

	if (!get) {
		int dx = x - savex;
		int dy = y - savey;
		float new_distance = sqrt((dx * dx) + (dy * dy));
		distance += new_distance;
		savex = x;
		savey = y;
	}

	if (get) {
		return distance;
	}

	return 0;
}

int checkInAir(int floor) {
	int inAir = 0;
	int heightAboveFloor = gl.ball_pos[1] - floor;
	if (heightAboveFloor > 24) {
		inAir = 1;
	}
	return inAir;
}

void dash() {
	// dash right
	if (gl.facing == 1) {
		for (int i = 20; i > 0; i--) {
			gl.camera[0] += i;
		}
	}
	// dash left
	else if (gl.facing == 0) {
		for (int i = 20; i > 0; i--) {
			gl.camera[0] -= i;
		}
	}
}

int findFloor(int col, int row) {
	int floor = -999999;
	for (int i = lev.nrows - row; i < lev.nrows ; i++) {
        if (lev.arr[i][col] == 'b' || lev.arr[i][col] == 'w') {
            floor = ((lev.nrows - i) * lev.tilesize[1]);
            break;
        }
    }
	return floor;
}

int findCeiling(int col, int row) {
	int ceiling = 999999;
	for (int i = lev.nrows - row - 1; i >= 0 ; i--) {
        if (lev.arr[i][col] == 'b' || lev.arr[i][col] == 'w') {
            ceiling = (lev.nrows - i - 1) * lev.tilesize[1];
            break;
        }
    }
	return ceiling;
}

float findRightWall(int col, int row) {
	float rightWall = 0;
	int arrRow = (lev.nrows - row) - 1;
	for (int i = col; i < lev.ncols; i++) {
        if (lev.arr[arrRow][i] == 'b' || lev.arr[arrRow][i] == 'w') {
            break;
        }
        rightWall = rightWall + 1.0f;
    }
    return rightWall = (rightWall * lev.tilesize[0]);
}

float findLeftWall(int col, int row) {
	float leftWall = 0;
	int arrRow = (lev.nrows - row) - 1;
	for (int j = col; j >= 0; j--) {
        if (lev.arr[arrRow][j] == 'b' || lev.arr[arrRow][j] == 'w') {
            break;
        }
        leftWall = leftWall - 1.0f;
    }
    return leftWall = (leftWall * lev.tilesize[0]);
}