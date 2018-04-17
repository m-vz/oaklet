//
// Created by Milan van Zanten on 16.04.18.
//

#ifndef BESTEST_GAME_LIGHTINGTECHNIQUE_H
#define BESTEST_GAME_LIGHTINGTECHNIQUE_H

#include <glm/glm.hpp>
#include "Technique.h"

class LightingTechnique : public Technique {
public:
    virtual void setModel(const glm::mat4 &model);
    virtual void setMVP(const glm::mat4 &mvp);
    virtual void setDiffuseTextureSampler(GLuint textureSampler);
    virtual void setNormalTextureSampler(GLuint textureSampler);
    virtual void setSpecularTextureSampler(GLuint textureSampler);

protected:
    GLuint modelID;
    GLuint mvpID;

    GLuint diffuseTextureSamplerID = 0;
    GLuint normalTextureSamplerID = 0;
    GLuint specularTextureSamplerID = 0;
};

#endif //BESTEST_GAME_LIGHTINGTECHNIQUE_H
