//
// Created by Milan van Zanten on 20.05.18.
//

#include "HDRCubeTexture.h"

HDRCubeTexture::HDRCubeTexture(int size, int desiredChannelCount) : CubeTexture(size, false, desiredChannelCount) {
    type = GL_FLOAT;
    if(desiredChannelCount > 3)
        internalFormat = GL_RGBA16F;
    else
        internalFormat = GL_RGB16F;
}
