//
// Created by Milan van Zanten on 22.03.18.
//

#define GLM_ENABLE_EXPERIMENTAL

#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <glm/ext.hpp>
#include "Model.h"
#include "../../io/FileLoader.h"

Model::Model() {
    calculateTranslationMatrix();
    calculateRotationMatrix();
    calculateScaleMatrix();
    calculateModelMatrix();
}

void Model::loadModel(const std::string &path) {
    Assimp::Importer importer;
    const aiScene *aiScene = FileLoader::loadModel(importer, path);

    // fetch and create meshes from the scene
    meshes.resize(aiScene->mNumMeshes);
    for(int i = 0; i < aiScene->mNumMeshes; i++) {
        auto *mesh = new Mesh();
        mesh->initMesh(aiScene->mMeshes[i]);
        meshes[i] = mesh;
    }

    // fetch materials from the scene
    materials.resize(aiScene->mNumMaterials);
    for(int i = 0; i < aiScene->mNumMaterials; ++i) {
        const aiMaterial *aiMaterial = aiScene->mMaterials[i];

        // search textures for all supported texture types
        for(int j = 0; j < TEXTURE_MAX; ++j) {
            aiTextureType aiType = Texture::textureTypeToAITextureType(static_cast<TextureType>(j));

            if(aiMaterial->GetTextureCount(aiType) > 0) { // the material contains a texture for the current type
                aiString texturePath;

                if(aiMaterial->GetTexture(aiType, 0, &texturePath) == AI_SUCCESS) { // the texture was successfully fetched, create the texture
                    Texture *texture;
                    std::string texturePathString = texturePath.data;
                    int offset = 0;
                    while(texturePathString[offset] == '.' || texturePathString[offset] == '\\' || texturePathString[offset] == '/')
                        offset++;

                    texture = new Texture(
                            path.substr(0, path.find_last_of("\\/") + 1) +
                            texturePathString.substr(static_cast<unsigned long>(offset))
                    );
                    texture->bindTexture(0);
                    texture->fillTexture();
                    materials[i][static_cast<TextureType>(j)] = texture;
                }
            }
        }
    }
}

void Model::render(GLuint modelMatrixID, GLuint diffuseTextureSamplerID, GLuint normalTextureSamplerID, GLuint specularTextureSamplerID) {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    for(auto mesh: meshes) {
        std::unordered_map<TextureType, Texture*, std::hash<int>> &material = materials[mesh->materialIndex];

        mesh->bindBuffer(mesh->vertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // NOLINT
        mesh->bindBuffer(mesh->uvBuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); // NOLINT
        mesh->bindBuffer(mesh->normalBuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0); // NOLINT
        mesh->bindBuffer(mesh->tangentBuffer);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0); // NOLINT

        mesh->bindBuffer(mesh->indexBuffer, GL_ELEMENT_ARRAY_BUFFER);

        if(material.find(TEXTURE_DIFFUSE) != material.end()) {
            material[TEXTURE_DIFFUSE]->bindTexture(TEXTURE_DIFFUSE);
            glUniform1i(diffuseTextureSamplerID, TEXTURE_DIFFUSE);
        }
        if(material.find(TEXTURE_NORMAL) != material.end()) {
            material[TEXTURE_NORMAL]->bindTexture(TEXTURE_NORMAL);
            glUniform1i(normalTextureSamplerID, TEXTURE_NORMAL);
        }
        if(material.find(TEXTURE_SPECULAR) != material.end()) {
            material[TEXTURE_SPECULAR]->bindTexture(TEXTURE_SPECULAR);
            glUniform1i(specularTextureSamplerID, TEXTURE_SPECULAR);
        }
        glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->indices.size()), GL_UNSIGNED_INT, 0); // NOLINT
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}

void Model::setTranslation(glm::vec3 translation) {
    translation = translation;

    calculateTranslationMatrix();
    calculateModelMatrix();
}

void Model::translate(glm::vec3 translation) {
    translation += translation;

    calculateTranslationMatrix();
    calculateModelMatrix();
}

void Model::setRotation(float angle, glm::vec3 axis) {
    rotationAngle = angle;
    rotationAxis = axis;

    calculateRotationMatrix();
    calculateModelMatrix();
}

void Model::rotate(float angle, glm::vec3 axis) {
    rotationAngle += angle;
    rotationAxis = axis;

    calculateRotationMatrix();
    calculateModelMatrix();
}

void Model::setScale(glm::vec3 scale) {
    scaleVector = scale;

    calculateScaleMatrix();
    calculateModelMatrix();
}

void Model::setScale(float scale) {
    scaleVector = glm::vec3(scale);

    calculateScaleMatrix();
    calculateModelMatrix();
}

void Model::scale(glm::vec3 scale) {
    scaleVector *= scale;

    calculateScaleMatrix();
    calculateModelMatrix();
}

void Model::scale(float scale) {
    scaleVector *= glm::vec3(scale);

    calculateScaleMatrix();
    calculateModelMatrix();
}

Model::~Model() {
    for(auto mesh: meshes)
        delete mesh;
    for(auto material: materials)
        for(int i = 0; i < TEXTURE_MAX; ++i)
            delete material[static_cast<TextureType>(i)];
}

void Model::calculateModelMatrix() {
    modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

void Model::calculateTranslationMatrix() {
    translationMatrix = glm::translate(translation);
}

void Model::calculateRotationMatrix() {
    rotationMatrix = glm::rotate(rotationAngle, rotationAxis);
}

void Model::calculateScaleMatrix() {
    scaleMatrix = glm::scale(scaleVector);
}
