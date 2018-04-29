//
// Created by Milan van Zanten on 19.04.18.
//

#include "Framebuffer.h"
#include "../../exception/Exception.h"

Framebuffer::Framebuffer(Texture *texture, GLenum attachment) : attachment(attachment) {
    this->framebufferWidth = texture->getImageWidth();
    this->framebufferHeight = texture->getImageHeight();
    this->texture = texture;
}

Framebuffer::Framebuffer(int width, int height, GLenum attachment, int channelCount)
        : framebufferWidth(width), framebufferHeight(height), attachment(attachment) {
    texture = new Texture(framebufferWidth, framebufferHeight, false, channelCount);
    texture->fillTexture(false, false, false, GL_CLAMP_TO_EDGE); // TODO: Let values be changed.
}

void Framebuffer::init(bool writeColor, bool readColor) {
    glGenFramebuffers(1, &fbo);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texture->getTarget(), texture->getTextureID(), 0);

    if(!writeColor)
        glDrawBuffer(GL_NONE);
    if(!readColor)
        glReadBuffer(GL_NONE);

    checkFramebuffer(glCheckFramebufferStatus(GL_FRAMEBUFFER));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    initialised = true;
}

void Framebuffer::bindFramebuffer(bool read) {
    if(!initialised)
        throw NotInitialisedException("Framebuffer");

    glBindFramebuffer(read ? GL_READ_FRAMEBUFFER : GL_DRAW_FRAMEBUFFER, fbo);
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

void Framebuffer::checkFramebuffer(GLenum status) {
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        if(status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
            throw Exception("Framebuffer could not be initialized: Not all framebuffer attachment points are framebuffer attachment complete.", status);
        else if(status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
            throw Exception("Framebuffer could not be initialized (GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER).", status);
        else if(status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
            throw Exception("Framebuffer could not be initialized: No images are attached to the framebuffer.", status);
        else if(status == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE)
            throw Exception("Framebuffer could not be initialized (GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE).", status);
        else if(status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
            throw Exception("Framebuffer could not be initialized (GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER).", status);
        else if(status == GL_FRAMEBUFFER_UNSUPPORTED)
            throw Exception("Framebuffer could not be initialized (GL_FRAMEBUFFER_UNSUPPORTED).", status);
        else
            throw Exception("Framebuffer could not be initialized.", status);
    }
}

Framebuffer::~Framebuffer() {
    delete texture;

    if(fbo != 0)
        glDeleteFramebuffers(1, &fbo);
}
