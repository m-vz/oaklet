//
// Created by Milan van Zanten on 13.03.18.
//

#include <GLFW/glfw3.h>
#include "Window.h"
#include "../util/Log.h"

Window::Window(GLFWwindow *window) : window(window) {
    windowMonitor = glfwGetWindowMonitor(window);

    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
}

void Window::windowSizeChanged(int width, int height) {
    Log::log << LogType::LOG_INFO << "Window width changed to " << width << "x" << height;

    windowWidth = width;
    windowHeight = height;

    if(!windowSizeCallbacks.empty())
        for(const auto &callback: windowSizeCallbacks)
            callback(*this, width, height);
}

void Window::framebufferSizeChanged(int width, int height) {
    Log::log << LogType::LOG_INFO << "Framebuffer width changed to " << width << "x" << height;

    framebufferWidth = width;
    framebufferHeight = height;

    if(!framebufferSizeCallbacks.empty())
        for(const auto &callback: framebufferSizeCallbacks)
            callback(*this, width, height);
}

void Window::focusChanged(bool focus) {
    Log::log << LogType::LOG_INFO << "Window focus " << (focus ? "gained" : "lost");

    Window::focus = focus;

    if(!focusedCallbacks.empty())
        for(const auto &callback: focusedCallbacks)
            callback(*this, focus);
}

void Window::setWindowSize(int width, int height) {
    glfwSetWindowSize(window, width, height);
}

void Window::setWindowSizeLimits(int minWidth, int minHeight, int maxWidth, int maxHeight) {
    Log::log << LogType::LOG_INFO
             << "Set window size limits to: min width " << (minWidth < 0 ? "not set" : std::to_string(minWidth))
             << ", min height " << (minHeight < 0 ? "not set" : std::to_string(minHeight))
             << ", max width " << (maxWidth < 0 ? "not set" : std::to_string(maxWidth))
             << ", max height " << (maxHeight < 0 ? "not set" : std::to_string(maxHeight));

    glfwSetWindowSizeLimits(window, minWidth, minHeight, maxWidth, maxHeight);
}

void Window::setWindowTitle(std::string title) {
    Log::log << LogType::LOG_INFO << "Set window title to: \"" << title << "\"";

    glfwSetWindowTitle(window, title.c_str());
}

void Window::setFullscreen(GLFWmonitor *monitor, int width, int height, int refreshRate) {
    if(isFullscreen()) {
        Log::log << LOG_WARNING << "Tried to set a window to fullscreen that was already in fullscreen.";
    } else {
        Log::log << LogType::LOG_INFO << "Set window to fullscreen";

        // save the current size and position for when the window is restored to windowed mode without a new position and size
        glfwGetWindowSize(window, &restoreWidth, &restoreHeight);
        glfwGetWindowPos(window, &restoreXPos, &restoreYPos);

        // take parameters from the monitors vidmode if they have not been specified
        const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);
        if (width < 0)
            width = vidmode->width;
        if (height < 0)
            height = vidmode->height;
        if (refreshRate < 0)
            refreshRate = vidmode->refreshRate;

        glfwSetWindowMonitor(window, monitor, 0, 0, width, height, refreshRate);
        fullscreen = true;
    }
}

void Window::setWindowed(int width, int height, int xPos, int yPos) {
    if(!isFullscreen()) {
        Log::log << LOG_WARNING << "Tried to set a window to windowed that was already windowed.";
    } else {
        Log::log << LogType::LOG_INFO << "Set window to windowed";

        // restore parameters from the last time the window was in windowed mode if they have not been specified
        if (width < 0)
            width = restoreWidth;
        if (height < 0)
            height = restoreHeight;
        if (xPos < 0)
            xPos = restoreXPos;
        if (yPos < 0)
            yPos = restoreYPos;

        glfwSetWindowMonitor(window, nullptr, xPos, yPos, width, height, 0);
        fullscreen = false;
    }
}

void Window::hideWindow() {
    if(!isVisible()) {
        Log::log << LOG_WARNING << "Tried to hide a window that was already hidden.";
    } else {
        Log::log << LogType::LOG_INFO << "Window hidden";

        glfwHideWindow(window);
    }
}

void Window::showWindow() {
    if(isVisible()) {
        Log::log << LOG_WARNING << "Tried to show a window that was already visible.";
    } else {
        Log::log << LogType::LOG_INFO << "Window shown";

        glfwShowWindow(window);
    }
}

void Window::addWindowSizeCallback(std::function<void(Window &, int, int)> callback) {
    windowSizeCallbacks.push_back(callback);
}

void Window::addFramebufferSizeCallback(std::function<void(Window &, int, int)> callback) {
    framebufferSizeCallbacks.push_back(callback);
}

int Window::getWindowWidth() const {
    return windowWidth;
}

int Window::getWindowHeight() const {
    return windowHeight;
}

int Window::getFramebufferWidth() const {
    return framebufferWidth;
}

int Window::getFramebufferHeight() const {
    return framebufferHeight;
}

GLFWmonitor *Window::getWindowMonitor() const {
    return windowMonitor;
}

bool Window::isFullscreen() const {
    return fullscreen;
}

bool Window::isVisible() const {
    return static_cast<bool>(glfwGetWindowAttrib(window, GLFW_VISIBLE));
}

bool Window::isFocused() const {
    return focus;
}
