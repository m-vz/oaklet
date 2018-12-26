//
// Created by Milan van Zanten on 19.04.18.
//

#ifndef BESTEST_GAME_FRAMEBUFFER_H
#define BESTEST_GAME_FRAMEBUFFER_H

#include "../texture/Texture.h"

class Framebuffer {
public:
    Framebuffer(Texture *texture, GLenum attachment);
    Framebuffer(int width, int height);
    void addTexture(Texture *texture, GLenum attachment);
    /// Initialize this framebuffer to be used in rendering.
    /// \param writeColor Whether the color buffer of this framebuffer will be written to.
    /// \param readColor Whether the color buffer of this framebuffer will be read from.
    void init(bool writeColor, bool readColor, bool specifyTextarget = false);
    void bindFramebuffer(bool read);
    Texture *getColorTexture(int attachmentIndex = 0) const;
    Texture *getDepthStencilTexture() const;
    Texture *getDepthTexture() const;
    int getFramebufferWidth() const;
    int getFramebufferHeight() const;
    virtual ~Framebuffer();

protected:
    bool initialised = false, colorTexture0Bound = false, depthStencilTextureBound = false, depthTextureBound = false;
    Texture *colorTexture0 = nullptr, *depthStencilTexture = nullptr, *depthTexture = nullptr;
    GLuint fbo = 0;

    void checkFramebuffer(GLenum status);

private:
    int framebufferWidth, framebufferHeight;
};

#endif //BESTEST_GAME_FRAMEBUFFER_H
