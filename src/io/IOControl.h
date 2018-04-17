//
// Created by Milan van Zanten on 09.02.18.
//

#ifndef BESTEST_GAME_IOCONTROL_H
#define BESTEST_GAME_IOCONTROL_H

#include <GLFW/glfw3.h>
#include "Mouse.h"
#include "Keyboard.h"
#include "Window.h"
#include "../world/Time.h"
#include "../graphics/camera/FreeCamera.h"

class IOControl {
public:
    Mouse *mouse;
    Keyboard *keyboard;
    Window *window;

    explicit IOControl(GLFWwindow *window);
    void processInput();
    int getMonitorCount() const;
    GLFWmonitor *getPrimaryMonitor();
    GLFWmonitor **getMonitors();
    virtual ~IOControl();

private:
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    static void windowSizeCallback(GLFWwindow *window, int width, int height);
    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    static void windowFocusCallback(GLFWwindow *window, int focus);
};

#endif //BESTEST_GAME_IOCONTROL_H
