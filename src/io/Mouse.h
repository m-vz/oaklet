//
// Created by Milan van Zanten on 09.02.18.
//

#ifndef BESTEST_GAME_MOUSE_H
#define BESTEST_GAME_MOUSE_H

#include <functional>
#include <vector>
#include <GLFW/glfw3.h>

struct MousePosition {
    double x;
    double y;
};

class Mouse {
public:
    // control
    /// Tell the mouse it was moved to a specific location.
    void movedTo(MousePosition position);
    /// Tell the mouse it was moved to a specific location.
    void movedTo(double x, double y);
    /// Tell the mouse it was moved.
    void scrolled(double xOffset, double yOffset);
    /// Tell the mouse a specific button is down.
    /// \param button The MouseButton that is down.
    void down(int button);
    /// Tell the mouse a specific button is up.
    /// \param button The mouse button code (GLFW_MOUSE_BUTTON_xxx) that is up.
    void up(int button);

    // query
    /// Get the current position of the mouse.
    /// \return A MousePosition structure specifying the mouse position.
    MousePosition position();
    /// Ask whether a specific button is currently down.
    /// \param button The button whose state is asked.
    /// \return Whether the specified mouse button code (GLFW_MOUSE_BUTTON_xxx) is currently down.
    bool isDown(int button);
    /// Ask whether a specific button is currently up.
    /// \param button The button whose state is asked.
    /// \return Whether the specified mouse button code (GLFW_MOUSE_BUTTON_xxx) is currently up.
    bool isUp(int button);
    /// Add a callback that is triggered whenever the mouse is moved.
    /// \param callback The callback to call whenever the mouse is moved.
    void addMoveCallback(std::function<void(Mouse&)> callback);
    /// Add a callback that is triggered whenever the mouse wheel is scrolled. The x and y offsets are passed on to the callback.
    /// \param callback The callback to call whenever the mouse wheel is scrolled.
    void addScrollCallback(std::function<void(Mouse&, double, double)> callback);
    /// Add a callback that is triggered whenever a specific mouse button code (GLFW_MOUSE_BUTTON_xxx) is pressed down.
    /// \param button The button that triggers the callback.
    /// \param callback The callback to call whenever a specific mouse button code (GLFW_MOUSE_BUTTON_xxx) is pressed down.
    void addDownCallback(int button, std::function<void(Mouse&, int)> callback);
    /// Add a callback that is triggered whenever a specific mouse button code (GLFW_MOUSE_BUTTON_xxx) is released.
    /// \param button The button that triggers the callback.
    /// \param callback The callback to call whenever a specific mouse button code (GLFW_MOUSE_BUTTON_xxx) is released.
    void addUpCallback(int button, std::function<void(Mouse&, int)> callback);

private:
    bool mouseDown[GLFW_MOUSE_BUTTON_LAST + 1]{};
    MousePosition mousePosition{};
    std::vector<std::function<void(Mouse&)>> moveCallbacks;
    std::vector<std::function<void(Mouse&, double, double)>> scrollCallbacks;
    std::vector<std::function<void(Mouse&, int)>> downCallbacks[GLFW_MOUSE_BUTTON_LAST + 1];
    std::vector<std::function<void(Mouse&, int)>> upCallbacks[GLFW_MOUSE_BUTTON_LAST + 1];
};

#endif //BESTEST_GAME_MOUSE_H
