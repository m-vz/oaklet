//
// Created by Milan van Zanten on 16.03.18.
//

#include "Texture.h"
#include "../../io/FileLoader.h"
#include "../../util/Config.h"

Texture::Texture(const std::string &texturePath, int desiredChannelCount) {
    FileLoader::loadImage(texturePath, &textureData, &imageWidth, &imageHeight, &channelCount, desiredChannelCount);

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

    // check power-of-two-ness
    int size = 2;
    if(imageWidth != imageHeight)
        size = -1;
    while(size > 0) {
        if(imageWidth == size)
            size = 0;
        else if(size > MAX_POWER_OF_TWO_TEXTURE_SIZE)
            size = -1;
        else
            size *= 2;
    }
    if(size < 0) { // texture is not a power-of-two texture (square with size equal to a power of two less than MAX_POWER_OF_TWO_TEXTURE_SIZE)
        // required for non-power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
}

void Texture::bindTexture(int unit) {
    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + unit));
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::fillTexture() {
    setFilter();
    glTexImage2D(GL_TEXTURE_2D, 0, format, imageWidth, imageHeight, 0, static_cast<GLenum>(format), GL_UNSIGNED_BYTE, textureData);
}

void Texture::useLinearFiltering(bool filter) {
    this->filter = filter;
}

void Texture::setFilter() {
    if(!filter) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    } else if(CONFIG.OPENGL_MIPMAP) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
}

Texture::~Texture() {
    FileLoader::freeImage(textureData);
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
