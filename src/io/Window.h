//
// Created by Milan van Zanten on 13.03.18.
//

#ifndef BESTEST_GAME_WINDOW_H
#define BESTEST_GAME_WINDOW_H

#include <functional>
#include <string>
#include <vector>
#include "../util/Config.h"
#include <GLFW/glfw3.h>

class Window {
public:
    explicit Window(GLFWwindow *window);

    // control
    /// Tell the window its size changed.
    /// \param width The new width of the window.
    /// \param height The new height of the window.
    void windowSizeChanged(int width, int height);
    /// Tell the framebuffer its size changed.
    /// \param width The new width of the framebuffer.
    /// \param height The new height of the framebuffer.
    void framebufferSizeChanged(int width, int height);
    /// Tell the window its focus changed.
    /// \param focus Whether the window has gained or lost input focus.
    void focusChanged(bool focus);
    /// Set a new size for the window. Ignores any window size limits.
    /// \param width The new width for the window.
    /// \param height The new height for the window.
    void setWindowSize(int width, int height);
    /// Set both an upper and lower limit for the window size. Use GLFW_DONT_CARE on a pair of width and height to remove the limit.
    /// \param minWidth A min width for the window.
    /// \param minHeight A min height for the window.
    /// \param maxWidth A max width for the window.
    /// \param maxHeight A max height for the window.
    void setWindowSizeLimits(int minWidth, int minHeight, int maxWidth, int maxHeight);
    /// Set a title for the window.
    /// \param title The new title for the window.
    void setWindowTitle(std::string title);
    /// Set a window to fullscreen. Width, height and refreshRate are being taken from the monitors vidmode if left at the default of -1.
    /// \param monitor The monitor to fullscreen the window on.
    /// \param width An optional horizontal resolution for the fullscreen window.
    /// \param height An optional vertical resolution for the fullscreen window.
    /// \param refreshRate An optional refresh rate for the fullscreen window.
    void setFullscreen(GLFWmonitor *monitor, int width = -1, int height = -1, int refreshRate = -1);
    /// Set a window back to windowed mode. Size and position are being reset to their values before
    /// \param width
    /// \param height
    /// \param xPos
    /// \param yPos
    void setWindowed(int width = -1, int height = -1, int xPos = -1, int yPos = -1);
    /// Hide the window.
    void hideWindow();
    /// Show the window.
    void showWindow();

    // query
    /// Add a callback that is triggered whenever the window size changes.
    /// \param callback The callback to call whenever the window size changes. The new window width and height are passed to the callback.
    void addWindowSizeCallback(std::function<void(Window&, int, int)> callback);
    /// Add a callback that is triggered whenever the framebuffer size changes.
    /// \param callback The callback to call whenever the framebuffer size changes. The new framebuffer width and height are passed to the callback.
    void addFramebufferSizeCallback(std::function<void(Window&, int, int)> callback);
    /// Return the current width of this window.
    int getWindowWidth() const;
    /// Return the current height of this window.
    int getWindowHeight() const;
    /// Return the current width of the framebuffer in this window.
    int getFramebufferWidth() const;
    /// Return the current height of the framebuffer in this window.
    int getFramebufferHeight() const;
    /// Return the monitor controlling the current window in fullscreen. Returns nullptr if the window is in windowed mode.
    GLFWmonitor *getWindowMonitor() const;
    /// Return a handle to the glfw window object.
    GLFWwindow *getGLFWWindow() const;
    /// Return whether the window is currently in fullscreen.
    bool isFullscreen() const;
    /// Return whether the window is currently hidden.
    bool isVisible() const;
    /// Return whether the window currently has input focus.
    bool isFocused() const;

private:
    /// A pointer to the glfw window structure this window controls.
    GLFWwindow *window;
    /// The monitor on which this window is in fullscreen or nullptr if the window is in windowed mode.
    GLFWmonitor *windowMonitor;
    /// Used to track the sizes of the window and the framebuffer.
    int windowWidth = CONFIG.DEFAULT_WINDOW_WIDTH, windowHeight = CONFIG.DEFAULT_WINDOW_HEIGHT, framebufferWidth = 0, framebufferHeight = 0;
    /// Used to track the focus and fullscreen state of the window.
    bool focus = true, fullscreen = false;
    /// Used when a fullscreen window is restored to windowed without a new size and position.
    int restoreWidth = CONFIG.DEFAULT_WINDOW_WIDTH, restoreHeight = CONFIG.DEFAULT_WINDOW_HEIGHT, restoreXPos = 0, restoreYPos = 0;
    std::vector<std::function<void(Window&, int, int)>> windowSizeCallbacks;
    std::vector<std::function<void(Window&, int, int)>> framebufferSizeCallbacks;
    std::vector<std::function<void(Window&, bool)>> focusedCallbacks;
};

#endif //BESTEST_GAME_WINDOW_H
