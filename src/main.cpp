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

using namespace std;

bool shouldEnd = false;
Renderer *renderer;
IOControl *ioControl;
World *world;
FreeCamera *mainCamera;
float angle = -0.44f;

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
    mainCamera->changeAspectRatio(width, height);
    renderer->lighting.setViewportSize(ioControl->window->getFramebufferWidth(), ioControl->window->getFramebufferHeight());
}

void scroll(Mouse &mouse, double xOffset, double yOffset) {
    world->activeScene->pointLights[0]->lightPosition += glm::vec3(xOffset, 0, yOffset)/5.0f;
}
#pragma clang diagnostic pop

int main() {
    // PREPARE TEST

    renderer = new Renderer;
    renderer->init(CONFIG.DEFAULT_WINDOW_WIDTH, CONFIG.DEFAULT_WINDOW_HEIGHT);
    ioControl = new IOControl(renderer->getWindow());
    renderer->lighting.setViewportSize(ioControl->window->getFramebufferWidth(), ioControl->window->getFramebufferHeight());
    mainCamera = new FreeCamera(*ioControl->window, *ioControl->mouse, *ioControl->keyboard, glm::vec3(6, 8, 12), glm::vec2(-0.3f, 0.4f));
    world = new World;
    world->activeScene = new Scene(mainCamera);
    world->scenes.push_back(world->activeScene);
    renderer->camera = mainCamera;

    Model test = Model();
    Entity testEntity = Entity();

//    Texture blackTexture("assets/images/pixel_black.png", true);
//    blackTexture.fillTexture(false);

    DirectionalLight testSunLight = DirectionalLight(glm::vec3(cosf(angle), 1, sinf(angle)), glm::vec3(0.933333333f, 0.847058824f, 0.62745098f), 1);
    PointLight testPointLight = PointLight(glm::vec3(0.15f, 1, 4.3f), glm::vec3(0.5f, 0.25f, 0.1f), 20);
    SpotLight testSpotLight = SpotLight(glm::vec3(1, 4, 7.3f), glm::vec3(-1, -1, -1), glm::vec3(1), 30, glm::radians(30.0f));

    Skybox skybox("assets/samples/images/skyboxes/thick_clouds");

    test.loadModel("assets/samples/meshes/medieval_house/medieval_house.obj");
    MeshFactory::addCuboid(&test, glm::vec3(0, -2, 0), 40, 4, 40, glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), glm::vec4(0.2f, 0.16f, 0.13f, 1));

    testEntity.setModel(&test);
    world->activeScene->entities.push_back(&testEntity);
    world->activeScene->directionalLights.push_back(&testSunLight);
    world->activeScene->pointLights.push_back(&testPointLight);
    world->activeScene->spotLights.push_back(&testSpotLight);
    world->activeScene->skybox = &skybox;

    renderer->lighting.setScene(world->activeScene);
    renderer->shadowing.setScene(world->activeScene);

    ioControl->keyboard->addReleasedCallback(endProgram, GLFW_KEY_ESCAPE);
    ioControl->keyboard->addReleasedCallback(toggleFullscreen, GLFW_KEY_F);
    ioControl->mouse->addScrollCallback(scroll);
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

    return 0;
}