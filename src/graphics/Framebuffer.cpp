//
// Created by Milan van Zanten on 19.04.18.
//

#include "Framebuffer.h"
#include "../exception/Exception.h"
#include "texture/DepthMapTexture.h"

Framebuffer::Framebuffer(int width, int height, GLenum attachment, int channelCount)
        : framebufferWidth(width), framebufferHeight(height), attachment(attachment) {
    if(attachment == GL_DEPTH_ATTACHMENT)
        texture = new DepthMapTexture(framebufferWidth, framebufferHeight);
    else
        texture = new Texture(framebufferWidth, framebufferHeight, false, channelCount);
    texture->fillTexture(false, false, false); // TODO: Let values be changed.
}

void Framebuffer::init() {
    glGenFramebuffers(1, &fbo);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->textureID, 0);

    // disable writing to the color buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        throw Exception("Framebuffer could not be initialized.", status); // NOLINT

    initialised = true;
}

void Framebuffer::bindFramebuffer() {
    if(!initialised)
        throw NotInitialisedException("Framebuffer");

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

Texture *Framebuffer::getTexture() const {
    return texture;
}

int Framebuffer::getFramebufferWidth() const {
    return framebufferWidth;
}

int Framebuffer::getFramebufferHeight() const {
    return framebufferHeight;
}

Framebuffer::~Framebuffer() {
    delete texture;

    if(fbo != 0)
        glDeleteFramebuffers(1, &fbo);
}
