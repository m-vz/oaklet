//
// Created by Milan van Zanten on 30.04.18.
//

#include <chrono>
#include "BestestGameEngine.h"
#include "graphics/model/MeshFactory.h"
#include "util/Log.h"
#include "world/entity/ModelEntity.h"
#include "graphics/light/MovingPointLight.h"

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
    for(auto scene: engine->world->scenes)
        for(auto camera: scene->cameras)
            camera->changeAspectRatio(width, height);
    engine->renderer->lighting.setViewportSize(engine->ioControl->window->getFramebufferWidth(), engine->ioControl->window->getFramebufferHeight());
}

void scroll(Mouse &mouse, double xOffset, double yOffset) {

}
#pragma clang diagnostic pop

int main() {
    engine = new BestestGameEngine;
    engine->init();

    auto mainCamera = new FreeCamera(*engine->ioControl->window, *engine->ioControl->mouse, *engine->ioControl->keyboard, glm::vec3(0, 8, 12), glm::vec2(-0.62f, 0));
    auto *testScene = new Scene(mainCamera);

    MovingPointLight light1 = MovingPointLight(glm::vec3(0, 2, 0), glm::vec3(1, 0.7f, 1), 8);
    testScene->pointLights.push_back(&light1);
    testScene->updateEntities.push_back(&light1);
    DirectionalLight light2 = DirectionalLight(glm::vec3(-1, 0.3f, 0.4f), glm::vec3(0.6f, 0.8f, 1), 0.2f);
    testScene->directionalLights.push_back(&light2);

    Model boxes, statue, floor;
    ModelEntity boxesEntity, statueEntity, floorEntity;
    glm::vec4 color = glm::vec4(0.8f, 0.6f, 0.4f, 1);
    int count = 20;
    float distance = 6, height = 0.4f;
    for (int i = 0; i < count; ++i) {
        float alpha = glm::radians(i*360.0f/count);
        glm::vec3 position = distance*glm::vec3(cosf(alpha), 0, sinf(alpha));
        MeshFactory::addCube(&boxes, glm::vec3(0, height, 0)+position, 0.8f, -position, glm::vec3(0, 1, 0), color);
    }
    statue.loadModel("../assets/samples/meshes/statue/statue_lowpoly.obj");
    statue.setScale(2.5f);
    statue.setTranslation(glm::vec3(0, -0.03f, 0));
    statue.meshes[0]->setColor(color);
    MeshFactory::addPlane(&floor, glm::vec3(0), 1000, 1000, glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), color);
    boxesEntity.setModel(&boxes);
    testScene->meshEntities.push_back(&boxesEntity);
    statueEntity.setModel(&statue);
    testScene->meshEntities.push_back(&statueEntity);
    floorEntity.setModel(&floor);
    testScene->meshEntities.push_back(&floorEntity);

    Skybox s = Skybox("../assets/samples/images/skyboxes/full_moon");
    testScene->skybox = &s;

    engine->setScene(testScene);

    BitmapFont *font = new BitmapFont("../assets/fonts/bitmap/font_bitmap.bmp", glm::vec2(8, 8), glm::vec2(16, 16),
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
