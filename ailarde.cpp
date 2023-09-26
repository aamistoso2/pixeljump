// Name: Adam Ilarde
// File: Source file
// Date: 9/25/2023

//CLASS EXAMPLE---------------------------------------
#include <fonts.h>
#include <GL/glx.h>

void display_border(int xres, int yres)
{
    //draw a border around the window
    int b = 50;
    glColor3f(1.0f, 1.0f, 0.0f);
    glPushMatrix();
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2fi(0, 0);
    glVertex2fi(0+b, 0+b);
    glVertex2fi(0, 0+yres);
    glVertex2fi(0, 0+yres-b);
    glVertex2fi(xres, 0+yres);
    glVertex2fi(xres-b, 0+yres-b);
    glVertex2fi(xres, 0);
    glVertex2fi(xres-b, b);
    glVertex2fi(0, 0);
    glVertex2fi(0+b, 0+b);
    glEnd();
    glPopMatrix();
}

void display_name(int x, int y)
{
    //draw a border

    Rect r;
    r.bot =y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 0 , 0x00ffff00, "ailarde");
}
//----------------------------------------------------

