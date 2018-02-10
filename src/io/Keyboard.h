//
// Created by Milan van Zanten on 10.02.18.
//

#ifndef BESTEST_GAME_KEYBOARD_H
#define BESTEST_GAME_KEYBOARD_H

class Keyboard {
public:
    /// Tell the keyboard a specific button is used.
    /// \param key The key that is used.
    /// \param action The action that happened on the key (GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE).
    /// \param mods The mods used when using this key.
    void key(int key, int action, int mods);
};

#endif //BESTEST_GAME_KEYBOARD_H
