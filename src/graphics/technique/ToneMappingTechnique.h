//
// Created by Milan van Zanten on 21.05.18.
//

#ifndef BESTEST_GAME_TONEMAPPINGTECHNIQUE_H
#define BESTEST_GAME_TONEMAPPINGTECHNIQUE_H

#include "Technique.h"
#include "../framebuffer/Framebuffer.h"
#include "../model/Model.h"

class ToneMappingTechnique : public Technique {
public:
    void init() override;
    void execute() override;
    void setRenderResult(Framebuffer *renderResult);
    void setExposure(float *exposure);
    void setViewportSize(int width, int height);
    ~ToneMappingTechnique() override;

private:
    Framebuffer *renderResult = nullptr;
    int viewportWidth = 0, viewportHeight = 0;
    float *exposure;
    Model quad;

    GLuint renderedTextureSamplerID = 0;

    GLuint exposureID;
};

#endif //BESTEST_GAME_TONEMAPPINGTECHNIQUE_H
