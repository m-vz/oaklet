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
Renderer *renderer;
IOControl *ioControl;
World *world;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void endProgram(Keyboard &keyboard, int key, int scancode, int mods) {
    shouldEnd = true;
}
#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void toggleFullscreen(Keyboard &keyboard, int key, int scancode, int mods) {
    if(ioControl->window->isFullscreen())
        ioControl->window->setWindowed();
    else
        ioControl->window->setFullscreen(ioControl->getPrimaryMonitor());
}
#pragma clang diagnostic pop

void resize(Window &window, int width, int height) {
    ioControl->camera->changeAspect(width, height);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void scrollMesh(Mouse &mouse, double xOffset, double yOffset) {
    renderer->lightPositionVector.x += xOffset/5.0f;
    renderer->lightPositionVector.z += yOffset/5.0f;
    Log::log << LOG_INFO << renderer->lightPositionVector;
}
#pragma clang diagnostic pop

int main() {
    renderer = new Renderer;
    renderer->init(CONFIG.DEFAULT_WINDOW_WIDTH, CONFIG.DEFAULT_WINDOW_HEIGHT);
    ioControl = new IOControl(renderer->getWindow());
    world = new World;
    renderer->camera = ioControl->camera;
    ioControl->time = world->time;

    ioControl->keyboard->addReleasedCallback(endProgram, GLFW_KEY_ESCAPE);
    ioControl->keyboard->addReleasedCallback(toggleFullscreen, GLFW_KEY_F);
    ioControl->mouse->addScrollCallback(scrollMesh);
    ioControl->window->setWindowSizeLimits(640, 420, GLFW_DONT_CARE, GLFW_DONT_CARE);
    ioControl->window->addFramebufferSizeCallback(resize);

    long long int timeLag = 0;
    std::chrono::time_point<std::chrono::steady_clock> thisTick = chrono::steady_clock::now(), lastTick = thisTick;

    while(!glfwWindowShouldClose(renderer->getWindow())) {
        thisTick = chrono::steady_clock::now();
        world->time->tick(std::chrono::duration_cast<std::chrono::nanoseconds>(thisTick - lastTick).count());
        timeLag += world->time->deltaTime();
        lastTick = thisTick;

        /*
         * input
         */
        glfwPollEvents();
        ioControl->processInput();

        /*
         * update
         */
        Log::log << LOG_FRAME << "Updating world by " << (int) (timeLag - timeLag%CONFIG.TIME_TICK_DURATION) << "ns.";

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