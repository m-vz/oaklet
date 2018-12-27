//
// Created by Milan van Zanten on 2018-12-27.
//

#ifndef BESTEST_GAME_TEXTELEMENT_H
#define BESTEST_GAME_TEXTELEMENT_H

#include <string>
#include <glm/glm.hpp>
#include "font/BitmapFont.h"

class TextElement {
public:
    TextElement(const std::string &text, BitmapFont *font, const glm::vec2 &position, const glm::vec3 &color, float scale);
    Mesh *createMesh();
    BitmapFont *getFont() const;
    void setText(const std::string &text);
    void setFont(BitmapFont *font);
    void setPosition(const glm::vec2 &position);
    void setColor(const glm::vec3 &color);
    void setScale(float scale);
    virtual ~TextElement();

private:
    bool changed = true;
    Mesh *mesh;
    std::string text;
    BitmapFont *font;
    glm::vec2 position;
    glm::vec3 color;
    float scale;
};

#endif //BESTEST_GAME_TEXTELEMENT_H
