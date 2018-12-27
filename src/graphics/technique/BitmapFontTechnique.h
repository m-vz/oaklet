//
// Created by Milan van Zanten on 2018-12-26.
//

#ifndef BESTEST_GAME_FONTTECHNIQUE_H
#define BESTEST_GAME_FONTTECHNIQUE_H

#include "Technique.h"
#include "../gui/font/BitmapFont.h"
#include "../model/Model.h"
#include "../gui/TextElement.h"

class BitmapFontTechnique : public Technique {
public:
    // TODO: currently, these also need to be changed in the vertex and fragment shader. stupid
    static const unsigned int MAX_DIRECTIONAL_LIGHTS = 1;
    static const unsigned int MAX_POINT_LIGHTS = 4;
    static const unsigned int MAX_SPOT_LIGHTS = 4;

    void init() override;
    void execute() override;
    void setViewportSize(int width, int height);
    void setTextElement(TextElement *text);
    void setBitmapTexture(Texture *texture);
    ~BitmapFontTechnique() override;

private:
    int viewportWidth = 0, viewportHeight = 0;

    TextElement *text;

    GLuint windowSizeID;

    GLuint bitmapTextureSamplerID;

    void setBitmapTextureSampler(GLuint textureSampler);
};

#endif //BESTEST_GAME_FONTTECHNIQUE_H
