//
// Created by Milan van Zanten on 09.02.18.
//

#include "IOControl.h"
#include "../util/Log.h"

IOControl::IOControl(GLFWwindow *window) {
    // prepare mouse and keyboard
    mouse = new Mouse();
    keyboard = new Keyboard();

    // set user pointer
    glfwSetWindowUserPointer(window, this);

    // start callbacks
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);
}

void IOControl::processInput() {
    Log::log << LogType::LOG_FRAME << "processing input.";
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

IOControl::~IOControl() {
    delete mouse;
    delete keyboard;
}
