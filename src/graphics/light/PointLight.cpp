//
// Created by Milan van Zanten on 16.04.18.
//

#include "PointLight.h"

PointLight::PointLight(glm::vec3 lightPosition,
                       glm::vec3 lightColor, float lightPower,
                       float constantAttenuation, float linearAttenuation, float exponentialAttenuation)
        : lightPosition(lightPosition), lightColor(lightColor), lightPower(lightPower) {
    attenuation = {
        exponentialAttenuation,
        linearAttenuation,
        constantAttenuation
    };
}
