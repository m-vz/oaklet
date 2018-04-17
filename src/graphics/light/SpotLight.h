//
// Created by Milan van Zanten on 17.04.18.
//

#ifndef BESTEST_GAME_SPOTLIGHT_H
#define BESTEST_GAME_SPOTLIGHT_H

#include <glm/glm.hpp>
#include "PointLight.h"

class SpotLight : public PointLight {
public:
    glm::vec3 lightDirection;
    float lightCutoff;

    SpotLight(const glm::vec3 &lightPosition, const glm::vec3 &lightDirection,
              const glm::vec3 &lightColor, float lightPower, float lightCutoff,
              float constantAttenuation = 0, float linearAttenuation = 0, float exponentialAttenuation = 1);
};

#endif //BESTEST_GAME_SPOTLIGHT_H
