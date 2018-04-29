//
// Created by Milan van Zanten on 27.04.18.
//

#include "CubeFramebuffer.h"

CubeFramebuffer::CubeFramebuffer(Texture *texture, GLenum attachment) : Framebuffer(texture, attachment) {}

CubeFramebuffer::CubeFramebuffer(int size, GLenum attachment, int channelCount)
        : Framebuffer(new Texture(size, size, false, channelCount), attachment) {
    texture->fillTexture(false, false, false, GL_CLAMP_TO_EDGE); // TODO: Let values be changed.
}

void CubeFramebuffer::init(bool writeColor, bool readColor) {
    glGenFramebuffers(1, &fbo);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, attachment, texture->getTextureID(), 0);

    if(!writeColor)
        glDrawBuffer(GL_NONE);
    if(!readColor)
        glReadBuffer(GL_NONE);

    checkFramebuffer(glCheckFramebufferStatus(GL_FRAMEBUFFER));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    initialised = true;
}
