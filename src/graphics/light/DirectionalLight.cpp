//
// Created by Milan van Zanten on 17.04.18.
//

#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3 &lightDirection, const glm::vec3 &lightColor, float lightPower)
        : lightColor(lightColor), lightPower(lightPower) {
    this->lightDirection = glm::normalize(lightDirection);
}
