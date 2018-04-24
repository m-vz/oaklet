//
// Created by Milan van Zanten on 09.02.18.
//

#ifndef BESTEST_GAME_CONFIG_H
#define BESTEST_GAME_CONFIG_H

#define BESTEST_GAME_DEBUG // comment to not use

#include <string>

static struct {
    // logging
    const std::string DEFAULT_LOG_FILE_PATH = "log.txt"; // If this is more than just a filename, make sure, the directory exists.
#ifdef BESTEST_GAME_DEBUG
    const int DEFAULT_LOG_LEVEL = 2; // LOG_INFO
    const int DEFAULT_FILE_LOG_LEVEL = 3; // LOG_DEBUG
#else
    const int DEFAULT_LOG_LEVEL = 1; // LOG_WARNING
    const int DEFAULT_FILE_LOG_LEVEL = 2; // LOG_INFO
#endif

    // time
    /// How many nanoseconds the clock waits per tick.
    const int TIME_TICK_DURATION = 1000000;

    // renderer
    const int DEFAULT_WINDOW_WIDTH = 1200, DEFAULT_WINDOW_HEIGHT = 900;
} CONFIG; // NOLINT

#endif //BESTEST_GAME_CONFIG_H
