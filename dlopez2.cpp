// Source file for Daniel Lopez
#include <GL/glx.h>
#include "fonts.h"
#include <math.h>
#include <stdio.h>
#include <iostream>

void display_border(int xres, int yres) 
{
	int b = 50;
	glColor3f(0, 1, 0);
	glPushMatrix();
	glBegin(GL_TRIANGLES);
		glVertex2f(0, 0);
		glVertex2f(0+b, 0+b);
		glVertex2f(0, 0+yres);
		glVertex2f(b, yres-b);
		glVertex2f(xres, 0+yres);
		glVertex2f(xres-b, yres-b);
		glVertex2f(xres, 0);
		glVertex2f(xres-b, b);
		glVertex2f(0, 0);
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
