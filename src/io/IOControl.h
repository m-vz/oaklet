//
// Created by Milan van Zanten on 09.02.18.
//

#ifndef BESTEST_GAME_IOCONTROL_H
#define BESTEST_GAME_IOCONTROL_H

#include <GLFW/glfw3.h>
#include "Mouse.h"

class IOControl {
public:
    Mouse mouse;

    IOControl(GLFWwindow *window);
    void processInput();
};

#endif //BESTEST_GAME_IOCONTROL_H
