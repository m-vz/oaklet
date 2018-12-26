//
// Created by Milan van Zanten on 2018-12-26.
//

#include "DepthStencilTexture.h"

DepthStencilTexture::DepthStencilTexture(int width, int height) : Texture(width, height, false, 1) {
    type = GL_UNSIGNED_INT_24_8;
    internalFormat = GL_DEPTH24_STENCIL8;
    format = GL_DEPTH_STENCIL;

    fillTexture(false, false, false, GL_CLAMP_TO_BORDER);
}
