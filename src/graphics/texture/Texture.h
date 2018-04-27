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
    TEXTURE_DEPTH
};

class Texture {
public:
    unsigned char *textureData = nullptr;
    GLuint textureID;

    explicit Texture(int width, int height, bool convertToLinearSpace, int desiredChannelCount = 3);
    explicit Texture(const std::string &texturePath, bool convertToLinearSpace, int desiredChannelCount = 3);
    void bindTexture(int unit);
    void fillTexture(bool filter = true, bool mipmap = false, GLint clamp = GL_CLAMP_TO_EDGE);
    int getImageWidth() const;
    int getImageHeight() const;
    int getChannelCount() const;
    virtual ~Texture();

    static aiTextureType textureTypeToAITextureType(TextureType type);

protected:
    int imageWidth, imageHeight, channelCount;
    GLint internalFormat;
    GLenum format = GL_RGB;
    GLenum type = GL_UNSIGNED_BYTE;

private:
    bool freeTextureData = false;

    void init(int desiredChannelCount, bool convertToLinearSpace);
};

#endif //BESTEST_GAME_TEXTURE_H
