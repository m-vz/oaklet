//
// Created by Milan van Zanten on 16.03.18.
//

#include "Texture.h"
#include "../../io/FileLoader.h"
#include "../../util/Config.h"

Texture::Texture(int width, int height, bool convertToLinearSpace, int desiredChannelCount)
        : imageWidth(width), imageHeight(height), channelCount(desiredChannelCount) {
    init(desiredChannelCount, convertToLinearSpace);
}

Texture::Texture(const std::string &texturePath, bool convertToLinearSpace, int desiredChannelCount) {
    FileLoader::loadImage(texturePath, &textureData, &imageWidth, &imageHeight, &channelCount, desiredChannelCount);
    freeTextureData = true;

    init(desiredChannelCount, convertToLinearSpace);
}

void Texture::init(int desiredChannelCount, bool convertToLinearSpace) {
    if(convertToLinearSpace)
        internalFormat = GL_SRGB_ALPHA;
    else
        internalFormat = GL_RGBA;

    switch(desiredChannelCount) {
        case 1:
            format = GL_RED;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            break;
    }

    glGenTextures(1, &textureID);
}

void Texture::bindTexture(int unit) {
    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + unit));
    glBindTexture(target, textureID);
}

void Texture::fillTexture(bool filter, bool mipmap, bool filterBetweenMipmaps, GLint clamp) {
    bindTexture(0);

    glTexParameteri(target, GL_TEXTURE_WRAP_S, clamp);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, clamp);
    if(clamp == GL_CLAMP_TO_BORDER) { // TODO: currently, this is only used for depth maps, where the border should be white.
        float borderColours[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, borderColours);
    }

    setFilters(filter, mipmap, filterBetweenMipmaps);

    glTexImage2D(target, 0, internalFormat, imageWidth, imageHeight, 0, format, type, textureData);
    if(freeTextureData)
        FileLoader::freeImage(textureData);

    if(mipmap)
        glGenerateMipmap(target);
}

void Texture::setFilters(bool filter, bool mipmap, bool filterBetweenMipmaps) {
    if(filter) {
        if(mipmap) {
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            if(filterBetweenMipmaps)
                glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            else
                glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        } else {
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
    } else {
        if(mipmap) {
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            if(filterBetweenMipmaps)
                glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            else
                glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        } else {
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
    }
}

GLuint Texture::getTextureID() const {
    return textureID;
}

int Texture::getImageWidth() const {
    return imageWidth;
}

int Texture::getImageHeight() const {
    return imageHeight;
}

int Texture::getChannelCount() const {
    return channelCount;
}

GLenum Texture::getTarget() const {
    return target;
}

Texture::~Texture() {
    glDeleteTextures(1, &textureID);
}

aiTextureType Texture::textureTypeToAITextureType(TextureType type) {
    switch(type) {
        case TEXTURE_DIFFUSE:
            return aiTextureType_DIFFUSE;
        case TEXTURE_NORMAL:
            return aiTextureType_NORMALS;
        case TEXTURE_SPECULAR:
            return aiTextureType_SPECULAR;
        default:
            return aiTextureType_UNKNOWN;
    }
}
