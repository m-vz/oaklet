//
// Created by Milan van Zanten on 2018-12-27.
//

#include "TextElement.h"

TextElement::TextElement(const std::string &text, BitmapFont *font, const glm::vec2 &position, const glm::vec3 &color, float scale)
                            : text(text), font(font), position(position), color(color), scale(scale) {}

Mesh *TextElement::createMesh() {
    if(changed) {
        mesh = font->triangulateTextUppercase(text, position, color, scale);
        changed = true; // TODO: this doesn't get set to false, because somehow, rendering text doesn't work with the cached mesh. find out why, set this to false and save recreating the same mesh every frame.
    }
    return mesh;
}

BitmapFont *TextElement::getFont() const {
    return font;
}

void TextElement::setText(const std::string &text) {
    TextElement::text = text;
    changed = true;
}

void TextElement::setFont(BitmapFont *font) {
    TextElement::font = font;
    changed = true;
}

void TextElement::setPosition(const glm::vec2 &position) {
    TextElement::position = position;
    changed = true;
}

void TextElement::setColor(const glm::vec3 &color) {
    TextElement::color = color;
    changed = true;
}

void TextElement::setScale(float scale) {
    TextElement::scale = scale;
    changed = true;
}

TextElement::~TextElement() {
    delete mesh;
}
