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

bool shouldEnd = false;
IOControl *ioControl;
void endProgram(Keyboard &keyboard, int key, int scancode, int mods) {
    shouldEnd = true;
}
void toggleFullscreen(Keyboard &keyboard, int key, int scancode, int mods) {
    if(ioControl->window->isFullscreen())
        ioControl->window->setWindowed();
    else
        ioControl->window->setFullscreen(ioControl->getPrimaryMonitor());
}

int main() {
    auto *renderer = new Renderer;
    renderer->init();
    ioControl = new IOControl(renderer->getWindow());
    auto *world = new World;

    ioControl->keyboard->addReleasedCallback(endProgram, GLFW_KEY_ESCAPE);
    ioControl->keyboard->addReleasedCallback(toggleFullscreen, GLFW_KEY_F);
    ioControl->window->setWindowSizeLimits(640, 320, GLFW_DONT_CARE, GLFW_DONT_CARE);

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

        if(shouldEnd)
            break;
    }

    delete renderer;
    delete ioControl;
    delete world;

    return 0;
}