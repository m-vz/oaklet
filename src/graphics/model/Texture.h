//
// Created by Milan van Zanten on 16.03.18.
//

#ifndef BESTEST_GAME_TEXTURE_H
#define BESTEST_GAME_TEXTURE_H

#include <GL/glew.h>
#include <string>
#include <assimp/material.h>

enum TextureType {
    TEXTURE_DIFFUSE = 0,
    TEXTURE_NORMAL,
    TEXTURE_SPECULAR,
    TEXTURE_MAX
};

class Texture {
public:
    static const int MAX_POWER_OF_TWO_TEXTURE_SIZE = 16384; // 2^14, largest size for a power-of-two texture to be recognized as one.
    int imageWidth, imageHeight, channelCount;
    unsigned char *textureData;
    GLuint textureID;

    explicit Texture(const std::string &texturePath, int desiredChannelCount = 3);
    void bindTexture(int unit);
    void fillTexture();
    void useLinearFiltering(bool filter = true);
    virtual ~Texture();

    static aiTextureType textureTypeToAITextureType(TextureType type);

private:
    GLint format;
    bool filter = true;

    void setFilter();
};

#endif //BESTEST_GAME_TEXTURE_H
