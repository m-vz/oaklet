//
// Created by Milan van Zanten on 09.02.18.
//

#include "Mouse.h"
#include "../util/Log.h"

void Mouse::movedTo(MousePosition position) {
    Log::log << LogType::LOG_DEBUG << "Mouse moved to: (" << position.x << ", " << position.y << ")";

    mousePosition = position;

    if(!moveCallbacks.empty())
        for(auto callback: moveCallbacks)
            callback(*this);
}

void Mouse::movedTo(int x, int y) {
    movedTo({x, y});
}

void Mouse::scrolled(int wheelDelta) {
    Log::log << LogType::LOG_DEBUG << "Mouse scrolled:" << wheelDelta;

    if(!scrollCallbacks.empty())
        for(auto callback: scrollCallbacks)
            callback(*this, wheelDelta);
}

void Mouse::down(MouseButton button) {
    Log::log << LogType::LOG_DEBUG << "Mouse button down:" << button;

    mouseDown[button] = true;

    if(!downCallbacks[button].empty())
        for(auto callback: downCallbacks[button])
            callback(*this, button);
}

void Mouse::up(MouseButton button) {
    Log::log << LogType::LOG_DEBUG << "Mouse button up:" << button;

    mouseDown[button] = false;

    if(!upCallbacks[button].empty())
        for(auto callback: upCallbacks[button])
            callback(*this, button);
}

MousePosition Mouse::position() {
    return mousePosition;
}

bool Mouse::isDown(MouseButton button) {
    return mouseDown[button];
}

bool Mouse::isUp(MouseButton button) {
    return !mouseDown[button];
}

void Mouse::addMoveCallback(std::function<void(Mouse&)> callback) {
    moveCallbacks.push_back(callback);
}

void Mouse::addScrollCallback(std::function<void(Mouse&, int)> callback) {
    scrollCallbacks.push_back(callback);
}

void Mouse::addDownCallback(MouseButton button, std::function<void(Mouse&, MouseButton)> callback) {
    downCallbacks[button].push_back(callback);
}

void Mouse::addUpCallback(MouseButton button, std::function<void(Mouse&, MouseButton)> callback) {
    upCallbacks[button].push_back(callback);
}
