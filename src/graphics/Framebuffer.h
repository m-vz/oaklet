//
// Created by Milan van Zanten on 19.04.18.
//

#ifndef BESTEST_GAME_FRAMEBUFFER_H
#define BESTEST_GAME_FRAMEBUFFER_H

#include "texture/Texture.h"

class Framebuffer {
public:
    Framebuffer(int width, int height, GLenum attachment, int channelCount = 3);
    void init();
    void bindFramebuffer();
    Texture *getTexture() const;
    int getFramebufferWidth() const;
    int getFramebufferHeight() const;
    virtual ~Framebuffer();

private:
    bool initialised = false;
    int framebufferWidth, framebufferHeight;
    GLenum attachment;
    Texture *texture;

    GLuint fbo = 0;
};

#endif //BESTEST_GAME_FRAMEBUFFER_H
