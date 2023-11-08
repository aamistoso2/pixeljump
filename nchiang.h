#ifndef _NICKLAS_C_
#define _NICKLAS_C_

enum screen_list {
    startup,
    play_game,
    credits,
    endgame
};

extern void display_name(int x, int y);
extern int get_last_mouse_movement(const bool yes);
//Jumping
extern void jumping();
extern void updateJump();

//Screens
extern void startup();
extern void credits();
extern void endgame();
extern void check_hp(int g.hp);

#endif
