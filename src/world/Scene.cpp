//
// Created by Milan van Zanten on 16.04.18.
//

#include "Scene.h"
#include "../graphics/camera/FreeCamera.h"

Scene::Scene(Camera *activeCamera) : activeCamera(activeCamera) {
    cameras.push_back(activeCamera);
}
