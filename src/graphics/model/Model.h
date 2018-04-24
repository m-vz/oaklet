//
// Created by Milan van Zanten on 22.03.18.
//

#ifndef BESTEST_GAME_MODEL_H
#define BESTEST_GAME_MODEL_H

#include <string>
#include <unordered_map>
#include "Mesh.h"
#include "../texture/Texture.h"

class Model {
public:
    TextureType TEXTURES_TO_LOAD[3] = {TEXTURE_DIFFUSE, TEXTURE_NORMAL, TEXTURE_SPECULAR};

    glm::mat4 modelMatrix;
    std::vector<Mesh*> meshes;
    std::vector<std::unordered_map<TextureType, Texture*, std::hash<int>>> materials; // TODO: Support for multiple textures per texture type.

    Model();
    void loadModel(const std::string &path);
    void setMeshTexture(int meshIndex, TextureType type, Texture *texture);
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
    /// Textures that were created with this model and need to be deleted in the destructor.
    std::vector<Texture*> texturesToDelete;
    float rotationAngle = 0.0f;
    glm::mat4 translationMatrix, rotationMatrix, scaleMatrix;
    glm::vec3 translation = glm::vec3(0.0f), rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f), scaleVector = glm::vec3(1.0f);

    void calculateModelMatrix();
    void calculateTranslationMatrix();
    void calculateRotationMatrix();
    void calculateScaleMatrix();
};

#endif //BESTEST_GAME_MODEL_H
