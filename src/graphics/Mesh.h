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
    GLuint vertexIndexBuffer, vertexBuffer, uvBuffer;
    std::vector<unsigned int> indices;
    std::vector<float> vertexData, normalData, colorData, uvData;
    glm::mat4 model;

    explicit Mesh(const std::string &path);
    void bindVertexBuffer();
    void fillVertexBuffer();
    void bindUVBuffer();
    void fillUVBuffer();
    virtual ~Mesh();

private:
    void calculateIndices();
};

#endif //BESTEST_GAME_MESH_H
