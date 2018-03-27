//
// Created by Milan van Zanten on 12.02.18.
//

#include <tiny_obj_loader.h>
#include "Mesh.h"
#include "../../io/FileLoader.h"
#include "../../exception/Exception.h"

Mesh::Mesh() {
    generateBuffers(&vertexBuffer);
    generateBuffers(&indexBuffer);
    generateBuffers(&uvBuffer);
    generateBuffers(&normalBuffer);
    generateBuffers(&tangentBuffer);
    generateBuffers(&colorBuffer);
}

void Mesh::initMesh(const aiMesh *aiMesh) {
    materialIndex = aiMesh->mMaterialIndex;

    /* vertex data */
    aiVector3D *v;
    aiColor4D *c;
    for(int i = 0; i < aiMesh->mNumVertices; ++i) {
        v = &aiMesh->mVertices[i];
        vertexData.push_back(v->x);
        vertexData.push_back(v->y);
        vertexData.push_back(v->z);

        if(aiMesh->HasNormals()) {
            v = &aiMesh->mNormals[i];
            normalData.push_back(v->x);
            normalData.push_back(v->y);
            normalData.push_back(v->z);
        }

        if(aiMesh->HasTangentsAndBitangents()) {
            v = &aiMesh->mTangents[i];
            tangentData.push_back(v->x);
            tangentData.push_back(v->y);
            tangentData.push_back(v->z);
        }

        // TODO: Make multiple vertex color sets possible?
        if(aiMesh->HasVertexColors(0)) {
            c = aiMesh->mColors[i];
            colorData.push_back(c->r);
            colorData.push_back(c->g);
            colorData.push_back(c->b);
            colorData.push_back(c->a);
        }

        // TODO: Make multiple texture coordinate sets possible.
        if(aiMesh->HasTextureCoords(0)) {
            v = &aiMesh->mTextureCoords[0][i];
            uvData.push_back(v->x);
            uvData.push_back(v->y);
        }
    }

    /* index data */
    aiFace *face;
    for(int i = 0; i < aiMesh->mNumFaces; ++i) {
        face = &aiMesh->mFaces[i];

        if(face->mNumIndices != 3) // expect mesh to be triangulated
            throw Exception("Mesh was not triangulated correctly."); // NOLINT

        indices.push_back(face->mIndices[0]);
        indices.push_back(face->mIndices[1]);
        indices.push_back(face->mIndices[2]);
    }

    /* fill buffers */
    bindBuffer(vertexBuffer);
    fillBuffer(&vertexData);
    bindBuffer(indexBuffer, GL_ELEMENT_ARRAY_BUFFER);
    fillBuffer(&indices, GL_ELEMENT_ARRAY_BUFFER);

    if(aiMesh->HasTextureCoords(0)) {
        bindBuffer(uvBuffer);
        fillBuffer(&uvData);
    }

    if(aiMesh->HasNormals()) {
        bindBuffer(normalBuffer);
        fillBuffer(&normalData);
    }

    if(aiMesh->HasTangentsAndBitangents()) {
        bindBuffer(tangentBuffer);
        fillBuffer(&tangentData);
    }

    if(aiMesh->HasVertexColors(0)) {
        bindBuffer(colorBuffer);
        fillBuffer(&colorData);
    }
}

void Mesh::generateBuffers(GLuint *buffers, int number) {
    glGenBuffers(number, buffers);
}

void Mesh::bindBuffer(GLuint buffer, GLenum type) {
    glBindBuffer(type, buffer);
}

template<typename T>
void Mesh::fillBuffer(std::vector<T> *data, GLenum target, GLenum usage) {
    if(data->empty())
        throw Exception("Data is empty, cannot fill buffer."); // NOLINT

    glBufferData(target, data->size() * sizeof(T), &(*data)[0], usage);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);
    glDeleteBuffers(1, &uvBuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteBuffers(1, &tangentBuffer);
    glDeleteBuffers(1, &colorBuffer);
}
