//
// Created by Milan van Zanten on 14.03.18.
//

#ifndef BESTEST_GAME_CIRCLINGCAMERA_H
#define BESTEST_GAME_CIRCLINGCAMERA_H

#include "Camera.h"

class CirclingCamera : public Camera {
public:
    float distance, angle, speed, rotation = 0;
    glm::vec3 lookingAt;

    CirclingCamera(int width, int height,
                   float distance, float angle, float speed = 1.0f,
                   glm::vec3 lookingAt = glm::vec3(0, 0, 0));
    void changeAspect(int width, int height) override;
    void update(long long int time);

private:
    glm::vec3 position;
};

#endif //BESTEST_GAME_CIRCLINGCAMERA_H
