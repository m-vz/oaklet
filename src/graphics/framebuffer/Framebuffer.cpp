//
// Created by Milan van Zanten on 19.04.18.
//

#include "Framebuffer.h"

Framebuffer::Framebuffer(Texture *texture, GLenum attachment) : Framebuffer(texture->getImageWidth(), texture->getImageHeight()) {
    addTexture(texture, attachment);
}

Framebuffer::Framebuffer(int width, int height) {
    this->framebufferWidth = width;
    this->framebufferHeight = height;
}

void Framebuffer::addTexture(Texture *texture, GLenum attachment) {
    switch(attachment) {
        case GL_DEPTH_STENCIL_ATTACHMENT: // depth-stencil texture
            this->depthStencilTexture = texture;
            this->depthStencilTextureBound = true;
            break;
        case GL_DEPTH_ATTACHMENT: // depth texture
            this->depthTexture = texture;
            this->depthTextureBound = true;
            break;
        default:
            if(attachment >= GL_COLOR_ATTACHMENT0 && attachment < GL_COLOR_ATTACHMENT0 + MAX_COLOR_ATTACHMENTS) { // color texture
                this->colorTextures[attachment - GL_COLOR_ATTACHMENT0] = texture;
                this->colorTexturesBound[attachment - GL_COLOR_ATTACHMENT0] = true;
            } else // no supported texture
                throw FramebufferException("Trying to bind a texture with an unsupported attachment.");
    }
}

void Framebuffer::init(GLuint *drawAttachments,
                       int drawAttachmentsSize,
                       GLuint readAttachment,
                       bool readAttachmentSet,
                       bool specifyTextarget) {
    glGenFramebuffers(1, &fbo);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    if(specifyTextarget) {
        // color textures
        for(int i = 0; i < MAX_COLOR_ATTACHMENTS; ++i)
            if(colorTexturesBound[i])
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTextures[i]->getTarget(), colorTextures[i]->getTextureID(), 0);
        // depth-stencil texture
        if(depthStencilTextureBound)
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthStencilTexture->getTarget(), depthStencilTexture->getTextureID(), 0);
        // depth texture
        if(depthTextureBound)
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture->getTarget(), depthTexture->getTextureID(), 0);
    } else {
        // color textures
        for(int i = 0; i < MAX_COLOR_ATTACHMENTS; ++i)
            if(colorTexturesBound[i])
                glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTextures[i]->getTextureID(), 0);
        // depth-stencil texture
        if(depthStencilTextureBound)
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthStencilTexture->getTextureID(), 0);
        // depth texture
        if(depthTextureBound)
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture->getTextureID(), 0);
    }

    if(drawAttachmentsSize > 0) {
        if(drawAttachmentsSize == 1)
            glDrawBuffer(drawAttachments[0]);
        else
            glDrawBuffers(drawAttachmentsSize, drawAttachments);
    }
    if(readAttachmentSet)
        glReadBuffer(readAttachment);

    checkFramebuffer(glCheckFramebufferStatus(GL_FRAMEBUFFER));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    initialised = true;
}

void Framebuffer::init(bool drawColor, bool readColor, bool specifyTextarget) {
    if(drawColor) {
        if(readColor)
            init(nullptr, 0, 0, false, specifyTextarget);
        else
            init(nullptr, 0, GL_NONE, true, specifyTextarget);
    } else {
        GLuint drawAttachments[1] = { GL_NONE };

        if(readColor)
            init(drawAttachments, 1, 0, false, specifyTextarget);
        else
            init(drawAttachments, 1, GL_NONE, true, specifyTextarget);
    }
}

void Framebuffer::bindFramebuffer(bool read) {
    if(!initialised)
        throw NotInitialisedException("Framebuffer");

    glBindFramebuffer(read ? GL_READ_FRAMEBUFFER : GL_DRAW_FRAMEBUFFER, fbo);
}

Texture *Framebuffer::getColorTexture(int attachmentIndex) const {
    if(attachmentIndex >= GL_COLOR_ATTACHMENT0 && attachmentIndex < GL_COLOR_ATTACHMENT0 + MAX_COLOR_ATTACHMENTS) // index is actually an attachment, turn it into an index
        attachmentIndex -= GL_COLOR_ATTACHMENT0;

    if(attachmentIndex < 0 || attachmentIndex >= MAX_COLOR_ATTACHMENTS)
        throw FramebufferException("Trying to access a color attachment with an unsupported index.");

    if(colorTexturesBound[attachmentIndex])
        return colorTextures[attachmentIndex];
    else
        throw FramebufferException("Trying to access a color attachment that is not bound.");
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
    for(auto &texture: colorTextures)
        delete texture;
    delete depthStencilTexture;

    if(fbo != 0)
        glDeleteFramebuffers(1, &fbo);
}
