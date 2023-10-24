#ifndef _UDAI_S_
#define _UDAI_S_

extern void display_border(int xres, int yres);

void incrementRenderCount();

// Function to get the current render count
// If 'get' is true, it returns the render count; otherwise, it returns 0
int getRenderCount(const bool get);

// Function to display the render statistics
void displayRenderStats();

#endif

