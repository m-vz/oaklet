//
// Created by Milan van Zanten on 14.03.18.
//

#include <thread>
#include "CirclingCamera.h"

CirclingCamera::CirclingCamera(int width, int height,
                               float distance, float angle, float speed,
                               glm::vec3 lookingAt)
        : distance(distance), angle(angle), speed(speed), lookingAt(lookingAt) {
    changeAspectRatio(width, height);
    update(0);
}

void CirclingCamera::changeAspectRatio(int width, int height) {
    aspectRatio = (float) width / (float) height;

    // projection matrix: 45° field of view, display range : 0.1 unit <-> 100 units
    projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}

void CirclingCamera::update(long long int time) {
    float planeDistance = distance * std::cosf(angle), t = rotation + speed * speed * time/10000000000.0f;
    position = lookingAt - glm::vec3(planeDistance * sinf(t), distance*std::sinf(-angle), planeDistance * cosf(t));

    view = glm::lookAt(
            position,
            lookingAt,
            glm::vec3(0, 1, 0) // head is up
    );
}

const glm::vec3 &CirclingCamera::getPosition() const {
    return position;
}

const glm::vec3 &CirclingCamera::getLookingAt() const {
    return lookingAt;
}
