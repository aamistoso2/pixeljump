// Source file for Daniel Lopez
#include <GL/glx.h>
#include "fonts.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "pixel.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <time.h>

extern Global gl;
extern Level lev;

void display_border(int xres, int yres) 
{
	float b = 50.0f;
	glColor3f(0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(b, b);
		glVertex2f(0.0f, yres);
		glVertex2f(b, yres - b);
		glVertex2f(xres, yres);
		glVertex2f(xres - b, yres - b);
		glVertex2f(xres, 0.0f);
		glVertex2f(xres - b, b);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(b, b);
	glEnd();
	glPopMatrix();
}

float mouse_movement_distance(int x, int y, bool get) {
	static float distance;
	static int first_time = 1;
	static int savex, savey;

	if (first_time) {
		distance  = 0;
		savex = x;
		savey = y;
		first_time = 0;
	}

	if (!get) {
		int dx = x - savex;
		int dy = y - savey;
		float new_distance = sqrt((dx * dx) + (dy * dy));
		distance += new_distance;
		savex = x;
		savey = y;
	}

	if (get) {
		return distance;
	}

	return 0;
}

int checkInAir(int floor) {
	int inAir = 0;
	int heightAboveFloor = gl.ball_pos[1] - floor;
	if (heightAboveFloor > 24) {
		inAir = 1;
	}
	return inAir;
}

void playJumpSound() {
    ALuint buffer1, source1;
    buffer1 = alutCreateBufferFromFile("./sounds/jump.wav");
    alGenSources(1, &source1);
	alSourcef(source1, AL_GAIN, 0.05f);
    alSourcei(source1, AL_BUFFER, buffer1);
    alSourcePlay(source1);
}

void playDashSound() {
    ALuint buffer2, source2;
    buffer2 = alutCreateBufferFromFile("./sounds/dash.wav");
    alGenSources(1, &source2);
	alSourcef(source2, AL_GAIN, 0.5f);
    alSourcei(source2, AL_BUFFER, buffer2);
    alSourcePlay(source2);
}

void playOwSound() {
    ALuint buffer, source;
    buffer = alutCreateBufferFromFile("./sounds/ow.wav");
    alGenSources(1, &source);
	alSourcef(source, AL_GAIN, 0.5f);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcePlay(source);
}

void dash() {
	// dash right
	static time_t lastTimeDashed = 0;
	time_t currentTime = time(NULL);
	time_t timeDiff = currentTime - lastTimeDashed;
	if (!gl.dashing && timeDiff >= 1) {
		playDashSound();
		gl.dashing = 1;
		gl.ball_speed = 2.0f;
		lastTimeDashed = currentTime;
	}
}

void updateDash() {
	if (gl.dashing) {
		if (gl.dashFrame < gl.maxDashFrames) {
			gl.dashFrame++;
			gl.ball_speed = 2.0f;
			gl.ball_vel[1] = 0.0f;
		} 
		else {
			gl.dashFrame = 0;
			gl.dashing = 0;
			gl.ball_speed = 0.5;
		}
	}
}

int findFloor(int col, int row) {
	int floor = -999999;
	for (int i = lev.nrows - row; i < lev.nrows ; i++) {
        if (lev.arr[i][col] == 'b' || lev.arr[i][col] == 'w') {
            floor = ((lev.nrows - i) * lev.tilesize[1]);
            break;
        }
    }
	return floor;
}

int findCeiling(int col, int row) {
	int ceiling = 999999;
	for (int i = lev.nrows - row - 1; i >= 0 ; i--) {
        if (lev.arr[i][col] == 'b' || lev.arr[i][col] == 'w') {
            ceiling = (lev.nrows - i - 1) * lev.tilesize[1];
            break;
        }
    }
	return ceiling;
}

float findRightWall(int col, int row) {
	float rightWall = 0;
	int arrRow = (lev.nrows - row) - 1;
	for (int i = col; i < lev.ncols; i++) {
        if (lev.arr[arrRow][i] == 'b' || lev.arr[arrRow][i] == 'w' 
									  || lev.arr[arrRow][i] == 'e') {
            break;
        }
        rightWall = rightWall + 1.0f;
    }
    return rightWall = (rightWall * lev.tilesize[0]);
}

float findLeftWall(int col, int row) {
	float leftWall = 0;
	int arrRow = (lev.nrows - row) - 1;
	for (int j = col; j >= 0; j--) {
        if (lev.arr[arrRow][j] == 'b' || lev.arr[arrRow][j] == 'w'
									  || lev.arr[arrRow][j] == 'e') {
            break;
        }
        leftWall = leftWall - 1.0f;
    }
    return leftWall = (leftWall * lev.tilesize[0]);
}

float findTileOffset(int col) {
	//int arrRow = (lev.nrows - row) - 1;
	float colPos = (col - 9) * lev.ftsz[0];
	float tileOffset = gl.camera[0] - colPos; // / lev.ftsz[0];
	return tileOffset;
}

int damageImmune() {
	static time_t lastTimeDamaged = 0;
	time_t current_time = time(NULL);
	if (current_time - lastTimeDamaged > 1) {
		lastTimeDamaged = current_time;
		return 0;
	}
	else {
		return 1;
	}
}

void updateYVelocity(int ceiling, int floor) {
	// jumping
    if (((gl.ball_pos[1] + 24.0f) > ceiling) && (gl.ball_vel[1] >= 0.0f)) {
        gl.ball_vel[1] = 0.0f;
    }
    // falling
    if (((gl.ball_pos[1] - 10.0f) < floor) && (gl.ball_vel[1] <= 0.0f)) {
        gl.ball_vel[1] = 0.0f;
    } else {
        if (gl.ball_vel[1] > -9.0f)
            gl.ball_vel[1] -= 0.9f;
    }
}

void printControls() {
	printf("A or D to walk left or right\n");
	printf("W to dash\n");
	printf("SPACE to jump\n");
	printf("Press SPACE again while in the air to double jump\n");
	printf("HOLD LEFT SHIFT to sprint\n");
	printf("R to reset\n");
	//fflush(stdout);
}