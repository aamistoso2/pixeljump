//3350
//program: walk2.cpp
//author:  Gordon Griesel
//date:    summer 2017
//         spring 2018
//
//Walk cycle using a sprite sheet.
//images courtesy: http://games.ucla.edu/resource/walk-cycles/
//
//This program includes:
//  multiple sprite-sheet animations
//  a level tiling system
//  parallax scrolling of backgrounds
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
//#include "ppm.h"
#include "fonts.h"
#include "dlopez2.h"
#include "nchiang.h"
#include "aamistoso.h"
#include "usingh.h"
#include "ailarde.h"
#include "pixel.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

//COINS - ADAM ILARDE
#define MAX_COINS_X 6
#define MAX_COINS_Y 6

extern int collectedCoins;

bool coinCollected[MAX_COINS_X][MAX_COINS_Y] = {0};
int coinFixedX[MAX_COINS_X][MAX_COINS_Y] = {
    {600, 0, 900},
    {0, 1300, 1500},
    {0, 1900, 2000},

    //second set
    {0, 0, 2700},
    {0, 3000, 3500},
    {4000, 4500, 5500}

};

int coinFixedY[MAX_COINS_X][MAX_COINS_Y] = {
    {330, 0, 550},
    {0, 560, 400},
    {0, 500, 330},

    //second set
    {0, 0, 550},
    {0, 560, 450},
    {550, 400, 500}

};

//SPIKES - ADAM ILARDE
#define MAX_SPIKES_X 12 
#define MAX_SPIKES_Y 12

int spikesFixedX[MAX_SPIKES_X][MAX_SPIKES_Y] = {
    {800, 830, 0}, 
    {1070, 1100, 1130}, 
    {1500, 0, 0},
    //the top of the 3 mini hills
    //add 128 to the previous location
    {1807, 1935, 2063},
    {2191, 0, 0},
    //last of cycle
    {2620, 2650, 2680},

    //second cycle
    {3300, 3330, 0},
    {3570, 3600, 3630},
    //the bottom of the 3 mini hills
    {4368, 4496, 4624},
    //middle
    {4800, 4830, 4860},
    //last of cycle
    {5100, 5200, 5230},


      
};

int spikesFixedY[MAX_SPIKES_X][MAX_SPIKES_Y] = {
    {390, 390, 0}, 
    {451, 451, 451}, 
    {518, 0, 0},
    //top of the 3 mini hills
    {390, 390, 390},
    {390, 0, 0},
    //last of cycle
    {422, 422, 422},
    
    //second cycle
    {390, 390, 0},
    {451, 451, 451},
    //bottom of the 3 mini hills
    {325, 325, 325},
    //middle
    {325, 325, 325},
    //last of cycle
    {422, 422, 422}


};

//RESET COORDINATES
int resetBallPosX = 250;
int resetBallPosY = 300; 


//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define ALPHA 1;

//function prototypes
void initOpengl();
void checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void init();
void physics();
void render();
void play_game();

//-----------------------------------------------------------------------------
//Setup timers
class Timers {
    public:
        double physicsRate;
        double oobillion;
        double timeSpan = 0.0;
        double physicsCountdown = 0.0;
        struct timespec timeStart, timeEnd, timeCurrent;
        struct timespec walkTime;
        struct timespec timePause;
        Timers() {
            physicsRate = 1.0 / 60.0;
            oobillion = 1.0 / 1e9;
        }
        double timeDiff(struct timespec *start, struct timespec *end) {
            return (double)(end->tv_sec - start->tv_sec ) +
                (double)(end->tv_nsec - start->tv_nsec) * oobillion;
        }
        void timeCopy(struct timespec *dest, struct timespec *source) {
            memcpy(dest, source, sizeof(struct timespec));
        }
        void recordTime(struct timespec *t) {
            clock_gettime(CLOCK_REALTIME, t);
        }
} timers;
//-----------------------------------------------------------------------------

Sprite::Sprite() {
    onoff = 0;
    frame = 0;
    image = NULL;
    delay = 0.1;
}

