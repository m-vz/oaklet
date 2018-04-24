//
// Created by Milan van Zanten on 16.04.18.
//

#include "PointLight.h"

PointLight::PointLight(glm::vec3 lightPosition,
                       glm::vec3 lightColor, float lightPower,
                       float exponentialAttenuation, float linearAttenuation, float constantAttenuation)
        : lightPosition(lightPosition), lightColor(lightColor), lightPower(lightPower) {
    attenuation = {
        exponentialAttenuation,
        linearAttenuation,
        constantAttenuation
    };
}
