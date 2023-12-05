// Aldrin Amistoso's source file
//
#include <iostream>
#include <GL/glx.h>
#include "aamistoso.h"  
#include "pixel.h"
#include <string.h>
#include "log.h"
#include "fonts.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
using namespace std;

extern Global gl;
extern Level lev;
#define MAX_COINS_X 6
#define MAX_COINS_Y 6

extern int coinFixedX[MAX_COINS_X][MAX_COINS_Y];
extern int coinFixedY[MAX_COINS_X][MAX_COINS_Y];
extern bool coinCollected[MAX_COINS_X][MAX_COINS_Y];

int collectedCoins = 0;

#define MAX_COINS_X 6
#define MAX_COINS_Y 6

void playCollectSound();

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
static int physics_call_count = 0;

int total_physics_function_calls(const bool get)
{
    if (get) {
        return physics_call_count;
    } else {
        physics_call_count++;
        return -1;
    }
}

// Function for detecting coin collision
void coinsCollection() {
    // Collision detection for coins
    for (int x = 0; x < MAX_COINS_X; x++) {
        for (int y = 0; y < MAX_COINS_Y; y++) {
            if (coinCollected[x][y]) 
                continue; // Skip already collected coins

            int coinX = coinFixedX[x][y] - (int)gl.camera[0];
            int coinY = coinFixedY[x][y];

            // Collision threshold
            int collisionDist = 20;

            if (abs(gl.ball_pos[0] - coinX) < collisionDist && abs(lev.tile_base + gl.ball_pos[1] - coinY) < collisionDist) {
                coinCollected[x][y] = true;
                collectedCoins += 1; // Increment coin count
                playCollectSound();
            }
        }
    }
}

void playCollectSound() {
    // Load and play the "collect.wav" sound
    ALuint buffer, source;
    buffer = alutCreateBufferFromFile("./sounds/collect.wav");
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcePlay(source);
    
}

void playBackgroundMusic() {
    ALuint buffer, source;
    buffer = alutCreateBufferFromFile("./sounds/battle.wav");
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    // Loop the background music
    alSourcei(source, AL_LOOPING, AL_TRUE);
    alSourcePlay(source);
}


void display_coinsCollected() {
    coinsCollection();
    // Display collected coins at the top-left of the screen
    Rect r;
    r.bot = gl.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, 0xffff00, "Collected coins: %d", collectedCoins);
}