Global::~Global() {
    logClose();
}

Global::Global() {
    logOpen();
    camera[0] = camera[1] = 0.0;
    //movie=0;
    //movieStep=2;
    xres=800;
    yres=600;
    walk=0;
    walkFrame=0;
    isJumping = 0;
    jumpFrame = 0;
    maxJumpFrames = 15;
    inAir = 0;
    secondJump = 1;
    ball_speed = 0.5f;
    dashing = 0;
    maxDashFrames = 5;
    dashFrame = 0;
    facing = 1;
    transitionTime = 2.0f; 
    walkImage=NULL;
    MakeVector(ball_pos, 520.0, 92, 0);
    MakeVector(ball_vel, 0, 0, 0);
    current_hp = 200.0f;
    delay = 0.02;
    show_name = 0;
    statistics = 0;
    show_credits = 0;
    max_hp = 200;
    for (int i=0; i<20; i++) {
        // box[i][0] = rnd() * xres;
        // box[i][1] = rnd() * (yres-220) + 220.0;
        // box[i][2] = 0.0;
        box[i][0] = xres;
        box[i][1] = (yres-220) + 220.0;
        box[i][2] = 0.0;
    }
    memset(keys, 0, 65536);
}

Global gl;

Level::Level() {
    //Log("Level constructor\n");
    tilesize[0] = 32;
    tilesize[1] = 32;
    ftsz[0] = (Flt)tilesize[0];
    ftsz[1] = (Flt)tilesize[1];
    tile_base = 220.0;
    //read level
    FILE *fpi = fopen("level1.txt","r");
    if (fpi) {
        nrows=0;
        char line[100];
        while (fgets(line, 100, fpi) != NULL) {
            removeCrLf(line);
            int slen = strlen(line);
            ncols = slen;
            //Log("line: %s\n", line);
                for (int j=0; j<slen; j++) {
                    arr[nrows][j] = line[j];
                }
            ++nrows;
        }
        fclose(fpi);
        //printf("nrows of background data: %i\n", nrows);
    }
    for (int i=0; i<nrows; i++) {
        for (int j=0; j<ncols; j++) {
            printf("%c", arr[i][j]);
        }
        printf("\n");
    }
}

void Level::removeCrLf(char *str) {
    //remove carriage return and linefeed from a Cstring
    char *p = str;
    while (*p) {
        if (*p == 10 || *p == 13) {
            *p = '\0';
            break;
        }
        ++p;
    }
}

Level lev;

//X Windows variables

X11_wrapper::~X11_wrapper() {
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}
void X11_wrapper::setTitle() {
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "PixelJump");
}
void X11_wrapper::setupScreenRes(const int w, const int h) {
    gl.xres = w;
    gl.yres = h;
}
X11_wrapper::X11_wrapper() {
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
    XSetWindowAttributes swa;
    setupScreenRes(gl.xres, gl.yres);
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        printf("\n\tcannot connect to X server\n\n");
        exit(EXIT_FAILURE);
    }
    Window root = DefaultRootWindow(dpy);
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if (vi == NULL) {
        printf("\n\tno appropriate visual found\n\n");
        exit(EXIT_FAILURE);
    } 
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
        PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease |
        StructureNotifyMask | SubstructureNotifyMask;
    win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
            vi->depth, InputOutput, vi->visual,
            CWColormap | CWEventMask, &swa);
    GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
    setTitle();
}
void X11_wrapper::reshapeWindow(int width, int height) {
    //window has been resized.
    setupScreenRes(width, height);
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
    setTitle();
}
void X11_wrapper::checkResize(XEvent *e) {
    //The ConfigureNotify is sent by the
    //server if the window is resized.
    if (e->type != ConfigureNotify)
        return;
    XConfigureEvent xce = e->xconfigure;
    if (xce.width != gl.xres || xce.height != gl.yres) {
        //Window size did change.
        reshapeWindow(xce.width, xce.height);
    }
}
bool X11_wrapper::getXPending() {
    return XPending(dpy);
}
XEvent X11_wrapper::getXNextEvent() {
    XEvent e;
    XNextEvent(dpy, &e);
    return e;
}
void X11_wrapper::swapBuffers() {
    glXSwapBuffers(dpy, win);
}
X11_wrapper x11;

