//
// Created by Milan van Zanten on 12.02.18.
//

#include <tiny_obj_loader.h>
#include "Mesh.h"
#include "../io/FileLoader.h"

Mesh::Mesh(const std::string &path) {
    model = glm::mat4(1.0f); // display at (0,0,0) for now

    FileLoader::loadOBJ(path, vertexData, normalData, uvData, colorData);
    calculateIndices();

    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &vertexIndexBuffer);
    glGenBuffers(1, &uvBuffer);
}

void Mesh::calculateIndices() {
    // TODO: actually calculate useful indices to reduce data amount.
    for(unsigned int i = 0; i < vertexData.size()/3; ++i)
        indices.push_back(i);
}

void Mesh::bindVertexBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer);
}

void Mesh::fillVertexBuffer() {
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void Mesh::bindUVBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
}

void Mesh::fillUVBuffer() {
    glBufferData(GL_ARRAY_BUFFER, uvData.size() * sizeof(float), &uvData[0], GL_STATIC_DRAW);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &vertexIndexBuffer);
    glDeleteBuffers(1, &uvBuffer);
}
