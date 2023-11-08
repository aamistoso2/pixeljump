// This is Nicklas Chiang's individual source file
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include "pixel.h"
#include "nchiang.h"

const float timeslice = 1.0f;
const float gravity = -0.2f;
#define ALPHA 1

//Class from pixeljump.cpp
extern Global gl;

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
const float JUMP_VELOCITY = 1.0;
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
// -------------------------------------------------------------
// ---------Implementation of the Jump Functionality------------
void jumping() {
    if (gl.isJumping == 0) {
        gl.isJumping = 1;
        gl.ball_vel[1] = JUMP_VELOCITY;
    }
}

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

//Potentially HUD

//Might need to fix window resolution-----------------------

void startup() 
{
    //code to print menu and title
}


//Game Over Screen -----------------------------
void endgame()
{
    //Code for the game over 
}

//Credits screen --------------------------------
void credits()
{
    string name = "Pixels:";
    //Shade of purple
    float color[3] = {0.682f, 0.0f, 1.0f};
    float top_left[2];
    top_left[0] = 0.0f;
    //Center vertical line
    top_left[1] = gl.yres / 2;
    draw_text(top_left, gl.xres, gl.yres/4, gl.xres/15, gl.yres/15, color, 
                                                                    name);
    //color[0] = ;
    //color[1] = ;
    //color[2] = ;

    // Aldrin
    top_left[1] = gl.yres / 2 - gl.yres/6;
    name = "Aldrin Amistoso";
    draw_text(top_left, gl.xres, gl.yres/6, gl.xres/25, gl.yres/25, color, 
                                                                    name);

    // Nicklas
    top_left[1] = gl.yres / 2 - (gl.yres/6 + gl.yres/25);
    name = "Nicklas Chiang";
    draw_text(top_left, gl.xres, gl.yres/6, gl.xres/25, gl.yres/25, color, 
                                                                    name);

    // Adam
    top_left[1] = gl.yres / 2 - (gl.yres/6 + 2 * gl.yres/25);
    name = "Adam Ilarde";
    draw_text(top_left, gl.xres, gl.yres/6, gl.xres/25, gl.yres/25, color, 
                                                                    name);

    // Daniel
    top_left[1] = gl.yres / 2 - (gl.yres/6 + 3 * gl.yres/25);
    name = "Daniel Lopez";
    draw_text(top_left, gl.xres, gl.yres/6, gl.xres/25, gl.yres/25, color, 
                                                                    name);

    // Udai
    top_left[1] = gl.yres / 2 - (gl.yres/6 + 4 * gl.yres/25);
    name = "Udai Singh";
    draw_text(top_left, gl.xres, gl.yres/6, gl.xres/25, gl.yres/25, color, 
                                                                    name);
}
// -----------------------------------------------------------------------

// Player health, if hp drops to 0, show endgame screen
void check_hp(int g.hp)
{
    if (g.hp <= 0) {
        screens(endgame);
    } else {
        g.hp = g.current_hp;
    }
}
