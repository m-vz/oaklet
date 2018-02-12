//
// Created by Milan van Zanten on 10.02.18.
//

#include "Keyboard.h"
#include "../util/Log.h"

void Keyboard::keyPressed(int key, int scancode, int mods) {
    if(key != -1) {
        Log::log << LogType::LOG_INFO << "Key " << key << " pressed (scancode " << scancode << ", mods " << mods << ")";
        keyPressed(-1, scancode, mods); // also call all general callbacks
    }

    if(keyPressedCallbacks.count(key) != 0)
        if(!keyPressedCallbacks[key].empty())
            for(const auto &callback: keyPressedCallbacks[key])
                callback(*this, key, scancode, mods);
}

void Keyboard::keyRepeated(int key, int scancode, int mods) {
    if(key != -1) {
        Log::log << LogType::LOG_INFO << "Key " << key << " repeated (scancode " << scancode << ", mods " << mods << ")";
        keyRepeated(-1, scancode, mods); // also call all general callbacks
    }

    if(keyRepeatedCallbacks.count(key) != 0)
        if(!keyRepeatedCallbacks[key].empty())
            for(const auto &callback: keyRepeatedCallbacks[key])
                callback(*this, key, scancode, mods);
}

void Keyboard::keyReleased(int key, int scancode, int mods) {
    if(key != -1) {
        Log::log << LogType::LOG_INFO << "Key " << key << " released (scancode " << scancode << ", mods " << mods << ")";
        keyReleased(-1, scancode, mods); // also call all general callbacks
    }

    if(keyReleasedCallbacks.count(key) != 0)
        if(!keyReleasedCallbacks[key].empty())
            for(const auto &callback: keyReleasedCallbacks[key])
                callback(*this, key, scancode, mods);
}

void Keyboard::addPressedCallback(std::function<void(Keyboard &, int, int, int)> callback, int key) {
    if(keyPressedCallbacks.count(key) != 0)
        keyPressedCallbacks[key].push_back(callback);
    else
        keyPressedCallbacks[key] = std::vector<std::function<void(Keyboard&, int, int, int)>>{callback};
}

void Keyboard::addRepeatedCallback(std::function<void(Keyboard &, int, int, int)> callback, int key) {
    if(keyRepeatedCallbacks.count(key) != 0)
        keyRepeatedCallbacks[key].push_back(callback);
    else
        keyRepeatedCallbacks[key] = std::vector<std::function<void(Keyboard&, int, int, int)>>{callback};
}

void Keyboard::addReleasedCallback(std::function<void(Keyboard &, int, int, int)> callback, int key) {
    if(keyReleasedCallbacks.count(key) != 0)
        keyReleasedCallbacks[key].push_back(callback);
    else
        keyReleasedCallbacks[key] = std::vector<std::function<void(Keyboard&, int, int, int)>>{callback};
}
