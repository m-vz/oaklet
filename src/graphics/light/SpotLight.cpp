//
// Created by Milan van Zanten on 17.04.18.
//

#include "SpotLight.h"

SpotLight::SpotLight(const glm::vec3 &lightPosition, const glm::vec3 &lightDirection,
                     const glm::vec3 &lightColor, float lightPower, float lightCutoff,
                     float constantAttenuation, float linearAttenuation, float exponentialAttenuation)
        : PointLight(lightPosition, lightColor, lightPower, constantAttenuation, linearAttenuation, exponentialAttenuation), lightCutoff(lightCutoff) {
    this->lightDirection = glm::normalize(lightDirection);
}
