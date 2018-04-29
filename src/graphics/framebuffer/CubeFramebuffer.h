//
// Created by Milan van Zanten on 27.04.18.
//

#ifndef BESTEST_GAME_CUBEFRAMEBUFFER_H
#define BESTEST_GAME_CUBEFRAMEBUFFER_H

#include "Framebuffer.h"

class CubeFramebuffer : public Framebuffer {
public:
    CubeFramebuffer(Texture *texture, GLenum attachment);
    CubeFramebuffer(int size, GLenum attachment, int channelCount);
    void init(bool writeColor, bool readColor) override;
};

#endif //BESTEST_GAME_CUBEFRAMEBUFFER_H
