//
// Created by Milan van Zanten on 09.02.18.
//

#ifndef BESTEST_GAME_WORLD_H
#define BESTEST_GAME_WORLD_H

#include "Time.h"
#include "physics/Physics.h"
#include "Scene.h"

class World {
public:
    Time *time;
    Physics *physics;
    std::vector<Scene*> scenes;
    Scene *activeScene;

    World();
    void init(Scene *initialScene);
    void perFrameUpdate(long long int frameTime);
    void firstUpdateInFrame(long long int deltaTime);
    void update(long long int deltaTime);
    long long int age();
    virtual ~World();
};

#endif //BESTEST_GAME_WORLD_H
