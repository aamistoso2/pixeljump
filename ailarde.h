#ifndef _AILARDE_H_
#define _AILARDE_H_

extern void displayName(int, int);
extern void displayBorder(int xres, int yres);

extern int time_key_press(const bool yes);
extern void showCoins(int, int);
extern void showSpikes(int, int, float);

extern bool spikesDetection(int, int, int, int, float);

extern void resetGame();
#endif

