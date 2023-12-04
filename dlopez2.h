#ifndef _DANIEL_L_
#define _DANIEL_L_

extern void display_border(int xres, int yres);
extern float mouse_movement_distance(int x, int y, bool get);
extern int checkInAir(int floor);
extern void dash();
extern void updateDash();
extern float findRightWall(int col, int row);
extern float findLeftWall(int col, int row);
extern int findFloor(int col, int row);
extern int findCeiling(int col, int row);
extern void playJumpSound();
extern int damageImmune();

#endif