Image::~Image() {
     delete [] data;
}

Image::Image(const char *fname) {
    if (fname[0] == '\0')
        return;
    //printf("fname **%s**\n", fname);
    int ppmFlag = 0;
    char name[40];
    strcpy(name, fname);
    int slen = strlen(name);
    char ppmname[80];
    if (strncmp(name+(slen-4), ".ppm", 4) == 0)
        ppmFlag = 1;
    if (ppmFlag) {
        strcpy(ppmname, name);
    } else {
        name[slen-4] = '\0';
        //printf("name **%s**\n", name);
        sprintf(ppmname,"%s.ppm", name);
        //printf("ppmname **%s**\n", ppmname);
        char ts[100];
        //system("convert eball.jpg eball.ppm");
        sprintf(ts, "convert %s %s", fname, ppmname);
        system(ts);
    }
    //sprintf(ts, "%s", name);
    //printf("read ppm **%s**\n", ppmname); fflush(stdout);
    FILE *fpi = fopen(ppmname, "r");
    if (fpi) {
        char line[200];
        fgets(line, 200, fpi);
        fgets(line, 200, fpi);
        //skip comments and blank lines
        while (line[0] == '#' || strlen(line) < 2)
            fgets(line, 200, fpi);
        sscanf(line, "%i %i", &width, &height);
        fgets(line, 200, fpi);
        //get pixel data
        int n = width * height * 3;			
        data = new unsigned char[n];			
        for (int i=0; i<n; i++)
            data[i] = fgetc(fpi);
        fclose(fpi);
    } else {
    printf("ERROR opening image: %s\n",ppmname);
    exit(0);
    }
    if (!ppmFlag)
    unlink(ppmname);
}

Image img[1] = {
    "./images/knight.png"
    };


int main(void)
{
    initOpengl();
    init();
    playBackgroundMusic(); // Background music from aamistoso.cpp
    display_menu(); // Calls from nchiang.cpp for a main menu

    cleanupAL();
    return 0;
}

unsigned char *buildAlphaData(Image *img)
{
    //add 4th component to RGB stream...
    int i;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = (unsigned char *)malloc(img->width * img->height * 4);
    ptr = newdata;
    unsigned char a,b,c;
    //use the first pixel in the image as the transparent color.
    unsigned char t0 = *(data+0);
    unsigned char t1 = *(data+1);
    unsigned char t2 = *(data+2);
    for (i=0; i<img->width * img->height * 3; i+=3) {
        a = *(data+0);
        b = *(data+1);
        c = *(data+2);
        *(ptr+0) = a;
        *(ptr+1) = b;
        *(ptr+2) = c;
        *(ptr+3) = 1;
        if (a==t0 && b==t1 && c==t2)
            *(ptr+3) = 0;
        //-----------------------------------------------
        ptr += 4;
        data += 3;
    }
    return newdata;
}

