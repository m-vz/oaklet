//
// Created by Milan van Zanten on 22.03.18.
//

#ifndef BESTEST_GAME_MODEL_H
#define BESTEST_GAME_MODEL_H

#include <string>
#include <unordered_map>
#include "Mesh.h"
#include "Texture.h"

class Model {
public:
    glm::mat4 modelMatrix;
    std::vector<Mesh*> meshes;
    std::vector<std::unordered_map<TextureType, Texture*, std::hash<int>>> materials; // TODO: Support for multiple textures per texture type.

    Model();
    void loadModel(const std::string &path);
    void render(GLuint modelMatrixID, GLuint diffuseTextureSamplerID, GLuint normalTextureSamplerID, GLuint specularTextureSamplerID);
    void setTranslation(glm::vec3 translation);
    void translate(glm::vec3 translation);
    void setRotation(float angle, glm::vec3 axis);
    void rotate(float angle, glm::vec3 axis);
    void setScale(glm::vec3 scale);
    void setScale(float scale);
    void scale(glm::vec3 scale);
    void scale(float scale);
    virtual ~Model();

private:
    Texture *blackTexture, *whiteTexture;
    float rotationAngle = 0.0f;
    glm::mat4 translationMatrix, rotationMatrix, scaleMatrix;
    glm::vec3 translation = glm::vec3(0.0f), rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f), scaleVector = glm::vec3(1.0f);

    void calculateModelMatrix();
    void calculateTranslationMatrix();
    void calculateRotationMatrix();
    void calculateScaleMatrix();
};

#endif //BESTEST_GAME_MODEL_H
