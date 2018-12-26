//
// Created by Milan van Zanten on 20.05.18.
//

#include "HDRTexture.h"

HDRTexture::HDRTexture(int width, int height, int desiredChannelCount) : Texture(width, height, false, desiredChannelCount) {
    type = GL_FLOAT;
    if(desiredChannelCount > 3)
        internalFormat = GL_RGBA16F;
    else
        internalFormat = GL_RGB16F;
}
