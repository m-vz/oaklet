//
// Created by Milan van Zanten on 19.04.18.
//

#ifndef BESTEST_GAME_DEPTHMAPTEXTURE_H
#define BESTEST_GAME_DEPTHMAPTEXTURE_H

#include "Texture.h"

class DepthMapTexture : public Texture {
public:
    GLuint texture = 0;

    DepthMapTexture(int width, int height);
};

#endif //BESTEST_GAME_DEPTHMAPTEXTURE_H
