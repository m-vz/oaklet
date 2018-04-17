//
// Created by Milan van Zanten on 16.04.18.
//

#ifndef BESTEST_GAME_LIGHTINGTECHNIQUE_H
#define BESTEST_GAME_LIGHTINGTECHNIQUE_H

#include <glm/glm.hpp>
#include "Technique.h"

class LightingTechnique : public Technique {
public:
    virtual void setModel(const glm::mat4 &model) = 0;
    virtual void setMVP(const glm::mat4 &mvp)= 0;

protected:
    GLuint modelID;
    GLuint mvpID;
};

#endif //BESTEST_GAME_LIGHTINGTECHNIQUE_H
