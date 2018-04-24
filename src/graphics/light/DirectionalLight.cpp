//
// Created by Milan van Zanten on 17.04.18.
//

#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3 &lightDirection, const glm::vec3 &lightColor, float lightPower)
        : lightColor(lightColor), lightPower(lightPower) {
    this->lightDirection = glm::normalize(lightDirection);
}

void DirectionalLight::calculateVP() {
    glm::vec3 position = 10.0f*this->lightDirection;
    view = glm::lookAt(position, position - lightDirection, glm::vec3(0, 1, 0));
    projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 20.0f);
    vp = projection*view;
}
