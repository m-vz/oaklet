//
// Created by Milan van Zanten on 16.03.18.
//

#include "BitmapFont.h"
#include "../exception/Exception.h"
#include "../io/FileLoader.h"
#include "../util/Log.h"

const char *BitmapFont::VERTEX_SHADER_PATH = "assets/shaders/bitmap_vertex.glsl",
        *BitmapFont::FRAGMENT_SHADER_PATH = "assets/shaders/bitmap_fragment.glsl",
        BitmapFont::NULL_CHARACTER = '\0';
bool BitmapFont::shadersLoaded = false;
GLuint BitmapFont::bitmapProgramID = 0;

BitmapFont::BitmapFont(const std::string &bitmapPath, glm::vec2 bitmapArrayDimensions, glm::vec2 bitmapCharacterSize, std::vector<char> characters)
        : bitmapArrayDimensions(bitmapArrayDimensions), bitmapCharacterSize(bitmapCharacterSize) {
    bitmapTexture = new Texture(bitmapPath);
    bitmapTexture->useLinearFiltering(false);

    if(bitmapTexture->imageWidth != bitmapArrayDimensions.x*bitmapCharacterSize.x)
        throw IOException("Bitmap texture width " + std::to_string(bitmapTexture->imageWidth) + " does not match required width " + std::to_string(bitmapArrayDimensions.x*bitmapCharacterSize.x));
    if(bitmapTexture->imageHeight != bitmapArrayDimensions.y*bitmapCharacterSize.y)
        throw IOException("Bitmap texture height " + std::to_string(bitmapTexture->imageHeight) + " does not match required height " + std::to_string(bitmapArrayDimensions.y*bitmapCharacterSize.y));

    bitmapTexture->bindTexture(GL_TEXTURE0);
    bitmapTexture->fillTexture();

    if(!shadersLoaded)
        loadShaders();

    textureSamplerID = static_cast<GLuint>(glGetUniformLocation(bitmapProgramID, "textureSampler"));
    colorID = static_cast<GLuint>(glGetUniformLocation(bitmapProgramID, "textColor"));
    windowSizeID = static_cast<GLuint>(glGetUniformLocation(bitmapProgramID, "windowSize"));

    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &uvBuffer);

    for(char c: characters)
        this->characters.insert(c);

    int i = 0;
    for(int y = 0; y < bitmapArrayDimensions.y; ++y) {
        for(int x = 0; x < bitmapArrayDimensions.x; ++x) {
            if(i >= characters.size())
                break;

            characterMap[characters[i]] = glm::vec2(x, y);

            i++;
        }
    }
}

void BitmapFont::renderText(std::string text, glm::vec2 position, glm::vec2 windowSize, glm::vec3 color, float scale) {
    if(text.length() == 0) {
        Log::log << LOG_WARNING << "Did not render zero-length text.";
    }

    std::vector<float> vertex, uv;
    char character;
    glm::vec2 size = bitmapCharacterSize * scale;
    glm::vec2 topLeft, topRight, bottomLeft, bottomRight, bitmapPosition; // NOLINT

    position.y -= size.y;

    for(int i = 0; i < text.length(); ++i) {
        topLeft =     glm::vec2(position.x +       i*size.x, position.y + size.y);
        topRight =    glm::vec2(position.x + (i + 1)*size.x, position.y + size.y);
        bottomLeft =  glm::vec2(position.x +       i*size.x, position.y);
        bottomRight = glm::vec2(position.x + (i + 1)*size.x, position.y);

        vertex.push_back(topLeft.x); vertex.push_back(topLeft.y);
        vertex.push_back(bottomLeft.x); vertex.push_back(bottomLeft.y);
        vertex.push_back(topRight.x); vertex.push_back(topRight.y);

        vertex.push_back(bottomRight.x); vertex.push_back(bottomRight.y);
        vertex.push_back(topRight.x); vertex.push_back(topRight.y);
        vertex.push_back(bottomLeft.x); vertex.push_back(bottomLeft.y);

        character = text[i];
        if(characters.find(character) == characters.end()) // font does not contain this character
            character = NULL_CHARACTER;

        bitmapPosition = characterMap[character];

        topLeft =     glm::vec2(      bitmapPosition.x/bitmapArrayDimensions.x,       bitmapPosition.y/bitmapArrayDimensions.y);
        topRight =    glm::vec2((bitmapPosition.x + 1)/bitmapArrayDimensions.x,       bitmapPosition.y/bitmapArrayDimensions.y);
        bottomLeft =  glm::vec2(      bitmapPosition.x/bitmapArrayDimensions.x, (bitmapPosition.y + 1)/bitmapArrayDimensions.y);
        bottomRight = glm::vec2((bitmapPosition.x + 1)/bitmapArrayDimensions.x, (bitmapPosition.y + 1)/bitmapArrayDimensions.y);

        uv.push_back(topLeft.x); uv.push_back(1 - topLeft.y);
        uv.push_back(bottomLeft.x); uv.push_back(1 - bottomLeft.y);
        uv.push_back(topRight.x); uv.push_back(1 - topRight.y);

        uv.push_back(bottomRight.x); uv.push_back(1 - bottomRight.y);
        uv.push_back(topRight.x); uv.push_back(1 - topRight.y);
        uv.push_back(bottomLeft.x); uv.push_back(1 - bottomLeft.y);

        int a = 1;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(float), &vertex[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(float), &uv[0], GL_STATIC_DRAW);

    glUseProgram(bitmapProgramID);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0); // NOLINT

    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0); // NOLINT

    bitmapTexture->bindTexture(GL_TEXTURE0);
    glUniform1i(textureSamplerID, 0);

    glUniform3f(colorID, color.r, color.g, color.b);
    glUniform2f(windowSizeID, windowSize.x, windowSize.y);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertex.size()));

    glDisable(GL_BLEND);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void BitmapFont::renderTextUppercase(std::string text, glm::vec2 position, glm::vec2 windowSize, glm::vec3 color, float scale) {
    std::transform(text.begin(), text.end(),text.begin(), ::toupper);
    renderText(text, position, windowSize, color, scale);
}

const glm::vec2 &BitmapFont::getCharacterSize() const {
    return bitmapCharacterSize;
}

const std::set<char> &BitmapFont::getCharacters() const {
    return characters;
}

BitmapFont::~BitmapFont() {
    delete bitmapTexture;

    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &uvBuffer);
}

void BitmapFont::loadShaders() {
    bitmapProgramID = FileLoader::loadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    shadersLoaded = true;
}
