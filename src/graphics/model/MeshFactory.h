//
// Created by Milan van Zanten on 27.03.18.
//

#ifndef BESTEST_GAME_MESHFACTORY_H
#define BESTEST_GAME_MESHFACTORY_H

#include "Model.h"

class MeshFactory {
public:
    static Model *addPlane(Model *addTo, glm::vec3 origin, float width, float height, glm::vec3 normal, glm::vec3 up, glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
};

#endif //BESTEST_GAME_MESHFACTORY_H
