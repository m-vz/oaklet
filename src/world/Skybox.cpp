//
// Created by Milan van Zanten on 25.04.18.
//

#include "Skybox.h"
#include "../graphics/model/MeshFactory.h"

Skybox::Skybox(std::string texturesPath, std::string fileExtension) {
    if(texturesPath[texturesPath.size() - 1] != '/')
        texturesPath += "/";
    std::vector<std::string> texturePaths{
        texturesPath + "right" + fileExtension,
        texturesPath + "left" + fileExtension,
        texturesPath + "top" + fileExtension,
        texturesPath + "bottom" + fileExtension,
        texturesPath + "back" + fileExtension,
        texturesPath + "front" + fileExtension
    };
    init(texturePaths);
}

Skybox::Skybox(const std::vector<std::string> &texturePaths) {
    init(texturePaths);
}

void Skybox::init(const std::vector<std::string> &texturePaths) {
    skybox = new Model();
    MeshFactory::addCube(skybox, glm::vec3(0), 1, glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), glm::vec4(0), true);
    cubeMap = new CubeTexture(texturePaths, true);
    cubeMap->fillTexture(true, false, false, GL_CLAMP_TO_EDGE);
}

Model *Skybox::getMesh() const {
    return skybox;
}

CubeTexture *Skybox::getCubeMap() const {
    return cubeMap;
}

Skybox::~Skybox() {
    delete skybox;
    delete cubeMap;
}
