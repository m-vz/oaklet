//
// Created by Milan van Zanten on 27.04.18.
//

#include "DepthMapCubeTexture.h"

DepthMapCubeTexture::DepthMapCubeTexture(int size) : CubeTexture(size, false, 1) {
    type = GL_FLOAT;
    internalFormat = GL_DEPTH_COMPONENT;
    format = GL_DEPTH_COMPONENT;

    fillTexture(false, false, false, GL_CLAMP_TO_EDGE);
}
