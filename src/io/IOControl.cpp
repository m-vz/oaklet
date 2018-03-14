//
// Created by Milan van Zanten on 09.02.18.
//

#include "IOControl.h"
#include "../util/Log.h"

IOControl::IOControl(GLFWwindow *window) {
    // prepare mouse, keyboard and window
    IOControl::mouse = new Mouse();
    IOControl::keyboard = new Keyboard();
    IOControl::window = new Window(window);

    // set user pointer
    glfwSetWindowUserPointer(window, this);

    // start callbacks
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetWindowFocusCallback(window, windowFocusCallback);
}

void IOControl::processInput() {
    Log::log << LogType::LOG_FRAME << "processing input.";

    camera->update(time->timeSinceEpoch());
}

int IOControl::getMonitorCount() const {
    int count;
    glfwGetMonitors(&count);
    return count;
}

GLFWmonitor *IOControl::getPrimaryMonitor() {
    return glfwGetPrimaryMonitor();
}

GLFWmonitor **IOControl::getMonitors() {
    int count;
    return glfwGetMonitors(&count);
}

void IOControl::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Keyboard *keyboard = ((IOControl*) glfwGetWindowUserPointer(window))->keyboard;

    if(action == GLFW_PRESS)
        keyboard->keyPressed(key, scancode, mods);
    else if(action == GLFW_REPEAT)
        keyboard->keyRepeated(key, scancode, mods);
    else if(action == GLFW_RELEASE)
        keyboard->keyReleased(key, scancode, mods);
}

void IOControl::cursorPositionCallback(GLFWwindow *window, double xpos, double ypos) {
    Mouse *mouse = ((IOControl*) glfwGetWindowUserPointer(window))->mouse;

    mouse->movedTo(xpos, ypos);
}

void IOControl::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    Mouse *mouse = ((IOControl*) glfwGetWindowUserPointer(window))->mouse;

    if(action == GLFW_PRESS)
        mouse->down(button);
    else if(action == GLFW_RELEASE)
        mouse->up(button);
}

void IOControl::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    Mouse *mouse = ((IOControl*) glfwGetWindowUserPointer(window))->mouse;

    mouse->scrolled(xoffset, yoffset);
}

void IOControl::windowSizeCallback(GLFWwindow *window, int width, int height) {
    Window *w = ((IOControl*) glfwGetWindowUserPointer(window))->window;

    w->windowSizeChanged(width, height);
}

void IOControl::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    Window *w = ((IOControl*) glfwGetWindowUserPointer(window))->window;

    w->framebufferSizeChanged(width, height);
}

void IOControl::windowFocusCallback(GLFWwindow *window, int focus) {
    Window *w = ((IOControl*) glfwGetWindowUserPointer(window))->window;

    w->focusChanged(static_cast<bool>(focus));
}

IOControl::~IOControl() {
    delete mouse;
    delete keyboard;
    delete window;
}
