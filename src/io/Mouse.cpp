//
// Created by Milan van Zanten on 09.02.18.
//

#include "Mouse.h"
#include "../util/Log.h"

void Mouse::movedTo(MousePosition position) {
    Log::log << LogType::LOG_DEBUG << "Mouse moved to: (" << position.x << ", " << position.y << ")";

    mousePosition = position;

    if(!moveCallbacks.empty())
        for(const auto &callback: moveCallbacks)
            callback(*this);
}

void Mouse::movedTo(double x, double y) {
    movedTo({x, y});
}

void Mouse::scrolled(double xOffset, double yOffset) {
    Log::log << LogType::LOG_INFO << "Mouse scrolled: (" << xOffset << ", " << yOffset << ")";

    if(!scrollCallbacks.empty())
        for(const auto &callback: scrollCallbacks)
            callback(*this, xOffset, yOffset);
}

void Mouse::down(int button) {
    Log::log << LogType::LOG_INFO << "Mouse button down: " << button;

    mouseDown[button] = true;

    if(!downCallbacks[button].empty())
        for(const auto &callback: downCallbacks[button])
            callback(*this, button);
}

void Mouse::up(int button) {
    Log::log << LogType::LOG_INFO << "Mouse button up: " << button;

    mouseDown[button] = false;

    if(!upCallbacks[button].empty())
        for(const auto &callback: upCallbacks[button])
            callback(*this, button);
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
