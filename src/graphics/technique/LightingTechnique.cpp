//
// Created by Milan van Zanten on 16.04.18.
//

#include "LightingTechnique.h"

void LightingTechnique::setModel(const glm::mat4 &model) {
    glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
}

void LightingTechnique::setMVP(const glm::mat4 &mvp) {
    glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
}

void LightingTechnique::setDiffuseTextureSampler(GLuint textureSampler) {
    glUniform1i(diffuseTextureSamplerID, textureSampler);
}

void LightingTechnique::setNormalTextureSampler(GLuint textureSampler) {
    glUniform1i(normalTextureSamplerID, textureSampler);
}

void LightingTechnique::setSpecularTextureSampler(GLuint textureSampler) {
    glUniform1i(specularTextureSamplerID, textureSampler);
}
