//
// Created by Milan van Zanten on 16.04.18.
//

#define GLM_ENABLE_EXPERIMENTAL

#include "PointLight.h"

PointLight::PointLight(glm::vec3 lightPosition,
                       glm::vec3 lightColor, float lightPower,
                       float exponentialAttenuation, float linearAttenuation, float constantAttenuation)
        : lightColor(lightColor), lightPower(lightPower) {
    this->lightPosition = lightPosition;
    this->attenuation = {
        exponentialAttenuation,
        linearAttenuation,
        constantAttenuation
    };
    near = 0.5f;
    far = 25.0f;

    depthMapIsCube = true;

    views.resize(6);
    vps.resize(6);
    calculateVP();
}

void PointLight::calculateVP() {
    views[0] = glm::lookAt(lightPosition, lightPosition + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0));
    views[1] = glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0));
    views[2] = glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
    views[3] = glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0));
    views[4] = glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0));
    views[5] = glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0));
    projection = glm::perspective(glm::radians(90.0f), 1.0f, near, far);
    vps[0] = projection*views[0];
    vps[1] = projection*views[1];
    vps[2] = projection*views[2];
    vps[3] = projection*views[3];
    vps[4] = projection*views[4];
    vps[5] = projection*views[5];
}
