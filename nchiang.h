#ifndef _NICKLAS_C_
#define _NICKLAS_C_

#include <string>
#include <string.h>

using namespace std;


extern void display_name(int x, int y);
extern int get_last_mouse_movement(const bool yes);

//Jumping
extern void jumping();
extern void updateJump();

extern void display_hp(int current_hp, int max_hp);

//Screebs
extern void display_credits();
extern void display_game_over();
extern void display_menu();
extern void display_options();
extern void return_to_menu();


#endif
