//
// Created by Milan van Zanten on 16.03.18.
//

#ifndef BESTEST_GAME_FONT_H
#define BESTEST_GAME_FONT_H

#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <glm/glm.hpp>
#include "model/Texture.h"

class BitmapFont {
public:
    static const char *VERTEX_SHADER_PATH, *FRAGMENT_SHADER_PATH;
    static GLuint bitmapProgramID;
    GLuint textureSamplerID, colorID, windowSizeID;
    GLuint vertexBuffer, uvBuffer;
    Texture *bitmapTexture;

    BitmapFont(const std::string &bitmapPath, glm::vec2 bitmapArrayDimensions, glm::vec2 bitmapCharacterSize, std::vector<char> characters);
    void renderText(std::string text, glm::vec2 position, glm::vec2 windowSize, glm::vec3 color = glm::vec3(1.0f), float scale = 1.0f);
    void renderTextUppercase(std::string text, glm::vec2 position, glm::vec2 windowSize, glm::vec3 color = glm::vec3(1.0f), float scale = 1.0f);
    const glm::vec2 &getCharacterSize() const;
    const std::set<char> &getCharacters() const;
    virtual ~BitmapFont();

private:
    static bool shadersLoaded;
    glm::vec2 bitmapArrayDimensions, bitmapCharacterSize;
    std::unordered_map<char, glm::vec2> characterMap;
    std::set<char> characters;

    static void loadShaders();
};

#endif //BESTEST_GAME_FONT_H
