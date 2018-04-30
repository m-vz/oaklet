//
// Created by Milan van Zanten on 14.03.18.
//

#ifndef BESTEST_GAME_CIRCLINGCAMERA_H
#define BESTEST_GAME_CIRCLINGCAMERA_H

#include "Camera.h"

class CirclingCamera : public Camera {
public:
    float distance, angle, speed, rotation = 0;

    CirclingCamera(int width, int height,
                   float distance, float angle, float speed = 0.0f,
                   glm::vec3 lookingDirection = glm::vec3(0, 0, 0));
    void changeAspectRatio(int width, int height) override;
    void update(long long int time) override;
};

#endif //BESTEST_GAME_CIRCLINGCAMERA_H
