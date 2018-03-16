//
// Created by Milan van Zanten on 12.02.18.
//

#ifndef BESTEST_GAME_MESH_H
#define BESTEST_GAME_MESH_H

#include <vector>
#include <string>
#include <glm/ext.hpp>
#include <GL/glew.h>

class Mesh {
public:
    GLuint vertexIndexBuffer, vertexBuffer, uvBuffer, normalBuffer, colorBuffer;
    std::vector<unsigned int> indices;
    std::vector<float> vertexData, normalData, colorData, uvData;
    glm::mat4 model;

    explicit Mesh(const std::string &path);
    void bindVertexBuffer();
    void fillVertexBuffer();
    void bindUVBuffer();
    void fillUVBuffer();
    void bindNormalBuffer();
    void fillNormalBuffer();
    void bindColorBuffer();
    void fillColorBuffer();
    void setTranslation(glm::vec3 translation);
    void translate(glm::vec3 translation);
    void setRotation(float angle, glm::vec3 axis);
    void rotate(float angle, glm::vec3 axis);
    void setScale(glm::vec3 scale);
    void setScale(float scale);
    void scale(glm::vec3 scale);
    void scale(float scale);
    virtual ~Mesh();

private:
    glm::mat4 translationMatrix, rotationMatrix, scaleMatrix;
    glm::vec3 translation = glm::vec3(0.0f), rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f), scaleVector = glm::vec3(1.0f);
    float rotationAngle = 0.0f;
    void calculateIndices();
    void calculateModelMatrix();
    void calculateTranslationMatrix();
    void calculateRotationMatrix();
    void calculateScaleMatrix();
};

#endif //BESTEST_GAME_MESH_H
