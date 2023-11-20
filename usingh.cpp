#include "usingh.h"
#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <GL/gl.h>
#include <fstream>
#include <sstream>

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

void displayExtendedRenderStats() {
    // Extended render statistics
    std::cout << "Render Function Calls: " << getRenderCount(true) << std::endl;
    // Additional stats can be added here
}

void changeBackgroundColor(float r, float g, float b) {
    // Set the clear color (background color) for OpenGL
    glClearColor(r, g, b, 1.0f); // Alpha is set to 1.0 for opaque
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer with the clear color
}

void logMessage(const std::string& message) {
    // Logging messages with timestamp
    std::cout << "[LOG " << time(nullptr) << "]: " << message << std::endl;
}

void logError(const std::string& error) {
    // Logging errors with timestamp
    std::cout << "[ERROR " << time(nullptr) << "]: " << error << std::endl;
}

void readConfig(const std::string& configFile) {
    // Code to read configuration settings
    std::cout << "Reading configuration from " << configFile << std::endl;
    // Implement reading the configuration file here
}

void applyConfig() {
    // Code to apply configuration settings
    std::cout << "Applying configuration settings." << std::endl;
    // Implement applying the configuration settings here
}

bool readConfiguration(const std::string& configFile, std::string& settings) {
    std::ifstream file(configFile);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        settings = buffer.str();
        file.close();
        return true;
    }
    return false;
}

void writeConfiguration(const std::string& configFile, const std::string& settings) {
    std::ofstream file(configFile);
    if (file.is_open()) {
        file << settings;
        file.close();
    }
}

void updateRenderStats() {
    incrementRenderCount();
    displayRenderStats();
    displayExtendedRenderStats();
}

