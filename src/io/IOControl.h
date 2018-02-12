//
// Created by Milan van Zanten on 09.02.18.
//

#ifndef BESTEST_GAME_IOCONTROL_H
#define BESTEST_GAME_IOCONTROL_H

#include <GLFW/glfw3.h>
#include "Mouse.h"
#include "Keyboard.h"

class IOControl {
public:
    Mouse *mouse;
    Keyboard *keyboard;

    explicit IOControl(GLFWwindow *window);
    void processInput();
    virtual ~IOControl();

private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif //BESTEST_GAME_IOCONTROL_H
