//
// Created by Milan van Zanten on 17.04.18.
//

#define GLM_ENABLE_EXPERIMENTAL

#include "SpotLight.h"

SpotLight::SpotLight(const glm::vec3 &lightPosition, const glm::vec3 &lightDirection,
                     const glm::vec3 &lightColor, float lightPower, float lightCutoff,
                     float exponentialAttenuation, float linearAttenuation, float constantAttenuation)
        : lightPosition(lightPosition), lightColor(lightColor), lightPower(lightPower) {
    this->lightDirection = glm::normalize(lightDirection);
    this->lightCutoff = cosf(lightCutoff);
    attenuation = {
            exponentialAttenuation,
            linearAttenuation,
            constantAttenuation
    };

    calculateVP();
}

void SpotLight::calculateVP() {
    view = glm::lookAt(lightPosition, lightPosition + lightDirection, glm::vec3(0, 1, 0));
    projection = glm::perspective(acosf(lightCutoff)*2, 1.0f, 1.0f, 100.0f);
    vp = projection*view;
}