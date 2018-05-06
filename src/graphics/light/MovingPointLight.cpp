//
// Created by Milan van Zanten on 06.05.18.
//

#include "MovingPointLight.h"
#include "../../util/Log.h"

MovingPointLight::MovingPointLight(const glm::vec3 &lightPosition,
                                   const glm::vec3 &lightColor, float lightPower,
                                   float exponentialAttenuation, float linearAttenuation, float constantAttenuation)
        : PointLight(lightPosition,
                     lightColor, lightPower,
                     exponentialAttenuation, linearAttenuation, constantAttenuation) {
    center = lightPosition;
    update();
}

void MovingPointLight::update(Time &time) {
    age = time.timeSinceEpoch()/1000000;
    update();
}

void MovingPointLight::setLightPosition(const glm::vec3 &lightPosition) {
    center = lightPosition;
}

const glm::vec3 &MovingPointLight::getCenter() const {
    return center;
}

void MovingPointLight::update() {
    lightPosition = center + glm::vec3(3, 0, 0)*std::cosf(age/600.0f) + glm::vec3(0, 0, 3)*std::sinf(age/600.0f);
}