void initOpengl(void)
{
    //OpenGL initialization
    glViewport(0, 0, gl.xres, gl.yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //This sets 2D mode (no perspective)
    glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
    //
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    //
    //Clear the screen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    //Do this to allow fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
    //
    //load the images file into a ppm structure.
    //
    int w = img[0].width;
    int h = img[0].height;
    //
    //create opengl texture elements
    glGenTextures(1, &gl.knightTexture);
    //-------------------------------------------------------------------------
    //knight character
    //
    glBindTexture(GL_TEXTURE_2D, gl.knightTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *knightData = buildAlphaData(&img[0]);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, knightData);
    free(knightData);
    //-------------------------------------------------------------------------
    //create opengl texture elements
    //w = img[0].width;
    //h = img[0].height;
    //glGenTextures(1, &gl.exp.tex);
    //-------------------------------------------------------------------------
    //this is similar to a sprite graphic
    //glBindTexture(GL_TEXTURE_2D, gl.exp.tex);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //must build a new set of data...
    //unsigned char *xData = buildAlphaData(&img[1]);	
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
    //        GL_RGBA, GL_UNSIGNED_BYTE, xData);
    //free(xData);
    //-------------------------------------------------------------------------
    //w = img[2].width;
    //h = img[2].height;
    //create opengl texture elements
    //glGenTextures(1, &gl.exp44.tex);
    //-------------------------------------------------------------------------
    //this is similar to a sprite graphic
    //glBindTexture(GL_TEXTURE_2D, gl.exp44.tex);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //must build a new set of data...
    //xData = buildAlphaData(&img[2]);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
    //        GL_RGBA, GL_UNSIGNED_BYTE, xData);
    //free(xData);
}

void init() {
    alutInit(0, NULL);
}

void cleanupAL() {
    ALCcontext *Context = alcGetCurrentContext();
    ALCdevice *Device = alcGetContextsDevice(Context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(Context);
    alcCloseDevice(Device);
}

void checkMouse(XEvent *e)
{
    //printf("checkMouse()...\n"); fflush(stdout);
    //Did the mouse move?
    //Was a mouse button clicked?
    static int savex = 0;
    static int savey = 0;
    //
    if (e->type != ButtonRelease && e->type != ButtonPress &&
            e->type != MotionNotify)
        return;
    if (e->type == ButtonRelease) {
        return;
    }
    if (e->type == ButtonPress) {
        if (e->xbutton.button==1) {
            //Left button is down
        }
        if (e->xbutton.button==3) {
            //Right button is down
        }
    }
    if (e->type == MotionNotify) {
        if (savex != e->xbutton.x || savey != e->xbutton.y) {
            //Mouse moved
            savex = e->xbutton.x;
            savey = e->xbutton.y;
            mouse_movement_distance(e->xbutton.x, e->xbutton.y, false);
            get_last_mouse_movement(false);
        }
    }
}

//Added Play Game - Nicklas Chiang
void play_game()
{
    int done = 0;
    clock_gettime(CLOCK_REALTIME, &timers.timePause);
    clock_gettime(CLOCK_REALTIME, &timers.timeStart);
    while (!done) {
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.checkResize(&e);
            checkMouse(&e);
            done = checkKeys(&e);
        }
        clock_gettime(CLOCK_REALTIME, &timers.timeCurrent);
        timers.timeSpan = timers.timeDiff(&timers.timeStart, &timers.timeCurrent);
        timers.timeCopy(&timers.timeStart, &timers.timeCurrent);
        timers.physicsCountdown += timers.timeSpan;
        while (timers.physicsCountdown >= timers.physicsRate) {
            physics();
            timers.physicsCountdown -= timers.physicsRate;
        }
        //physics();
        render();
        x11.swapBuffers();
    }
    cleanup_fonts();
}

void screenCapture()
{
    static int fnum = 0;
    static int vid = 0;
    if (!vid) {
        system("mkdir ./vid");
        vid = 1;
    }
    unsigned char *data = (unsigned char *)malloc(gl.xres * gl.yres * 3);
    glReadPixels(0, 0, gl.xres, gl.yres, GL_RGB, GL_UNSIGNED_BYTE, data);
    char ts[32];
    sprintf(ts, "./vid/pic%03i.ppm", fnum);
    FILE *fpo = fopen(ts,"w");	
    if (fpo) {
        fprintf(fpo, "P6\n%i %i\n255\n", gl.xres, gl.yres);
        unsigned char *p = data;
        //go backwards a row at a time...
        p = p + ((gl.yres-1) * gl.xres * 3);
        unsigned char *start = p;
        for (int i=0; i<gl.yres; i++) {
            for (int j=0; j<gl.xres*3; j++) {
                fprintf(fpo, "%c",*p);
                ++p;
            }
            start = start - (gl.xres*3);
            p = start;
        }
        fclose(fpo);
        char s[256];
        sprintf(s, "convert ./vid/pic%03i.ppm ./vid/pic%03i.gif", fnum, fnum);
        system(s);
        unlink(ts);
    }
    ++fnum;
}

int checkKeys(XEvent *e)
{
    //keyboard input?
    static int shift=0;
    static bool isShiftHeld = false;
    if (e->type != KeyPress && e->type != KeyRelease)
        //time_key_press(false);
        return 0;
    int key = XLookupKeysym(&e->xkey, 0);
    gl.keys[key]=1;

    if (e->type == KeyRelease) {
        gl.keys[key] = 0;
        if (key == XK_Shift_L || key == XK_Shift_R) {
            shift = 0;
            isShiftHeld = false;
        }
        return 0;
    }
    gl.keys[key] = 1;
    if (key == XK_Shift_L || key == XK_Shift_R) {
        shift = 1;
        isShiftHeld = true;
        return 0;
    }
    //-Sprint Functionality WORKING - Nicklas Chiang----
    if (isShiftHeld) {
        gl.ball_speed = 0.8f;
    } else {
        gl.ball_speed = 0.5f;
    }
    //--------------------------------------------------
    (void)shift;
    //-Jump Functionality Working!-------------------
    //gl.keys[key]=1;
    if (key == XK_space && !gl.isJumping && gl.secondJump) {
        if (gl.inAir)
            gl.secondJump = 0;
        jumping();
        playJumpSound();
        //printf("%d\n", gl.isJumping);
    }
    //--------------------------------------------------
    switch (key) {
        case XK_C:
        case XK_c:
            gl.show_credits ^= 1;
            break;
        case XK_R:
        case XK_r:
            resetGame();
            gl.ball_pos[0] = resetBallPosX;
            gl.ball_pos[1] = resetBallPosY;
            gl.current_hp = 200;
            break;
        case XK_s:
            //screenCapture();
            gl.statistics ^= 1;
            break;
        case XK_g:
            gl.show_name ^= 1;
            break;
        case XK_m:
            gl.movie ^= 1;
            break;
        case XK_w:
            timers.recordTime(&timers.walkTime);
            gl.walk ^= 1;
            break;
        case XK_e:
            // gl.exp.pos[0] = 200.0;
            // gl.exp.pos[1] = -60.0;
            // gl.exp.pos[2] =   0.0;
            // timers.recordTime(&gl.exp.time);
            // gl.exp.onoff ^= 1;
            dash();
            break;
        case XK_f:
            gl.exp44.pos[0] = 200.0;
            gl.exp44.pos[1] = -60.0;
            gl.exp44.pos[2] =   0.0;
            timers.recordTime(&gl.exp44.time);
            gl.exp44.onoff ^= 1;
            break;
        case XK_Left:
            break;
        case XK_Right:
            break;
        case XK_Up:
            break;
        case XK_Down:
            break;
            //	case XK_equal:
            //		gl.delay -= 0.005;
            //		if (gl.delay < 0.005)
            //			gl.delay = 0.005;
            //		break;
            //	case XK_minus:
            //		gl.delay += 0.005;
            //		break;
        case XK_Escape:
            return 1;
            break;
    }
    return 0;
}

Flt VecNormalize(Vec vec)
{
    Flt len, tlen;
    Flt xlen = vec[0];
    Flt ylen = vec[1];
    Flt zlen = vec[2];
    len = xlen*xlen + ylen*ylen + zlen*zlen;
    if (len == 0.0) {
        MakeVector(vec, 0.0, 0.0, 1.0);
        return 1.0;
    }
    len = sqrt(len);
    tlen = 1.0 / len;
    vec[0] = xlen * tlen;
    vec[1] = ylen * tlen;
    vec[2] = zlen * tlen;
    return(len);
}


void physics(void)
{
    total_physics_function_calls(false);

    Flt dd = lev.ftsz[0];                                     // x width for each block
    int col = (int)((gl.camera[0] + gl.ball_pos[0]) / dd);    // finds the column that the block is on
    int row = (int)((gl.camera[1] + gl.ball_pos[1]) / dd);    // finds the row that the block is on
    col = col % lev.ncols;
    //row = row % (lev.nrows + 1);

    float rightWall = findRightWall(col, row);
    float leftWall = findLeftWall(col, row);

    //printf("%f %f\n", leftWall, rightWall);

    // if (gl.walk || gl.keys[XK_Right] || gl.keys[XK_Left] || gl.keys[XK_a] || gl.keys[XK_d]) {
    //     for ( int i=0; i<20; i++ ) {
    //         if ( (gl.keys[XK_Left] || gl.keys[XK_a]) && leftWall < -32.0f ) {
    //             gl.facing = 0;
    //             gl.box[i][0] += 1.0 * ( 0.1 / gl.delay );
    //             if ( gl.box[i][0] > gl.xres + 10.0 )
    //                 gl.box[i][0] -= gl.xres + 10.0;
    //             gl.camera[0] -= 2.0/lev.tilesize[0] * ( 0.1 / gl.delay );
    //             if ( gl.camera[0] < 0.0 )
    //                 gl.camera[0] = 0.0;
    //         } else if ( (gl.keys[XK_Right] || gl.keys[XK_d]) && rightWall > 32.0f ) {
    //             gl.facing = 1;
    //             gl.box[i][0] -= 1.0 * (0.1 / gl.delay);
    //             if ( gl.box[i][0] < -10.0 )
    //                 gl.box[i][0] += gl.xres + 10.0;
    //             gl.camera[0] += 2.0/lev.tilesize[0] * ( 0.1 / gl.delay );
    //             if ( gl.camera[0] < 0.0 )
    //                 gl.camera[0] = 0.0;
    //         }
    //     }
    // }

    updateDash();
    if (gl.keys[XK_Right] || gl.keys[XK_Left] || gl.keys[XK_a] || gl.keys[XK_d]) {
        //updateDash();
        for ( int i=0; i<20; i++ ) {
            if ( (gl.keys[XK_Left] || gl.keys[XK_a]) && leftWall < -32.0f ) {
                gl.facing = 0;
                gl.ball_vel[0] = -gl.ball_speed;
                gl.box[i][0] += gl.ball_vel[0];
                // if ( gl.box[i][0] > gl.xres + 10.0 )
                //     gl.box[i][0] -= gl.xres + 10.0;
                gl.camera[0] += gl.ball_vel[0];
                // if ( gl.camera[0] < 0.0 )
                //     gl.camera[0] = 0.0;
            } else if ( (gl.keys[XK_Right] || gl.keys[XK_d]) && rightWall > 32.0f ) {
                gl.facing = 1;
                gl.ball_vel[0] = gl.ball_speed;
                gl.box[i][0] += gl.ball_vel[0];
                // if ( gl.box[i][0] < -10.0 )
                //     gl.box[i][0] += gl.xres + 10.0;
                gl.camera[0] += gl.ball_vel[0];
                // if ( gl.camera[0] < 0.0 )
                //     gl.camera[0] = 0.0;
            }
        }
    }

    //printf("%f\n", gl.camera[0]);

    //Jump Functionality - Nicklas Chiang
    for ( int i=0; i<20; i++ ) {
        if ( gl.isJumping ) {
            updateJump();
        }
    }
    gl.keys[XK_space] = 0;

    int floor = findFloor(col, row);
    int cieling = findCeiling(col, row);
    // jumping
    if (((gl.ball_pos[1] + 24.0f) > cieling) && (gl.ball_vel[1] >= 0.0f)) {
        gl.ball_vel[1] = 0.0f;
    }
    // falling
    if (((gl.ball_pos[1] - 10.0f) < floor) && (gl.ball_vel[1] <= 0.0f)) {
        gl.ball_vel[1] = 0.0f;
    } else {
        if (gl.ball_vel[1] > -9.0f)
            gl.ball_vel[1] -= 0.9f;
    }
    // update y position with current y velocity
    gl.ball_pos[1] += gl.ball_vel[1];
    if (gl.ball_pos[1] < -256.0f) {
        gl.current_hp = 0.0f;
    }

    // one air jump
    gl.inAir = checkInAir(floor);
    if(!gl.inAir)
        gl.secondJump = 1;

    // Call from aamistoso.cpp for coins collection functionality
    coinsCollection();
    
}
// Udai Singh
// Changed background to a Starry Background
// Render Counter Function

void render(void) {
      incrementRenderCount();
      Rect r;

      // Set the midnight sky color
      float spaceR = 0.1f; // Dark space color
      float spaceG = 0.1f;
      float spaceB = 0.2f;
      glClearColor(spaceR, spaceG, spaceB, 1.0); // Set the background colo     r to a dark blue
      glClear(GL_COLOR_BUFFER_BIT);

      // Initialize random seed
      srand(time(NULL));

      // Draw the stars
     int numStars = 100; // Number of stars you want
      glColor3f(1.0, 1.0, 1.0); // Set the star color to white
      glPointSize(2.0); // Set the size of the stars
      glBegin(GL_POINTS);
      for (int i = 0; i < numStars; i++) {
          float x = static_cast<float>(rand() % gl.xres);
          float y = static_cast<float>(rand() % gl.yres);
          glVertex2f(x, y);
      }
      glEnd();

      // Restore the default color (if other drawing operations follow)
      glColor3f(1.0, 1.0, 1.0);

    //float cx = gl.xres/2.0;
    //float cy = gl.yres/2.0;   

    //========================
    //Render the tile system
    //========================
    int tx = lev.tilesize[0];
    int ty = lev.tilesize[1];
    Flt dd = lev.ftsz[0];
    Flt offy = lev.tile_base;
    int ncols_to_render = gl.xres / lev.tilesize[0] + 2;
    int col = (int)(gl.camera[0] / dd);
    
    col = col % lev.ncols;
    //Partial tile offset must be determined here.
    //The leftmost tile might be partially off-screen.
    //cdd: camera position in terms of tiles.
    Flt cdd = gl.camera[0] / dd;
    //flo: just the integer portion
    Flt flo = floor(cdd);
    //dec: just the decimal portion
    Flt dec = (cdd - flo);
    //offx: the offset to the left of the screen to start drawing tiles
    Flt offx = -dec * dd;
    //Log("gl.camera[0]: %lf   offx: %lf\n",gl.camera[0],offx);
    //
    
    for (int j=0; j<ncols_to_render; j++) {
        int row = lev.nrows-1;
        for (int i=0; i<lev.nrows; i++) {

            //SHOWING WHITE COINS - ADAM ILARDE
            //int tileCenterX = j * tx + offx + tx / 2;
            //int tileCenterY = i * lev.ftsz[1] + offy + ty / 2;
            
            //int circleX; = 600;
            //int circleY; = 300;

            // Draw a circle at the center of the tile
            //showCoins(circleX, circleY);
            //showCoins(tileCenterX, tileCenterY);

            if (lev.arr[row][col] == 'w') {
                glColor3f(0.8, 0.8, 0.6);
                glPushMatrix();
                //put tile in its place
                glTranslated((Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0);
                glBegin(GL_QUADS);
                glVertex2i( 0,  0);
                glVertex2i( 0, ty);
                glVertex2i(tx, ty);
                glVertex2i(tx,  0);
                glEnd();
                glPopMatrix();
            }
            if (lev.arr[row][col] == 'b') {
                glColor3f(0.9, 0.2, 0.2);
                glPushMatrix();
                glTranslated((Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0);
                glBegin(GL_QUADS);
                glVertex2i( 0,  0);
                glVertex2i( 0, ty);
                glVertex2i(tx, ty);
                glVertex2i(tx,  0);
                glEnd();
                glPopMatrix();
            }
            --row;

        }

        col = (col+1) % lev.ncols;
    }

    //COINS - ADAM ILARDE
    for (int x = 0; x < MAX_COINS_X; x++) {
        for (int y = 0; y < MAX_COINS_Y; y++) {
            // Calculate new coordinates for each coin based on camera movement
            if (coinCollected[x][y]) {
                continue; // Don't render collected coins
            }
            int coinX = coinFixedX[x][y] - (int)gl.camera[0];
            int coinY = coinFixedY[x][y];

            // Display the coin at the updated position
            showCoins(coinX, coinY);
        }
    }

    //SPIKES - ADAM ILARDE
    // Display spikes at different fixed positions on the terrain
    for (int x = 0; x < MAX_SPIKES_X; x++) {
        for (int y = 0; y < MAX_SPIKES_Y; y++) {
            // Calculate new coordinates for each spike based on camera movement
            int spikeX = spikesFixedX[x][y] - (int)gl.camera[0];
            int spikeY = spikesFixedY[x][y];

            //showSpikes(spikeX, spikeY, 10.0);

            // Check if the player is colliding with spikes
            if (spikesDetection(gl.ball_pos[0], lev.tile_base + gl.ball_pos[1], spikeX, spikeY, 10.0)) {
                // Spike collision logic
                //gl.current_hp -= 25;

                //if (gl.current_hp > 0) {
                //    gl.current_hp -= 1;
                //}
                if (!damageImmune()) {
                    float decrease = 25;
                    gl.current_hp -= decrease;

                    if (gl.current_hp <= 0) {
                        //gl.current_hp = 0;
                        display_game_over();
                    }
                }
            } else {
                // Display the spike at the updated position
                showSpikes(spikeX, spikeY, 10.0);             }
        }
    }


    //Ball ------------
    glColor3f(1.0, 1.0, 0.1);
    glPushMatrix();
    //put ball in its place
    glTranslated(gl.ball_pos[0], lev.tile_base+gl.ball_pos[1], 0);
    glBindTexture(GL_TEXTURE_2D, gl.knightTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f);  glVertex2i(-10, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-10, 20);
        glTexCoord2f(1.0f, 0.0f); glVertex2i( 10, 20);
        glTexCoord2f(1.0f, 1.0f); glVertex2i( 10, 0);
    glEnd();
    glPopMatrix();

    //--------------------------------------
    //float h = 200.0;
    //float w = h * 0.5;
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    //glBindTexture(GL_TEXTURE_2D, gl.walkTexture);
    //
 
    //--------------------------------------------------------------
    //unsigned int c = 0x00ffff44;
    r.bot = gl.yres - 20;
    r.left = 10;
    r.center = 0;

    if (gl.show_name) {
        display_border(gl.xres, gl.yres);
    }

    //Calls from nchiang.cpp to display credits
    if (gl.show_credits) {
        display_credits();
    }

    if (gl.movie) {
        screenCapture();
    }

    if (gl.statistics) {
        glColor3ub(100, 100, 100);
        glPushMatrix();
        glTranslatef(20.0, 20.0, 0.0);
        int w = 230;
        int h = 130;
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(0, h);
        glVertex2f(0, h);
        glVertex2f(w, 0);
        glVertex2f(w, 0);
        glEnd();
        glPopMatrix();
        r.bot = 124;
        r.left = 28;
        r.center = 0;
        ggprint13(&r, 20, 0x0055ff55, "Statistics...");
        ggprint13(&r, 16, 0x00ffff00, "sec running time: %i",
                total_running_time(true));
        ggprint13(&r, 16, 0x00ffff00, "sec since mouse move: %i",
                get_last_mouse_movement(true));
        ggprint13(&r, 16, 0x00ffff00, "n render calls: %i",
                getRenderCount(true));
        ggprint13(&r, 16, 0x00ffff00, "n physics calls: %i",
                total_physics_function_calls(true));
        ggprint13(&r, 16, 0x00ffff00, "mouse distance: %lf",
                mouse_movement_distance(0, 0, true));
        ggprint13(&r, 16, 0x00ffff00, "sec since key pressed: %i",
                 time_key_press(true));
    }
    // Call from aamistoso.cpp to display coins collected on screen
    display_coinsCollected();
    //Calls from nchiang.cpp to display hp on screen
    //gl.current_hp = 100;
    display_hp(gl.current_hp, gl.max_hp); 

}
