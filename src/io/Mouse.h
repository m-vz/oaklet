//
// Created by Milan van Zanten on 09.02.18.
//

#ifndef BESTEST_GAME_MOUSE_H
#define BESTEST_GAME_MOUSE_H

#include <functional>
#include <vector>

struct MousePosition {
    int x;
    int y;
};

const unsigned int mouseButtonCount = 3;
enum MouseButton {
    LEFT_BUTTON,
    RIGHT_BUTTON,
    MIDDLE_BUTTON
};

class Mouse {
public:
    // control
    /// Tell the mouse it was moved to a specific location.
    void movedTo(MousePosition position);
    /// Tell the mouse it was moved to a specific location.
    void movedTo(int x, int y);
    /// Tell the mouse it was moved, expressed in multiples or divisions of WHEEL_DELTA, which is 120. A positive value indicates forward rotation; a negative value indicates backward rotation.
    void scrolled(int wheelDelta);
    /// Tell the mouse a specific button is down.
    /// \param button The MouseButton that is down.
    void down(MouseButton button);
    /// Tell the mouse a specific button is up.
    /// \param button The MouseButton that is up.
    void up(MouseButton button);

    // query
    /// Get the current position of the mouse.
    /// \return A MousePosition structure specifying the mouse position.
    MousePosition position();
    /// Ask whether a specific button is currently down.
    /// \param button The button whose state is asked.
    /// \return Whether the specified MouseButton is currently down.
    bool isDown(MouseButton button);
    /// Ask whether a specific button is currently up.
    /// \param button The button whose state is asked.
    /// \return Whether the specified MouseButton is currently up.
    bool isUp(MouseButton button);
    /// Add a callback that is triggered whenever the mouse is moved.
    /// \param callback The callback to call whenever the mouse is moved.
    void addMoveCallback(std::function<void(Mouse&)> callback);
    /// Add a callback that is triggered whenever the mouse wheel is scrolled.
    /// \param callback The callback to call whenever the mouse wheel is scrolled.
    void addScrollCallback(std::function<void(Mouse&, int)> callback);
    /// Add a callback that is triggered whenever a specific MouseButton is pressed down.
    /// \param button The button that triggers the callback.
    /// \param callback The callback to call whenever a specific MouseButton is pressed down.
    void addDownCallback(MouseButton button, std::function<void(Mouse&, MouseButton)> callback);
    /// Add a callback that is triggered whenever a specific MouseButton is released.
    /// \param button The button that triggers the callback.
    /// \param callback The callback to call whenever a specific MouseButton is released.
    void addUpCallback(MouseButton button, std::function<void(Mouse&, MouseButton)> callback);

private:
    bool mouseDown[mouseButtonCount]{};
    MousePosition mousePosition{};
    std::vector<std::function<void(Mouse&)>> moveCallbacks;
    std::vector<std::function<void(Mouse&, int)>> scrollCallbacks;
    std::vector<std::function<void(Mouse&, MouseButton)>> downCallbacks[mouseButtonCount];
    std::vector<std::function<void(Mouse&, MouseButton)>> upCallbacks[mouseButtonCount];
};

#endif //BESTEST_GAME_MOUSE_H
