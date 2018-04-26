//
// Created by Milan van Zanten on 16.04.18.
//

#ifndef BESTEST_GAME_POINTLIGHT_H
#define BESTEST_GAME_POINTLIGHT_H

#include <glm/glm.hpp>

class PointLight {
public:
    static constexpr float DEFAULT_ATTENUATION_EXPONENTIAL = 1;
    static constexpr float DEFAULT_ATTENUATION_LINEAR = 0;
    static constexpr float DEFAULT_ATTENUATION_CONSTANT = 1;

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
               float exponentialAttenuation = DEFAULT_ATTENUATION_EXPONENTIAL,
               float linearAttenuation = DEFAULT_ATTENUATION_LINEAR,
               float constantAttenuation = DEFAULT_ATTENUATION_CONSTANT);
};

#endif //BESTEST_GAME_POINTLIGHT_H
