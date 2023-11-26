#include "usingh.h"
#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <GL/gl.h>
#include <fstream>
#include <sstream>
#include <map>

static int renderCount = 0;
static std::map<std::string, std::string> configSettings;
static float lastFrameTime = 0.0f;
static float fps = 0.0f;

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

void changeBackgroundColor(float r, float g, float b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void logMessage(const std::string& message) {
    std::cout << "[LOG " << time(nullptr) << "]: " << message << std::endl;
}

void logError(const std::string& error) {
    std::cout << "[ERROR " << time(nullptr) << "]: " << error << std::endl;
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

void parseConfig(const std::string& configFile) {
    std::string line, key, value;
    std::ifstream file(configFile);
    if (file.is_open()) {
        while (getline(file, line)) {
            std::stringstream ss(line);
            getline(ss, key, '=');
            getline(ss, value);
            configSettings[key] = value;
        }
        file.close();
    }
}

std::string getConfigValue(const std::string& key) {
    if (configSettings.find(key) != configSettings.end()) {
        return configSettings[key];
    }
    return "";
}

void applyConfig() {
    std::string bgColor = getConfigValue("backgroundColor");
    if (!bgColor.empty()) {
        float r, g, b;
        std::stringstream ss(bgColor);
        ss >> r >> g >> b;
        changeBackgroundColor(r, g, b);
    }
}

void updateRenderStats() {
    incrementRenderCount();
    displayRenderStats();
}

void checkForErrors() {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
}

void calculateFPS() {
    static int frameCount = 0;
    static float timeAccumulator = 0.0f;
    float currentTime = static_cast<float>(clock()) / CLOCKS_PER_SEC;
    float deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    frameCount++;
    timeAccumulator += deltaTime;

    if (timeAccumulator >= 1.0f) {
        fps = static_cast<float>(frameCount) / timeAccumulator;
        frameCount = 0;
        timeAccumulator -= 1.0f;
    }
}

float getFPS() {
    return fps;
}

void displayFrameTime() {
    std::cout << "Frame Time: " << lastFrameTime << " seconds" << std::endl;
}

void startFrame() {
    calculateFPS();
}

void endFrame() {
    displayFrameTime();
    checkForErrors();
}

void initializeOpenGLSettings() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void resizeWindow(int width, int height) {
    glViewport(0, 0, width, height);
}

