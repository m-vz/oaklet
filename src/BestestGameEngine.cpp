//
// Created by Milan van Zanten on 30.04.18.
//

#include <chrono>
#include "BestestGameEngine.h"
#include "graphics/model/MeshFactory.h"
#include "util/Log.h"

BestestGameEngine::BestestGameEngine() {
    renderer = new Renderer;
    ioControl = new IOControl;
    world = new World;
}

void BestestGameEngine::init() {
    renderer->init(CONFIG.DEFAULT_WINDOW_WIDTH, CONFIG.DEFAULT_WINDOW_HEIGHT);
    ioControl->init(renderer->getWindow());
    renderer->lighting.setViewportSize(ioControl->window->getFramebufferWidth(), ioControl->window->getFramebufferHeight());
}

void BestestGameEngine::setScene(Scene *scene) {
    world->init(scene);
    renderer->shadowing.setScene(scene);
    renderer->lighting.setScene(scene);
}

void BestestGameEngine::start() {
    long long int timeLag = 0;
    std::chrono::time_point<std::chrono::steady_clock> thisTick = std::chrono::steady_clock::now(), lastTick = thisTick;

    while(!glfwWindowShouldClose(renderer->getWindow())) {
        thisTick = std::chrono::steady_clock::now();
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
}

void BestestGameEngine::stop() {
    shouldEnd = true;
}

BestestGameEngine *engine;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void endProgram(Keyboard &keyboard, int key, int scancode, int mods) {
    engine->stop();
}

void toggleFullscreen(Keyboard &keyboard, int key, int scancode, int mods) {
    if(engine->ioControl->window->isFullscreen())
        engine->ioControl->window->setWindowed();
    else
        engine->ioControl->window->setFullscreen(engine->ioControl->getPrimaryMonitor());
}

void resize(Window &window, int width, int height) {
    engine->mainCamera->changeAspectRatio(width, height);
    engine->renderer->lighting.setViewportSize(engine->ioControl->window->getFramebufferWidth(), engine->ioControl->window->getFramebufferHeight());
}

void scroll(Mouse &mouse, double xOffset, double yOffset) {
    engine->world->activeScene->pointLights[0]->lightPosition += glm::vec3(xOffset, 0, yOffset)/5.0f;
}
#pragma clang diagnostic pop

int main() {
    engine = new BestestGameEngine;
    engine->init();

    auto mainCamera = new FreeCamera(*engine->ioControl->window, *engine->ioControl->mouse, *engine->ioControl->keyboard, glm::vec3(6, 8, 12), glm::vec2(-0.3f, 0.4f));
    auto *testScene = new Scene(mainCamera);

    Model test = Model();
    Entity testEntity = Entity();

    DirectionalLight testSunLight = DirectionalLight(glm::vec3(cosf(-0.44f), 1, sinf(-0.44f)), glm::vec3(0.933333333f, 0.847058824f, 0.62745098f), 1);
    PointLight testPointLight = PointLight(glm::vec3(3.8f, 1, 3.5f), glm::vec3(0.5f, 0.25f, 0.1f), 20);
    SpotLight testSpotLight = SpotLight(glm::vec3(1, 4, 7.3f), glm::vec3(-1, -1, -1), glm::vec3(1), 30, glm::radians(30.0f));

    Skybox skybox("assets/samples/images/skyboxes/thick_clouds");

    test.loadModel("assets/samples/meshes/medieval_house/medieval_house.obj");
    MeshFactory::addCuboid(&test, glm::vec3(0, -2, 0), 40, 4, 40, glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), glm::vec4(0.2f, 0.16f, 0.13f, 1));

    testEntity.setModel(&test);
    testScene->entities.push_back(&testEntity);
    testScene->directionalLights.push_back(&testSunLight);
    testScene->pointLights.push_back(&testPointLight);
    testScene->spotLights.push_back(&testSpotLight);
    testScene->skybox = &skybox;

    engine->setScene(testScene);

    BitmapFont *font = new BitmapFont("assets/fonts/bitmap/font_bitmap.bmp", glm::vec2(8, 8), glm::vec2(16, 16),
                                      {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                       'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5',
                                       '6', '7', '8', '9', ':', ';', '#', '%', '+', '-', '/', '\\', '!', '?', '.', ',',
                                       '\'', '*', '(', ')', '[', ']', '{', '}', '<', '>', '"', '=', ' ', '_', '|', '\0'});

    engine->ioControl->keyboard->addReleasedCallback(endProgram, GLFW_KEY_ESCAPE);
    engine->ioControl->keyboard->addReleasedCallback(toggleFullscreen, GLFW_KEY_F);
    engine->ioControl->mouse->addScrollCallback(scroll);
    engine->ioControl->window->setWindowSizeLimits(640, 420, GLFW_DONT_CARE, GLFW_DONT_CARE);
    engine->ioControl->window->addFramebufferSizeCallback(resize);

    engine->start();

    delete engine;
    return 0;
}
