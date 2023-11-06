// This is Nicklas Chiang's individual source file
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include "nchiang.h"

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(v, x, y, z) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                             (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define ALPHA 1

class global{
    public:
    int isJumping = 0;
    int jumpFrame = 0;
    int maxJumpFrames = 15;
    Vec ball_vel;

}g;


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
const float JUMP_VELOCITY = 0.5;
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
        sec_elapsed += (double)(current_time - last_mouse_time) / CLOCKS_PER_SEC;
        last_mouse_time = current_time;
        return sec_elapsed;
    }

    return 0;
}
// -------------------------------------------------------------
// ---------Implementation of the Jump Functionality------------
void jumping() {
    if (g.isJumping == 0) {
        g.isJumping = 1;
        g.ball_vel[1] = JUMP_VELOCITY;
    }
}

void updateJump() {
    if (g.isJumping) {
        if (g.jumpFrame < g.maxJumpFrames) {
            // Apply jump strength for a certain number of frames
            g.ball_vel[1] = JUMP_VELOCITY;
            g.jumpFrame++;
        } else {
            g.isJumping = 0;
            g.jumpFrame = 0;
        }
    } else {
        // Apply gravity to bring the character down
        g.ball_vel[1] -= gravity;
    }
}
