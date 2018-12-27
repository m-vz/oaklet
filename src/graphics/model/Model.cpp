//
// Created by Milan van Zanten on 22.03.18.
//

#define GLM_ENABLE_EXPERIMENTAL

#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <glm/ext.hpp>
#include "Model.h"
#include "../../io/FileLoader.h"
#include "../../exception/Exception.h"

Model::Model() {
    calculateTranslationMatrix();
    calculateRotationMatrix();
    calculateScaleMatrix();
    calculateModelMatrix();
}

void Model::loadModel(const std::string &path) {
    unsigned long oldSize;
    Assimp::Importer importer;
    const aiScene *aiScene = FileLoader::loadModel(importer, path);

    // fetch and create meshes from the scene
    oldSize = meshes.size();
    meshes.resize(oldSize + aiScene->mNumMeshes);
    for(unsigned long i = oldSize; i < meshes.size(); i++) {
        auto *mesh = new Mesh();
        mesh->initFromAIMesh(aiScene->mMeshes[i - oldSize], static_cast<int>(materials.size()));
        meshes[i] = mesh;
    }

    // fetch materials from the scene
    oldSize = materials.size();
    materials.resize(oldSize + aiScene->mNumMaterials);
    for(unsigned long i = oldSize; i < materials.size(); ++i) {
        const aiMaterial *aiMaterial = aiScene->mMaterials[i - oldSize];

        // search textures for all supported texture types
        for(TextureType type: TEXTURES_TO_LOAD) {
            aiTextureType aiType = Texture::textureTypeToAITextureType(type);

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
                            texturePathString.substr(static_cast<unsigned long>(offset)),
                            (type == TEXTURE_DIFFUSE) // only convert diffuse textures to linear space
                    );
                    texture->fillTexture(true, true, true, GL_CLAMP_TO_EDGE);
                    materials[i][type] = texture;
                    texturesToDelete.push_back(texture);
                }
            }
        }
    }
}

void Model::setMeshTexture(int meshIndex, TextureType type, Texture *texture) {
    if(meshIndex >= meshes.size())
        throw Exception("Mesh index out of bounds.");

    Mesh *addTo = meshes[meshIndex];

    if(addTo->materialIndex < 0) {
        addTo->materialIndex = materials.size();
        materials.resize(static_cast<unsigned long>(addTo->materialIndex) + 1);
    }

    materials[addTo->materialIndex][type] = texture;
}

void Model::setTranslation(glm::vec3 translation) {
    this->translation = translation;

    calculateTranslationMatrix();
    calculateModelMatrix();
}

void Model::translate(glm::vec3 translation) {
    this->translation += translation;

    calculateTranslationMatrix();
    calculateModelMatrix();
}

void Model::setRotation(glm::vec3 angles) {
    rotation = angles;

    calculateRotationMatrix();
    calculateModelMatrix();
}

void Model::rotate(glm::vec3 angles) {
    rotation += angles;

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

const glm::vec3 &Model::getTranslation() const {
    return translation;
}

const glm::vec3 &Model::getRotation() const {
    return rotation;
}

const glm::vec3 &Model::getScale() const {
    return scaleVector;
}

Model::~Model() {
    for(auto texture: texturesToDelete)
        delete texture;
    for(auto mesh: meshes)
        delete mesh;
}

void Model::calculateModelMatrix() {
    modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

void Model::calculateTranslationMatrix() {
    translationMatrix = glm::translate(translation);
}

void Model::calculateRotationMatrix() {
    rotationMatrix = glm::rotate(glm::rotate(glm::rotate(rotation.x, glm::vec3(1, 0, 0)), rotation.y, glm::vec3(0, 1, 0)), rotation.z, glm::vec3(0, 0, 1));
}

void Model::calculateScaleMatrix() {
    scaleMatrix = glm::scale(scaleVector);
}
