// This is Nicklas Chiang's individual source file
//
#include "fonts.h"
#include <GL/glx.h>
#include <time.h>

static clock_t last_mouse_time = 0;
static double sec_elapsed = 0;

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
    if (!get){
        clock_t current_time = clock();

        sec_elapsed = (double)(current_time - last_mouse_time) / CLOCKS_PER_SEC;

        last_mouse_time = current_time;
    }

    if (get){
        return sec_elapsed;
    }

    return 0;
}
