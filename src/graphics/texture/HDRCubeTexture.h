//
// Created by Milan van Zanten on 20.05.18.
//

#ifndef BESTEST_GAME_HDRCUBETEXTURE_H
#define BESTEST_GAME_HDRCUBETEXTURE_H

#include "CubeTexture.h"

class HDRCubeTexture : public CubeTexture {
public:
    explicit HDRCubeTexture(int size, int desiredChannelCount = 3);
};

#endif //BESTEST_GAME_HDRCUBETEXTURE_H
