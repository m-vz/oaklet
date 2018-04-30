//
// Created by Milan van Zanten on 17.04.18.
//

#define GLM_ENABLE_EXPERIMENTAL

#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3 &lightDirection, const glm::vec3 &lightColor, float lightPower)
        : lightColor(lightColor), lightPower(lightPower) {
    this->lightDirection = glm::normalize(lightDirection);
    this->lightPosition = 10.0f*lightDirection; // TODO: this is very hacky.
    near = 0.1f;
    far = 50.0f;

    views.resize(1);
    vps.resize(1);
    calculateVP();
}

void DirectionalLight::calculateVP() {
    views[0] = glm::lookAt(lightPosition, lightPosition - lightDirection, glm::vec3(0, 1, 0));
    projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near, far);
    vps[0] = projection*views[0];
}
