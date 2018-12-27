//
// Created by Milan van Zanten on 16.03.18.
//

#include "BitmapFont.h"
#include "../../../exception/Exception.h"
#include "../../../io/FileLoader.h"
#include "../../../util/Log.h"

BitmapFont::BitmapFont(const std::string &bitmapPath, glm::vec2 bitmapArrayDimensions, glm::vec2 bitmapCharacterSize, std::vector<char> characters)
        : bitmapArrayDimensions(bitmapArrayDimensions), bitmapCharacterSize(bitmapCharacterSize) {
    bitmapTexture = new Texture(bitmapPath, false);

    if(bitmapTexture->getImageWidth() != bitmapArrayDimensions.x*bitmapCharacterSize.x)
        throw IOException("Bitmap texture width " + std::to_string(bitmapTexture->getImageWidth()) + " does not match required width " + std::to_string(bitmapArrayDimensions.x*bitmapCharacterSize.x));
    if(bitmapTexture->getImageHeight() != bitmapArrayDimensions.y*bitmapCharacterSize.y)
        throw IOException("Bitmap texture height " + std::to_string(bitmapTexture->getImageHeight()) + " does not match required height " + std::to_string(bitmapArrayDimensions.y*bitmapCharacterSize.y));

    bitmapTexture->fillTexture(false, false, false, GL_CLAMP_TO_EDGE);

    for(char c: characters)
        this->characters.insert(c);

    int i = 0;
    for(auto y = static_cast<int>(bitmapArrayDimensions.y - 1); y >= 0; --y) {
        for(int x = 0; x < bitmapArrayDimensions.x; ++x) {
            if(i >= characters.size())
                break;

            characterMap[characters[i]] = glm::vec2(x, y);

            i++;
        }
    }
}

Mesh *BitmapFont::triangulateText(std::string text, glm::vec2 position, glm::vec3 color, float scale) {
    if(text.length() == 0)
        Log::log << LOG_WARNING << "Did not triangulate zero-length text.";

    auto *mesh = new Mesh();
    char character;
    unsigned long textLength = text.length();
    glm::vec2 size = bitmapCharacterSize * scale;
    glm::vec2 bitmapPosition;
    std::vector<glm::vec2> vertices(4*textLength);
    std::vector<glm::vec2> uvs(4*textLength);
    std::vector<unsigned int> indices(6*textLength);

    for(unsigned int i = 0; i < textLength; ++i) {
        vertices[4*i]     = glm::vec2(position.x +       i*size.x, position.y + size.y); // bottom left
        vertices[4*i + 1] = glm::vec2(position.x + (i + 1)*size.x, position.y + size.y); // bottom right
        vertices[4*i + 2] = glm::vec2(position.x + (i + 1)*size.x, position.y);          // top right
        vertices[4*i + 3] = glm::vec2(position.x +       i*size.x, position.y);          // top left

        character = text[i];
        if(characters.find(character) == characters.end()) // font does not contain this character
            character = '\0';

        bitmapPosition = characterMap[character];

        uvs[4*i]     = glm::vec2(      bitmapPosition.x/bitmapArrayDimensions.x,       bitmapPosition.y/bitmapArrayDimensions.y); // bottom left
        uvs[4*i + 1] = glm::vec2((bitmapPosition.x + 1)/bitmapArrayDimensions.x,       bitmapPosition.y/bitmapArrayDimensions.y); // bottom right
        uvs[4*i + 2] = glm::vec2((bitmapPosition.x + 1)/bitmapArrayDimensions.x, (bitmapPosition.y + 1)/bitmapArrayDimensions.y); // top right
        uvs[4*i + 3] = glm::vec2(      bitmapPosition.x/bitmapArrayDimensions.x, (bitmapPosition.y + 1)/bitmapArrayDimensions.y); // top left

        indices[6*i] = 4*i; indices[6*i + 1] = 4*i + 1; indices[6*i + 2] = 4*i + 2;
        indices[6*i + 3] = 4*i; indices[6*i + 4] = 4*i + 2; indices[6*i + 5] = 4*i + 3;
    }

    for(auto &vertex: vertices) {
        mesh->vertexData.push_back(vertex.x);
        mesh->vertexData.push_back(vertex.y);

        mesh->colorData.push_back(color.r);
        mesh->colorData.push_back(color.g);
        mesh->colorData.push_back(color.b);
    }
    for(auto &uv: uvs) {
        mesh->uvData.push_back(uv.x);
        mesh->uvData.push_back(uv.y);
    }
    for(auto &index: indices)
        mesh->indices.push_back(index);

    mesh->fillBuffers(true, false, false, false);
    mesh->setColor(glm::vec4(color.r, color.g, color.b, 1.0));

    return mesh;
}

Mesh *BitmapFont::triangulateTextUppercase(std::string text, glm::vec2 position, glm::vec3 color, float scale) {
    std::transform(text.begin(), text.end(),text.begin(), ::toupper);
    return triangulateText(text, position, color, scale);
}

const glm::vec2 &BitmapFont::getCharacterSize() const {
    return bitmapCharacterSize;
}

const std::set<char> &BitmapFont::getCharacters() const {
    return characters;
}

BitmapFont::~BitmapFont() {
    delete bitmapTexture;
}
