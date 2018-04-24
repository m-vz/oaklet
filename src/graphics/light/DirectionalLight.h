//
// Created by Milan van Zanten on 17.04.18.
//

#ifndef BESTEST_GAME_DIRECTIONALLIGHT_H
#define BESTEST_GAME_DIRECTIONALLIGHT_H

#include <glm/glm.hpp>
#include "LightWithShadowMap.h"

class DirectionalLight : public LightWithShadowMap {
public:
    glm::vec3 lightDirection;
    glm::vec3 lightColor;
    float lightPower;

    DirectionalLight(const glm::vec3 &lightDirection, const glm::vec3 &lightColor, float lightPower);
    void calculateVP() override;
};

#endif //BESTEST_GAME_DIRECTIONALLIGHT_H
