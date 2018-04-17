//
// Created by Milan van Zanten on 16.04.18.
//

#ifndef BESTEST_GAME_SCENE_H
#define BESTEST_GAME_SCENE_H

#include "../graphics/model/Model.h"
#include "../graphics/camera/Camera.h"
#include "../graphics/light/PointLight.h"
#include "Entity.h"

class Scene {
public:
    std::vector<Entity*> entities;
    std::vector<PointLight*> pointLights;
    std::vector<Camera*> cameras;
    Camera *activeCamera;

    explicit Scene(Camera *activeCamera);
};

#endif //BESTEST_GAME_SCENE_H
