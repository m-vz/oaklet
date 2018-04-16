//
// Created by Milan van Zanten on 09.02.18.
//

#include "Mouse.h"
#include "../util/Log.h"

void Mouse::movedTo(MousePosition position) {
    if(firstMovement) {
        mousePositionOld = position;
        firstMovement = false;
    }
    if(captured) {
        mousePosition = position - mousePositionOld;
        mousePositionOld = position;
    } else
        mousePosition = position;

    Log::log << LogType::LOG_DEBUG << "Mouse moved to: (" << mousePosition.x << ", " << mousePosition.y << ")";

    if(!moveCallbacks.empty())
        for(const auto &callback: moveCallbacks)
            callback(*this);
}

void Mouse::movedTo(double x, double y) {
    movedTo({x, y});
}

void Mouse::scrolled(double xOffset, double yOffset) {
    Log::log << LogType::LOG_DEBUG << "Mouse scrolled: (" << xOffset << ", " << yOffset << ")";

    if(!scrollCallbacks.empty())
        for(const auto &callback: scrollCallbacks)
            callback(*this, xOffset, yOffset);
}

void Mouse::down(int button) {
    Log::log << LogType::LOG_DEBUG << "Mouse button down: " << button;

    mouseDown[button] = true;

    if(!downCallbacks[button].empty())
        for(const auto &callback: downCallbacks[button])
            callback(*this, button);
}

void Mouse::up(int button) {
    Log::log << LogType::LOG_DEBUG << "Mouse button up: " << button;

    mouseDown[button] = false;

    if(!upCallbacks[button].empty())
        for(const auto &callback: upCallbacks[button])
            callback(*this, button);
}

void Mouse::capture(Window &window) {
    if(captured) // if the mouse is already captured (maybe in another window), free it first
        free();

    glfwSetInputMode(window.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    capturedIn = &window;
    captured = true;
    firstMovement = true;
}

void Mouse::free() {
    if(!captured)
        Log::log << LOG_WARNING << "Trying to free mouse that is not captured.";

    glfwSetInputMode(capturedIn->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    capturedIn = nullptr;
    captured = false;
}

MousePosition Mouse::position() {
    return mousePosition;
}

bool Mouse::isDown(int button) {
    return mouseDown[button];
}

bool Mouse::isUp(int button) {
    return !mouseDown[button];
}

bool Mouse::isCaptured() {
    return captured;
}

void Mouse::addMoveCallback(std::function<void(Mouse&)> callback) {
    moveCallbacks.push_back(callback);
}

void Mouse::addScrollCallback(std::function<void(Mouse&, double, double)> callback) {
    scrollCallbacks.push_back(callback);
}

void Mouse::addDownCallback(int button, std::function<void(Mouse&, int)> callback) {
    downCallbacks[button].push_back(callback);
}

void Mouse::addUpCallback(int button, std::function<void(Mouse&, int)> callback) {
    upCallbacks[button].push_back(callback);
}
