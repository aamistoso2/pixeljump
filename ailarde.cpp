// Name: Adam Ilarde
// File: Source file
// Date: 9/25/2023

//CLASS EXAMPLE---------------------------------------
#include "fonts.h"
#include <GL/glx.h>
#include <iostream>
#include <time.h>


/*
void display_border(int xres, int yres)
{
    //draw a border around the window
    int b = 50;
    glColor3f(1.0f, 1.0f, 0.0f);
    glPushMatrix();
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2i(0, 0);
    glVertex2i(0+b, 0+b);
    glVertex2i(0, 0+yres);
    glVertex2i(0, 0+yres-b);
    glVertex2i(xres, 0+yres);
    glVertex2i(xres-b, 0+yres-b);
    glVertex2i(xres, 0);
    glVertex2i(xres-b, b);
    glVertex2i(0, 0);
    glVertex2i(0+b, 0+b);
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
*/

/*----------------------------------------------------
//MAIN MENU
int displayMainMenu()
{
int choice;

wnile (true)
{

std::cout << "Main Menu" << std:endl;
std::cout << "Main Menu" << std:endl;
std::cout << "Main Menu" << std:endl;
std::cout << "Main Menu" << std:endl;
std::cout << "Main Menu" << std:endl;
}
}
//----------------------------------------------------
*/

//----------------------------------------------------
//LAB 9
//

static clock_t last_key_time;
static clock_t timer;
static int first = 1;
static double time_passes;

/*
int total_run_time(const bool get)
{
   // static int first = 1;
    static int start;
    if (first)
    {
        start = time(NULL);
        first = 0;
    }
    if (get)
    {
        return time(NULL) - start;
    }
    return 0;
}
*/


int time_key_press(const bool get2)
{
    if (first)
    {
        first = 0;
        time_passes = 0;
        last_key_time = clock();
    }

    if (get2)
    {
        timer = clock();
        time_passes += (double)(timer - last_key_time) / CLOCKS_PER_SEC;
        last_key_time = timer;
        return time_passes;
    }

    if(!get2)
    {
        //clock_t time = clock();
        //time_passes = (double)(time - last_key_time) / CLOCKS_PER_SEC;
        //last_key_time = time;

        time_passes = 0;
    }

    return 0;

}

//----------------------------------------------------
