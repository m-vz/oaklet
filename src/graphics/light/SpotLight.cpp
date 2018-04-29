//
// Created by Milan van Zanten on 17.04.18.
//

#define GLM_ENABLE_EXPERIMENTAL

#include "SpotLight.h"

SpotLight::SpotLight(const glm::vec3 &lightPosition, const glm::vec3 &lightDirection,
                     const glm::vec3 &lightColor, float lightPower, float lightCutoff,
                     float exponentialAttenuation, float linearAttenuation, float constantAttenuation)
        : lightColor(lightColor), lightPower(lightPower) {
    this->lightDirection = glm::normalize(lightDirection);
    this->lightPosition = lightPosition;
    this->lightCutoff = cosf(lightCutoff);
    attenuation = {
            exponentialAttenuation,
            linearAttenuation,
            constantAttenuation
    };
    near = 1.0f;
    far = 100.0f;

    views.resize(1);
    vps.resize(1);
    calculateVP();
}

void SpotLight::calculateVP() {
    views[0] = glm::lookAt(lightPosition, lightPosition + lightDirection, glm::vec3(0, 1, 0));
    projection = glm::perspective(acosf(lightCutoff)*2, 1.0f, near, far);
    vps[0] = projection*views[0];
}