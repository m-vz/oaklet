//
// Created by Milan van Zanten on 27.03.18.
//

#ifndef BESTEST_GAME_MESHFACTORY_H
#define BESTEST_GAME_MESHFACTORY_H

#include "Model.h"

class MeshFactory {
public:
    static Model *addPlane(Model *addTo, glm::vec3 origin, float width, float height, glm::vec3 normal, glm::vec3 up, glm::vec4 color = glm::vec4(0, 0, 0, 1));
    static Model *addQuad(Model *addTo, glm::vec2 origin = glm::vec2(0), float width = 2, float height = 2, glm::vec4 color = glm::vec4(0, 0, 0, 1));
    static Model *addCuboid(Model *addTo, glm::vec3 origin, float width, float height, float length, glm::vec3 forward, glm::vec3 up, glm::vec4 color = glm::vec4(0, 0, 0, 1), bool insideOut = false);
    static Model *addCube(Model *addTo, glm::vec3 origin, float size, glm::vec3 forward, glm::vec3 up, glm::vec4 color = glm::vec4(0, 0, 0, 1), bool insideOut = false);
};

#endif //BESTEST_GAME_MESHFACTORY_H
