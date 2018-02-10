//
// Created by Milan van Zanten on 09.02.18.
//

#include "IOControl.h"
#include "../util/Log.h"

IOControl::IOControl(GLFWwindow *window) {
    // Enable sticky keys
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

}

void IOControl::processInput() {
    Log::log << LogType::LOG_DEBUG << "processing input.";
}
