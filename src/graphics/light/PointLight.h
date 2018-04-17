//
// Created by Milan van Zanten on 16.04.18.
//

#ifndef BESTEST_GAME_POINTLIGHT_H
#define BESTEST_GAME_POINTLIGHT_H

#include <glm/glm.hpp>

class PointLight {
public:
    glm::vec3 lightPosition;
    glm::vec3 lightColor;
    float lightPower;
    struct {
        float exponential;
        float linear;
        float constant;
    } attenuation;

    PointLight(glm::vec3 lightPosition,
               glm::vec3 lightColor, float lightPower,
               float constantAttenuation = 0, float linearAttenuation = 0, float exponentialAttenuation = 1);
};

#endif //BESTEST_GAME_POINTLIGHT_H
