//
// Created by Milan van Zanten on 06.05.18.
//

#ifndef BESTEST_GAME_MOVINGPOINTLIGHT_H
#define BESTEST_GAME_MOVINGPOINTLIGHT_H

#include "PointLight.h"
#include "../../world/entity/UpdateEntity.h"

class MovingPointLight : public PointLight, public UpdateEntity {
public:
    MovingPointLight(const glm::vec3 &lightPosition,
                     const glm::vec3 &lightColor, float lightPower,
                     float exponentialAttenuation = DEFAULT_ATTENUATION_EXPONENTIAL,
                     float linearAttenuation = DEFAULT_ATTENUATION_LINEAR,
                     float constantAttenuation = DEFAULT_ATTENUATION_CONSTANT);
    void update(Time &time) override;
    void setLightPosition(const glm::vec3 &lightPosition) override;
    const glm::vec3 &getCenter() const;

private:
    long long int age = 0;
    glm::vec3 center;

    void update();
};

#endif //BESTEST_GAME_MOVINGPOINTLIGHT_H
