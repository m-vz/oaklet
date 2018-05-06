//
// Created by Milan van Zanten on 16.04.18.
//

#ifndef BESTEST_GAME_SCENE_H
#define BESTEST_GAME_SCENE_H

#include "../graphics/model/Model.h"
#include "../graphics/camera/Camera.h"
#include "../graphics/light/PointLight.h"
#include "entity/ModelEntity.h"
#include "../graphics/light/DirectionalLight.h"
#include "../graphics/light/SpotLight.h"
#include "Skybox.h"
#include "entity/UpdateEntity.h"

class Scene {
public:
    std::vector<UpdateEntity*> updateEntities;
    std::vector<ModelEntity*> meshEntities;
    std::vector<DirectionalLight*> directionalLights;
    std::vector<PointLight*> pointLights;
    std::vector<SpotLight*> spotLights;
    std::vector<Camera*> cameras;
    Camera *activeCamera = nullptr;
    Skybox *skybox = nullptr;

    explicit Scene(Camera *activeCamera);
};

#endif //BESTEST_GAME_SCENE_H
