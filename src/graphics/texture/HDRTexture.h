//
// Created by Milan van Zanten on 20.05.18.
//

#ifndef BESTEST_GAME_HDRTEXTURE_H
#define BESTEST_GAME_HDRTEXTURE_H

#include "Texture.h"

class HDRTexture : public Texture {
public:
    HDRTexture(int width, int height, int desiredChannelCount = 3);
};

#endif //BESTEST_GAME_HDRTEXTURE_H
