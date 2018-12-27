//
// Created by Milan van Zanten on 30.04.18.
//

#ifndef BESTEST_GAME_BESTESTGAMEENGINE_H
#define BESTEST_GAME_BESTESTGAMEENGINE_H

#include "graphics/Renderer.h"
#include "world/World.h"
#include "io/IOControl.h"

class BestestGameEngine {
public:
    static const int FRAMERATE_COMPUTATION_SAMPLE_COUNT = 4;

    Renderer *renderer;
    IOControl *ioControl;
    World *world;

    FreeCamera *mainCamera;

    long long int frameTime[FRAMERATE_COMPUTATION_SAMPLE_COUNT];
    int frameTimeSampleCount = 0;

    BestestGameEngine();
    void init();
    void setScene(Scene *scene);
    void start();
    void stop();

private:
    bool shouldEnd = false;
};

#endif //BESTEST_GAME_BESTESTGAMEENGINE_H
