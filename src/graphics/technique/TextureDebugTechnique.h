//
// Created by Milan van Zanten on 2018-12-29.
//

#ifndef BESTEST_GAME_TEXTUREDEBUGTECHNIQUE_H
#define BESTEST_GAME_TEXTUREDEBUGTECHNIQUE_H

#include "Technique.h"
#include "../model/Model.h"

class TextureDebugTechnique : public Technique {
public:
    void init() override;
    void execute() override;
    void addTexture(Texture *texture);
    void setViewportSize(int width, int height);
    ~TextureDebugTechnique() override;

private:
    Model quads;
    int viewportWidth = 0, viewportHeight = 0;

    GLuint renderedTextureSamplerID = 0;
};

#endif //BESTEST_GAME_TEXTUREDEBUGTECHNIQUE_H
