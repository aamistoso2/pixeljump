// This is Nicklas Chiang's individual source file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <string.h>
#include "log.h"
#include "fonts.h"
#include "pixel.h"
#include "nchiang.h"

using namespace std;

const float timeslice = 1.0f;
const float gravity = -0.05f;
#define ALPHA 1;

extern Global gl;
extern X11_wrapper x11;
extern Level lev;

//Display name on screen
void display_name(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 0x00ffff00, 0, "Nicklas");
}

// ---------Sec Since Mouse Last Moved--------------------------
static clock_t last_mouse_time;
static clock_t current_time;
static int first_time = 1;
static double sec_elapsed;
const float JUMP_VELOCITY = 15.0;
static time_t startTime = time(nullptr);

int get_last_mouse_movement(const bool get)
{
    if (first_time){
        first_time = 0;
        sec_elapsed = 0;
        last_mouse_time = clock();
    }
    if (!get){
        sec_elapsed = 0;
    }
    if (get){
        current_time = clock();
        sec_elapsed += 
            (double)(current_time - last_mouse_time) / CLOCKS_PER_SEC;
        last_mouse_time = current_time;
        return sec_elapsed;
    }
    return 0;
}

// ---------Implementation of the Jump Functionality------------
void jumping() {
    if (gl.isJumping == 0) {
        gl.isJumping = 1;
        gl.ball_vel[1] = JUMP_VELOCITY;
    }
}

//Function to update if jumping
void updateJump() {
    if (gl.isJumping) {
        if (gl.jumpFrame < gl.maxJumpFrames) {
            // Apply jump strength for a certain number of frames
            gl.ball_vel[1] = JUMP_VELOCITY;
            gl.jumpFrame++;
        } else {
            gl.isJumping = 0;
            gl.jumpFrame = 0;
        }
    } else {
        // Apply gravity to bring the character down
        gl.ball_vel[1] -= gravity;
    }
}

//Function to display hp
void display_hp(int current_hp, int max_hp)
{
    Rect r;
    unsigned int c = 0x00ffff44;

    // Set the position for displaying HP in the top right corner
    r.bot = gl.yres - 20;
    r.left = gl.xres - 150;
    r.center = 0;

    // Calculate the percentage of health remaining
    int percentage = (int)(((float)current_hp / max_hp) * 100);

    // Display HP using ggprint8b function
    ggprint8b(&r, 16, c, "HP: %d/%d (%d%%)", current_hp, max_hp, percentage);

    if (current_hp <= 0) {
    current_hp = 0;
    display_game_over();
    }
}

//Function to display credits
void display_credits() {

    int windowWidth = gl.xres;
    int windowHeight = gl.yres;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4ub(0, 0, 0, 200);

    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(0, windowHeight);
    glVertex2f(windowWidth, windowHeight);
    glVertex2f(windowWidth, 0);
    glEnd();

    // Center the text within the window
    Rect r;
    r.bot = windowHeight / 2;
    r.left = windowWidth / 2;
    r.center = 1;

    ggprint13(&r, 20, 0x00ff0000, "Credits:");
    ggprint13(&r, 16, 0x00ff0000, "Aldrin Amistoso");
    ggprint13(&r, 16, 0x00ff0000, "Nicklas Chiang");
    ggprint13(&r, 16, 0x00ff0000, "Adam Ilarde");
    ggprint13(&r, 16, 0x00ff0000, "Daniel Lopez");
    ggprint13(&r, 16, 0x00ff0000, "Udai Singh");

    glDisable(GL_BLEND);

    return_to_menu();
}

//Function to display game over screen
//Right now need to manually set the hp to 0.
void display_game_over() {
    int windowWidth = gl.xres;
    int windowHeight = gl.yres;

    // Set the background color and transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4ub(0, 0, 0, 200);

    // Draw a rectangle to cover the entire window as the background
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(0, windowHeight);
    glVertex2f(windowWidth, windowHeight);
    glVertex2f(windowWidth, 0);
    glEnd();

    // Display the "Game Over" text centered on the screen
    Rect r;
    r.bot = windowHeight / 2 + 20;  // Adjusted for placement of "Press R to restart"
    r.left = windowWidth / 2;
    r.center = 1;

    ggprint13(&r, 30, 0x00ff0000, "Game Over");

    // Display the prompt to restart the game
    r.bot -= 40;
    ggprint13(&r, 16, 0x00ffffff, "Press 'R' to restart");

    glDisable(GL_BLEND);
}

