//
// Created by Milan van Zanten on 19.04.18.
//

#include "Framebuffer.h"
#include "../../exception/Exception.h"

Framebuffer::Framebuffer(Texture *texture, GLenum attachment) : Framebuffer(texture->getImageWidth(), texture->getImageHeight()) {
    addTexture(texture, attachment);
}

Framebuffer::Framebuffer(int width, int height) {
    this->framebufferWidth = width;
    this->framebufferHeight = height;
}

void Framebuffer::addTexture(Texture *texture, GLenum attachment) {
    switch(attachment) {
        case GL_COLOR_ATTACHMENT0:
            this->colorTexture0 = texture;
            this->colorTexture0Bound = true;
            break;
        case GL_DEPTH_STENCIL_ATTACHMENT:
            this->depthStencilTexture = texture;
            this->depthStencilTextureBound = true;
            break;
        case GL_DEPTH_ATTACHMENT:
            this->depthTexture = texture;
            this->depthTextureBound = true;
            break;
        default:
            throw FramebufferException("Trying to bind a texture with an unsupported attachment.");
    }
}

void Framebuffer::init(bool writeColor, bool readColor, bool specifyTextarget) {
    glGenFramebuffers(1, &fbo);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    if(specifyTextarget) {
        if(colorTexture0Bound)
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTexture0->getTarget(), colorTexture0->getTextureID(), 0);
        if(depthStencilTextureBound)
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthStencilTexture->getTarget(), depthStencilTexture->getTextureID(), 0);
        if(depthTextureBound)
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture->getTarget(), depthTexture->getTextureID(), 0);
    } else {
        if(colorTexture0Bound)
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTexture0->getTextureID(), 0);
        if(depthStencilTextureBound)
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthStencilTexture->getTextureID(), 0);
        if(depthTextureBound)
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture->getTextureID(), 0);
    }

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

Texture *Framebuffer::getColorTexture(int attachmentIndex) const {
    if(!colorTexture0Bound)
        throw FramebufferException("Trying to access a color attachment that is not bound.");

    switch(attachmentIndex) {
        case 0:
            return colorTexture0;
        default:
            throw FramebufferException("Trying to access a color attachment with an unsupported index.");
    }
}

Texture *Framebuffer::getDepthStencilTexture() const {
    if(!depthStencilTextureBound)
        throw FramebufferException("Trying to access a depth-stencil attachment that is not bound.");

    return depthStencilTexture;
}

Texture *Framebuffer::getDepthTexture() const {
    if(!depthTextureBound)
        throw FramebufferException("Trying to access a depth attachment that is not bound.");

    return depthTexture;
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
            throw FramebufferException("Framebuffer could not be initialized: Not all framebuffer attachment points are framebuffer attachment complete.", status);
        else if(status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
            throw FramebufferException("Framebuffer could not be initialized (GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER).", status);
        else if(status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
            throw FramebufferException("Framebuffer could not be initialized: No images are attached to the framebuffer.", status);
        else if(status == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE)
            throw FramebufferException("Framebuffer could not be initialized (GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE).", status);
        else if(status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
            throw FramebufferException("Framebuffer could not be initialized (GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER).", status);
        else if(status == GL_FRAMEBUFFER_UNSUPPORTED)
            throw FramebufferException("Framebuffer could not be initialized (GL_FRAMEBUFFER_UNSUPPORTED).", status);
        else
            throw FramebufferException("Framebuffer could not be initialized.", status);
    }
}

Framebuffer::~Framebuffer() {
    delete colorTexture0;
    delete depthStencilTexture;

    if(fbo != 0)
        glDeleteFramebuffers(1, &fbo);
}
