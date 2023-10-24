#include "usingh.h"
#include <iostream>

static int renderCount = 0;

void incrementRenderCount() {
    renderCount++;
}

int getRenderCount(const bool get) {
    if (get) {
        return renderCount;
    } else {
        return 0;
    }
}

void displayRenderStats() {
    std::cout << "Render Function Calls: " << getRenderCount(true) << std::endl;
}
