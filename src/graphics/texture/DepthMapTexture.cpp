//
// Created by Milan van Zanten on 19.04.18.
//

#include "DepthMapTexture.h"

DepthMapTexture::DepthMapTexture(int width, int height) : Texture(width, height, false, 1) {
    type = GL_FLOAT;
    internalFormat = GL_DEPTH_COMPONENT;
    format = GL_DEPTH_COMPONENT;

    fillTexture(false, false, false, GL_CLAMP_TO_BORDER);
}
