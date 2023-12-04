#ifndef _ALDRIN_A_
#define _ALDRIN_A_

extern int total_running_time(const bool get);
extern int total_physics_function_calls(const bool get);

extern void coinsCollection();
extern void playCollectedSound();
extern void display_coinsCollected();

//TO MAKE ADAM'S GAME RESET WORK
extern int collectedCoins;

#define MAX_COINS_X 6
#define MAX_COINS_Y 6
extern bool coinCollected[MAX_COINS_X][MAX_COINS_Y];

#endif

