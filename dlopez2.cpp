// Source file for Daniel Lopez
#include <GL/glx.h>
#include "fonts.h"

void display_border(int xres, int yres) 
{
	int b = 50;
	glColor3f();
	glPushMatrix();
	glBegin(GL_TRIANGLES);
		glVertex2f(0, 0);
		glVertex2f(0+b, 0+b);
		glVertex2f(0, 0+yres);
		glVertex2f(0+b, 0+yres-b);
		glVertex2f(xres, 0+yres);
		glVertex2f(xres-b, yres-b);
		glVertex2f(xres, 0);
		glVertex2f(xres-b, b);
		glVertex2f(0, 0);
		glVertex2f(b, b);

	glEnd();
	glPopMatrix();
}
