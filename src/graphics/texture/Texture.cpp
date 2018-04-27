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
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::fillTexture(bool filter, bool mipmap, bool filterBetweenMipmaps, GLint clamp) {
    bindTexture(0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp);
    if(clamp == GL_CLAMP_TO_BORDER) { // TODO: currently, this is only used for depth maps, where the border should be white.
        float borderColours[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColours);
    }

    if(filter) {
        if(mipmap) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            if(filterBetweenMipmaps)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            else
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
    } else {
        if(mipmap) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            if(filterBetweenMipmaps)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            else
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imageWidth, imageHeight, 0, format, type, textureData);
    if(freeTextureData)
        FileLoader::freeImage(textureData);

    if(mipmap)
        glGenerateMipmap(GL_TEXTURE_2D);
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
