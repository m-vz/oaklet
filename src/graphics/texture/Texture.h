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
    explicit Texture(int width, int height, bool convertToLinearSpace, int desiredChannelCount = 3);
    explicit Texture(const std::string &texturePath, bool convertToLinearSpace, int desiredChannelCount = 3);
    void bindTexture(int unit);
    virtual void fillTexture(bool filter, bool mipmap, bool filterBetweenMipmaps, GLint clamp);
    GLuint getTextureID() const;
    int getImageWidth() const;
    int getImageHeight() const;
    int getChannelCount() const;
    GLenum getTarget() const;
    virtual ~Texture();

    static aiTextureType textureTypeToAITextureType(TextureType type);

protected:
    bool freeTextureData = false;
    int imageWidth, imageHeight, channelCount;
    GLuint textureID;
    GLint internalFormat;
    GLenum format = GL_RGB;
    GLenum type = GL_UNSIGNED_BYTE, target = GL_TEXTURE_2D;

    virtual void init(int desiredChannelCount, bool convertToLinearSpace);
    void setFilters(bool filter, bool mipmap, bool filterBetweenMipmaps);

private:
    unsigned char *textureData = nullptr;
};

#endif //BESTEST_GAME_TEXTURE_H
