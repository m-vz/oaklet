//
// Created by Milan van Zanten on 12.02.18.
//

#ifndef BESTEST_GAME_MESH_H
#define BESTEST_GAME_MESH_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <assimp/mesh.h>

class Mesh {
friend class Model;

public:
    GLuint indexBuffer, vertexBuffer, uvBuffer, normalBuffer, tangentBuffer, colorBuffer;
    std::vector<unsigned int> indices;
    std::vector<float> vertexData, normalData, tangentData, colorData, uvData;
    int materialIndex;

    Mesh();
    void initMesh(const aiMesh* aiMesh);
    void generateBuffers(GLuint *buffers, int number = 1);
    void bindBuffer(GLuint buffer, GLenum type = GL_ARRAY_BUFFER);
    template<typename T> void fillBuffer(std::vector<T> *data, GLenum target = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_DRAW);
    virtual ~Mesh();
};

#endif //BESTEST_GAME_MESH_H
