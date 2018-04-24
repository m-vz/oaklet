//
// Created by Milan van Zanten on 17.04.18.
//

#ifndef BESTEST_GAME_SPOTLIGHT_H
#define BESTEST_GAME_SPOTLIGHT_H

#include <GL/glew.h>
#include "PointLight.h"
#include "../texture/Texture.h"
#include "LightWithShadowMap.h"

class SpotLight : public LightWithShadowMap {
public:
    static constexpr float DEFAULT_ATTENUATION_EXPONENTIAL = 1;
    static constexpr float DEFAULT_ATTENUATION_LINEAR = 0;
    static constexpr float DEFAULT_ATTENUATION_CONSTANT = 0;

    glm::vec3 lightPosition;
    glm::vec3 lightDirection;
    glm::vec3 lightColor;
    float lightPower;
    float lightCutoff;
    struct {
        float exponential;
        float linear;
        float constant;
    } attenuation;

    SpotLight(const glm::vec3 &lightPosition, const glm::vec3 &lightDirection,
              const glm::vec3 &lightColor, float lightPower, float lightCutoff,
              float exponentialAttenuation = DEFAULT_ATTENUATION_EXPONENTIAL,
              float linearAttenuation = DEFAULT_ATTENUATION_LINEAR,
              float constantAttenuation = DEFAULT_ATTENUATION_CONSTANT);
    void calculateVP() override;
};

#endif //BESTEST_GAME_SPOTLIGHT_H
