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
