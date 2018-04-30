//
// Created by Milan van Zanten on 13.03.18.
//

#include "Camera.h"

const glm::mat4 &Camera::getView() const {
    return view;
}

const glm::mat4 &Camera::getProjection() const {
    return projection;
}

float Camera::getAspectRatio() const {
    return aspectRatio;
}

const glm::vec3 &Camera::getPosition() const {
    return position;
}

const glm::vec3 &Camera::getLookingDirection() const {
    return lookingDirection;
}
