//
// Created by Milan van Zanten on 13.03.18.
//

#ifndef BESTEST_GAME_CAMERA_H
#define BESTEST_GAME_CAMERA_H

#include <glm/ext.hpp>

class Camera {
public:
    virtual void update(long long int time) = 0;
    virtual void changeAspectRatio(int width, int height) = 0;
    const glm::mat4 &getView() const;
    const glm::mat4 &getProjection() const;
    float getAspectRatio() const;
    const glm::vec3 &getPosition() const;
    const glm::vec3 &getLookingDirection() const;

protected:
    float aspectRatio = 1;
    glm::mat4 view, projection;
    glm::vec3 position, lookingDirection;
};

#endif //BESTEST_GAME_CAMERA_H
