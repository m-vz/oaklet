//
// Created by Milan van Zanten on 19.04.18.
//

#ifndef BESTEST_GAME_FRAMEBUFFER_H
#define BESTEST_GAME_FRAMEBUFFER_H

#include "../texture/Texture.h"

class Framebuffer {
public:
    Framebuffer(Texture *texture, GLenum attachment);
    Framebuffer(int width, int height, GLenum attachment, int channelCount = 3);
    virtual void init(bool writeColor, bool readColor);
    void bindFramebuffer(bool read);
    Texture *getTexture() const;
    int getFramebufferWidth() const;
    int getFramebufferHeight() const;
    virtual ~Framebuffer();

protected:
    bool initialised = false;
    Texture *texture;
    GLenum attachment;
    GLuint fbo = 0;

    void checkFramebuffer(GLenum status);

private:
    int framebufferWidth, framebufferHeight;
};

#endif //BESTEST_GAME_FRAMEBUFFER_H
