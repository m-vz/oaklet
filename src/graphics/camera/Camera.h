//
// Created by Milan van Zanten on 13.03.18.
//

#ifndef BESTEST_GAME_CAMERA_H
#define BESTEST_GAME_CAMERA_H

#include <glm/ext.hpp>

class Camera {
public:
    virtual void changeAspect(int width, int height) = 0;
    const glm::mat4 &getView() const;
    const glm::mat4 &getProjection() const;

protected:
    glm::mat4 view, projection;
};

#endif //BESTEST_GAME_CAMERA_H
