//
// Created by Milan van Zanten on 16.03.18.
//

#include "Texture.h"
#include "../io/FileLoader.h"

Texture::Texture(const std::string &texturePath, int desiredChannelCount) {
    FileLoader::loadPNG(texturePath, &textureData, &imageWidth, &imageHeight, &channelCount, desiredChannelCount);

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

void Texture::bindTexture() {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::fillTexture() {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
}
