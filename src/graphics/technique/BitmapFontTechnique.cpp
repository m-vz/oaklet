//
// Created by Milan van Zanten on 2018-12-26.
//

#include "BitmapFontTechnique.h"

void BitmapFontTechnique::init() {
    Technique::init();
    addShader(GL_VERTEX_SHADER, "../assets/shaders/bitmap_vertex.glsl");
    addShader(GL_FRAGMENT_SHADER, "../assets/shaders/bitmap_fragment.glsl");
    finalize();

    windowSizeID = getUniformLocation("windowSize");

    bitmapTextureSamplerID = getUniformLocation("bitmapTextureSampler");
}

void BitmapFontTechnique::execute() {
    // enable technique
    enable();

    // prepare and validate
    glViewport(0, 0, viewportWidth, viewportHeight);

    validate();

    // bind vertex, uv and color buffers
    Mesh *mesh = text->createMesh();
    mesh->bindBuffer(mesh->vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // NOLINT
    mesh->bindBuffer(mesh->uvBuffer);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); // NOLINT
    mesh->bindBuffer(mesh->colorBuffer);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0); // NOLINT

    // bind bitmap texture to 0
    text->getFont()->bitmapTexture->bindTexture(0);
    setBitmapTextureSampler(0);

    // set viewport size
    glUniform2f(windowSizeID, viewportWidth, viewportHeight);

    // render
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->indices.size()), GL_UNSIGNED_INT, 0); // NOLINT

    glDisable(GL_BLEND);

    // finalize
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void BitmapFontTechnique::setBitmapTextureSampler(GLuint textureSampler) {
    glUniform1i(bitmapTextureSamplerID, textureSampler);
}

void BitmapFontTechnique::setViewportSize(int width, int height) {
    this->viewportWidth = width;
    this->viewportHeight = height;
}

void BitmapFontTechnique::setTextElement(TextElement *text) {
    this->text = text;
}

BitmapFontTechnique::~BitmapFontTechnique() = default;
