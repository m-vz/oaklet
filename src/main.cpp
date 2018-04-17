//
// Created by Milan van Zanten on 09.02.18.
//

#include "graphics/Renderer.h"
#include <chrono>
#include <thread>
#include "world/World.h"
#include "io/IOControl.h"
#include "util/Log.h"
#include "graphics/model/MeshFactory.h"
#include "world/Entity.h"

using namespace std;

bool shouldEnd = false;
Renderer *renderer;
IOControl *ioControl;
World *world;
FreeCamera *mainCamera;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void endProgram(Keyboard &keyboard, int key, int scancode, int mods) {
    shouldEnd = true;
}

void toggleFullscreen(Keyboard &keyboard, int key, int scancode, int mods) {
    if(ioControl->window->isFullscreen())
        ioControl->window->setWindowed();
    else
        ioControl->window->setFullscreen(ioControl->getPrimaryMonitor());
}

void resize(Window &window, int width, int height) {
    mainCamera->changeAspect(width, height);
}

void scrollMesh(Mouse &mouse, double xOffset, double yOffset) {
    world->activeScene->pointLights[0]->lightPosition.x += 2*xOffset/5.0f;
    world->activeScene->pointLights[0]->lightPosition.z += 2*yOffset/5.0f;
    world->activeScene->pointLights[1]->lightPosition.x += xOffset/5.0f;
    world->activeScene->pointLights[1]->lightPosition.z += yOffset/5.0f;
    Log::log << LOG_INFO << world->activeScene->pointLights[0]->lightPosition;
}
#pragma clang diagnostic pop

int main() {
    // PREPARE TEST

    renderer = new Renderer;
    renderer->init(CONFIG.DEFAULT_WINDOW_WIDTH, CONFIG.DEFAULT_WINDOW_HEIGHT);
    ioControl = new IOControl(renderer->getWindow());
    mainCamera = new FreeCamera(*ioControl->window, *ioControl->mouse, *ioControl->keyboard, glm::vec3(6, 8, 12), glm::vec2(-0.3f, 0.4f));
    world = new World;
    world->activeScene = new Scene(mainCamera);
    world->scenes.push_back(world->activeScene);
    renderer->camera = mainCamera;

    Model test = Model(), test2 = Model();
    test.loadModel("assets/meshes/medieval_house/medieval_house.obj");
    MeshFactory::addPlane(&test, glm::vec3(0.0f), 10000, 10000, glm::vec3(0, 1, 0), glm::vec3(0, 0, -1), glm::vec4(0.6f, 0.5f, 0.4f, 1));
    test2.setTranslation(glm::vec3(0, 1, 0));
    Entity testEntity = Entity(), testEntity2 = Entity();
    testEntity.setModel(&test);
    testEntity2.setModel(&test2);
    world->activeScene->entities.push_back(&testEntity);
    world->activeScene->entities.push_back(&testEntity2);
    PointLight testLight = PointLight(glm::vec3(0, 3, 0), glm::vec3(0.5f, 0.25f, 0.1f), 4), testLight2 = PointLight(glm::vec3(0, 20, 0), glm::vec3(1, 1, 1), 80);
    world->activeScene->pointLights.push_back(&testLight);
    world->activeScene->pointLights.push_back(&testLight2);

    ioControl->keyboard->addReleasedCallback(endProgram, GLFW_KEY_ESCAPE);
    ioControl->keyboard->addReleasedCallback(toggleFullscreen, GLFW_KEY_F);
    ioControl->mouse->addScrollCallback(scrollMesh);
    ioControl->window->setWindowSizeLimits(640, 420, GLFW_DONT_CARE, GLFW_DONT_CARE);
    ioControl->window->addFramebufferSizeCallback(resize);

    // MAIN LOOP

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

        // special updates that are run once per frame
        world->perFrameUpdate(world->time->deltaTime());

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
        renderer->renderScene(world->activeScene, timeLag);

        if(shouldEnd)
            break;
    }

    delete renderer;
    delete ioControl;
    delete world;
    delete mainCamera;

    return 0;
}