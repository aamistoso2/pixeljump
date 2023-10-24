// Aldrin Amistoso's source file
//
#include <iostream>
#include <GL/glx.h>
using namespace std;
void display_aldrin() {
    /*
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 0x00ffff00, "Aldrin");
    */
    cout << "Aldrin\n";
}

int total_running_time(const bool get) {
    static int firsttime = 1;
    static int start_time;
    if (firsttime) {
        start_time = time(NULL);
        firsttime = 0;
    }
    if (get) {
        return time(NULL) - start_time;
    }
    return 0;
}
/*
int total_physics_call(const bool get) {
    
    return 0;
}
*/