//Function to display options on screen
void display_options()
{   
    int windowWidth = gl.xres;
    int windowHeight = gl.yres;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4ub(0, 0, 0, 200);

    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(0, windowHeight);
    glVertex2f(windowWidth, windowHeight);
    glVertex2f(windowWidth, 0);
    glEnd();

    // Center the text within the window
    Rect r;
    r.bot = windowHeight / 2;
    r.left = windowWidth / 2;
    r.center = 1;

    ggprint8b(&r, 16, 0xffffffff, "Spacebar -> Jump");
    ggprint8b(&r, 16, 0xffffffff, "Shift -> Sprint");
    ggprint8b(&r, 16, 0xffffffff, "Right Arrow or 'D' -> Walk Right");
    ggprint8b(&r, 16, 0xffffffff, "Left arrow or 'A'  <- Walk Left");
    ggprint8b(&r, 16, 0xffffffff, "E ->  Dash");
    ggprint8b(&r, 16, 0xffffffff, "frame: %i", gl.walkFrame);

    glDisable(GL_BLEND);

    return_to_menu();
}

//Function to allow returning to menu
void return_to_menu()
{
    bool returnToMenu = false;
    while (!returnToMenu) {
        XEvent event;
        while (XPending(x11.dpy)) {
            XNextEvent(x11.dpy, &event);
            switch (event.type) {
                case KeyPress:
                    // Check if 'q' is pressed to return to the menu
                    if (XLookupKeysym(&event.xkey, 0) == XK_q) {
                        returnToMenu = true;
                    }
                    break;
            }
        }
        // Swap buffers
        glXSwapBuffers(x11.dpy, x11.win);
        usleep(1000);
    }

    display_menu(); // Return to the menu after options
}

//Function to Display Main Menu
void display_menu() {
    int selectedOption = 0; // Track the selected option
    bool menuActive = true; // Control variable for the menu loop

    while (menuActive) {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Display menu options
        Rect r;
        r.bot = gl.yres / 2;
        r.left = gl.xres / 2;
        r.center = 1;
        unsigned int color = 0x00000000;

        ggprint13(&r, 45, 0x00ff0000, "PixelJump");
        ggprint13(&r, 20, color, selectedOption == 0 ? "> Play Game" : "Play Game");
        ggprint13(&r, 20, color, selectedOption == 1 ? "> Credits" : "Credits");
        ggprint13(&r, 45, color, selectedOption == 2 ? "> Options" : "Options");
        ggprint13(&r, 45, color, " Press Enter to select.");
        ggprint13(&r, 45, color, " Press q to return.");

        // Handle user input
        XEvent event;
        while (XPending(x11.dpy)) {
            XNextEvent(x11.dpy, &event);
            switch (event.type) {
                case KeyPress:
                    KeySym key;
                    char text[10];
                    int i = XLookupString(&event.xkey, text, 10, &key, 0);
                    if (i == 1) {
                        if (text[0] == '\r' || text[0] == '\n') {
                            // Perform actions based on selected option
                            switch (selectedOption) {
                                case 0:
                                    play_game();
                                    menuActive = false;
                                    break;
                                case 1:
                                    display_credits();
                                    break;
                                case 2:
                                    display_options();
                                    break;
                            }
                        } else if (text[0] == 'w' || text[0] == 'W') {
                            // Move selection up on 'W' or 'w'
                            selectedOption = (selectedOption - 1 + 3) % 3;
                        } else if (text[0] == 's' || text[0] == 'S') {
                            // Move selection down on 'S' or 's'
                            selectedOption = (selectedOption + 1) % 3;
                        } 
                    }
                    break;
            }
        }

        // Swap buffers
        glXSwapBuffers(x11.dpy, x11.win);
        usleep(1000);
    }
}
