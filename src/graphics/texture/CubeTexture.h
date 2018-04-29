//
// Created by Milan van Zanten on 25.04.18.
//

#ifndef BESTEST_GAME_CUBETEXTURE_H
#define BESTEST_GAME_CUBETEXTURE_H

#include <GL/glew.h>
#include <string>
#include <vector>
#include "Texture.h"

class CubeTexture : public Texture {
public:
    explicit CubeTexture(int size, bool convertToLinearSpace, int desiredChannelCount = 3);
    explicit CubeTexture(const std::vector<std::string> &texturePaths, bool convertToLinearSpace, int desiredChannelCount = 3);
    void fillTexture(bool filter, bool mipmap, bool filterBetweenMipmaps, GLint clamp) override;

protected:
    void init(int desiredChannelCount, bool convertToLinearSpace) override;

private:
    std::vector<unsigned char*> textureData;
};

#endif //BESTEST_GAME_CUBETEXTURE_H
