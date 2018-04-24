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
friend class MeshFactory;

public:
    GLuint indexBuffer, vertexBuffer, uvBuffer, normalBuffer, tangentBuffer, colorBuffer;
    std::vector<unsigned int> indices;
    /// Data structure where every three numbers represent a vector (x, y, z)
    std::vector<float> vertexData, normalData, tangentData, uvData;
    /// Color data structure where every four numbers represent a color (r, g, b, a)
    std::vector<float> colorData;
    long materialIndex = -1;

    Mesh();
    void initFromAIMesh(const aiMesh *aiMesh, int materialIndexOffset);
    void generateBuffers(GLuint *buffers, int number = 1);
    void bindBuffer(GLuint buffer, GLenum type = GL_ARRAY_BUFFER);
    template<typename T> void fillBuffer(std::vector<T> *data, GLenum target = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_DRAW);
    bool containsNormalData() const;
    bool containsTangentData() const;
    bool containsColorData() const;
    virtual ~Mesh();

private:
    bool hasNormalData = false, hasTangentData = false, hasColorData = false;

    void fillBuffers(bool fillUVBuffer = true, bool fillNormalBuffer = true, bool fillTangentBuffer = true, bool fillColorBuffer = true);
};

#endif //BESTEST_GAME_MESH_H
