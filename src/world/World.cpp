//
// Created by Milan van Zanten on 09.02.18.
//

#include "World.h"
#include "../util/Log.h"

World::World() {
    time = new Time;
    physics = new Physics;

}

void World::firstUpdateInFrame(long long int deltaTime) {
    // special updates at the start of each frame go here.

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
}
