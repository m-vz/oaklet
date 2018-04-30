//
// Created by Milan van Zanten on 09.02.18.
//

#include "World.h"
#include "../util/Log.h"

World::World() {
    time = new Time;
    physics = new Physics;
}

void World::init(Scene *initialScene) {
    activeScene = initialScene;
    scenes.push_back(initialScene);
}

void World::perFrameUpdate(long long int frameTime) {
    // special updates that run once per frame go here
    for(auto scene: scenes)
        for(auto camera: scene->cameras)
            camera->update(frameTime);
}

void World::firstUpdateInFrame(long long int deltaTime) {
    // special updates at the start of each frame go here

    // do the usual updates as well
    update(deltaTime);
}

void World::update(long long int deltaTime) {
    physics->update(deltaTime);
}

long long int World::age() {
    return time->timeSinceEpoch();
}

World::~World() {
    delete time;
    delete physics;
    for(auto scene: scenes)
        delete scene;
}
