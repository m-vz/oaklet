//
// Created by Milan van Zanten on 12.02.18.
//

#define GLM_ENABLE_EXPERIMENTAL

#include <tiny_obj_loader.h>
#include "Mesh.h"
#include "../io/FileLoader.h"
#include "../exception/Exception.h"

Mesh::Mesh(const std::string &path) {
    calculateTranslationMatrix();
    calculateRotationMatrix();
    calculateScaleMatrix();
    calculateModelMatrix();

    FileLoader::loadOBJ(path, vertexData, normalData, uvData, colorData);
    calculateIndices();

    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &vertexIndexBuffer);
    glGenBuffers(1, &uvBuffer);
    glGenBuffers(1, &normalBuffer);
    glGenBuffers(1, &colorBuffer);
}

void Mesh::calculateIndices() {
    // TODO: actually calculate useful indices to reduce data amount.
    for(unsigned int i = 0; i < vertexData.size()/3; ++i)
        indices.push_back(i);
}

void Mesh::calculateModelMatrix() {
    model = translationMatrix * rotationMatrix * scaleMatrix;
}

void Mesh::calculateTranslationMatrix() {
    translationMatrix = glm::translate<glm::highp_float, glm::highp>(translation);
}

void Mesh::calculateRotationMatrix() {
    rotationMatrix = glm::rotate<glm::highp_float, glm::highp>(rotationAngle, rotationAxis);
}

void Mesh::calculateScaleMatrix() {
    scaleMatrix = glm::scale<glm::highp_float, glm::highp>(scaleVector);
}

void Mesh::bindVertexBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer);
}

void Mesh::fillVertexBuffer() {
    if(vertexData.empty())
        throw Exception("Vertex data is empty. Cannot fill vertex buffer."); // NOLINT


    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void Mesh::bindUVBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
}

void Mesh::fillUVBuffer() {
    if(uvData.empty())
        throw Exception("UV data is empty. Cannot fill uv buffer."); // NOLINT


    glBufferData(GL_ARRAY_BUFFER, uvData.size() * sizeof(float), &uvData[0], GL_STATIC_DRAW);
}

void Mesh::bindNormalBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
}

void Mesh::fillNormalBuffer() {
    if(normalData.empty())
        throw Exception("Normal data is empty. Cannot fill normal buffer."); // NOLINT

    glBufferData(GL_ARRAY_BUFFER, normalData.size() * sizeof(float), &normalData[0], GL_STATIC_DRAW);
}

void Mesh::bindColorBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
}

void Mesh::fillColorBuffer() {
    if(colorData.empty())
        throw Exception("Color data is empty. Cannot fill color buffer."); // NOLINT


    glBufferData(GL_ARRAY_BUFFER, colorData.size() * sizeof(float), &colorData[0], GL_STATIC_DRAW);
}

void Mesh::setTranslation(glm::vec3 translation) {
    this->translation = translation;

    calculateTranslationMatrix();
    calculateModelMatrix();
}

void Mesh::translate(glm::vec3 translation) {
    this->translation += translation;

    calculateTranslationMatrix();
    calculateModelMatrix();
}

void Mesh::setRotation(float angle, glm::vec3 axis) {
    this->rotationAngle = angle;
    this->rotationAxis = axis;

    calculateRotationMatrix();
    calculateModelMatrix();
}

void Mesh::rotate(float angle, glm::vec3 axis) {
    this->rotationAngle += angle;
    this->rotationAxis = axis;

    calculateRotationMatrix();
    calculateModelMatrix();
}

void Mesh::setScale(glm::vec3 scale) {
    this->scaleVector = scale;

    calculateScaleMatrix();
    calculateModelMatrix();
}

void Mesh::setScale(float scale) {
    this->scaleVector = glm::vec3(scale);

    calculateScaleMatrix();
    calculateModelMatrix();
}

void Mesh::scale(glm::vec3 scale) {
    this->scaleVector *= scale;

    calculateScaleMatrix();
    calculateModelMatrix();
}

void Mesh::scale(float scale) {
    this->scaleVector *= glm::vec3(scale);

    calculateScaleMatrix();
    calculateModelMatrix();
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &vertexIndexBuffer);
    glDeleteBuffers(1, &uvBuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteBuffers(1, &colorBuffer);
}
