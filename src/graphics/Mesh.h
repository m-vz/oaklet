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
    GLuint vertexIndexBuffer, vertexBuffer;
    std::vector<unsigned int> vertexIndices, normalIndices, uvIndices, colorIndices;
    std::vector<float> vertexData;
    std::vector<glm::vec3> normalData, colorData;
    std::vector<glm::vec2> uvData;
    glm::mat4 model;

    explicit Mesh(const std::string &path);
    void bindVertexBuffer();
    void fillVertexBuffer();
    virtual ~Mesh();
};

#endif //BESTEST_GAME_MESH_H
