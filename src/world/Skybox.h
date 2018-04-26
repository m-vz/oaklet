//
// Created by Milan van Zanten on 25.04.18.
//

#ifndef BESTEST_GAME_SKYBOX_H
#define BESTEST_GAME_SKYBOX_H

#include "../graphics/texture/CubeMapTexture.h"
#include "../graphics/model/Model.h"

class Skybox {
public:
    explicit Skybox(std::string texturesPath, std::string fileExtension = ".png");
    explicit Skybox(const std::vector<std::string> &texturePaths);
    Model *getMesh() const;
    CubeMapTexture *getCubeMap() const;
    virtual ~Skybox();

private:
    Model *skybox;
    CubeMapTexture *cubeMap;

    void init(const std::vector<std::string> &texturePaths);
};

#endif //BESTEST_GAME_SKYBOX_H
