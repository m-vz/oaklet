//
// Created by Milan van Zanten on 09.02.18.
//

#include "graphics/Renderer.h"
#include <chrono>
#include <thread>
#include "world/World.h"
#include "io/IOControl.h"
#include "util/Log.h"

using namespace std;

int main() {
    auto *renderer = new Renderer;
    renderer->init();
    auto *ioControl = new IOControl(renderer->getWindow());
    auto *world = new World;

    bool endProgram = false;
    long long int timeLag = 0;
    std::chrono::time_point<std::chrono::steady_clock> lastTick, thisTick;

    while(true) {
        thisTick = chrono::steady_clock::now();
        world->time->tick((thisTick - lastTick).count());
        timeLag += world->time->deltaTime();
        lastTick = thisTick;

        /*
         * input
         */
        ioControl->processInput();

        /*
         * update
         */
        Log::log << LOG_DEBUG << "Updating world by " << (int) (timeLag - timeLag%CONFIG.TIME_TICK_DURATION) << "ns.";

        // first update this frame
        // update world and lag once
        world->firstUpdateInFrame(CONFIG.TIME_TICK_DURATION);
        timeLag -= CONFIG.TIME_TICK_DURATION;

        // other updates
        while(timeLag >= CONFIG.TIME_TICK_DURATION) {
            // update world
            world->update(CONFIG.TIME_TICK_DURATION);
            // update lag
            timeLag -= CONFIG.TIME_TICK_DURATION;
        }

        /*
         * render
         */
        renderer->render(timeLag);

        if(endProgram)
            break;
    }

    delete renderer;
    delete ioControl;
    delete world;

    return 0;
}