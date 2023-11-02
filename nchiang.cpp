// This is Nicklas Chiang's individual source file
//
#include "fonts.h"
#include <GL/glx.h>
#include <time.h>

// ---------Sec Since Mouse Last Moved--------------------------
static clock_t last_mouse_time;
static clock_t current_time;
static int first_time = 1;
static double sec_elapsed;


void display_name(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 0x00ffff00, 0, "Nicklas");
}

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
        sec_elapsed += (double)(current_time - last_mouse_time) / CLOCKS_PER_SEC;
        last_mouse_time = current_time;
        return sec_elapsed;
    }

    return 0;
}
// -------------------------------------------------------------
// ---------Implementation of the Jump Functionality------------
int JUMP_VELOCITY = 0.02;
void ball_jump(){
    if (gl.keys[XK_space]) {
        // Implement the jump functionality here
        if (gl.ball_pos[1] == (Flt)hgt) {
            // If on ground, allow to jump
            gl.ball_vel[1] = JUMP_VELOCITY;
        }
    }
}
// -------------------------------------------------------------
