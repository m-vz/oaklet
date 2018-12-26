//
// Created by Milan van Zanten on 21.05.18.
//

#include "ToneMappingTechnique.h"
#include "../../util/Log.h"

void ToneMappingTechnique::init() {
    Technique::init();
    addShader(GL_VERTEX_SHADER, "../assets/shaders/tone_mapping_vertex.glsl");
    addShader(GL_FRAGMENT_SHADER, "../assets/shaders/tone_mapping_fragment.glsl");
    finalize();

    renderedTextureSamplerID = getUniformLocation("renderedTextureSampler");
}

void ToneMappingTechnique::execute() {
    if(renderResult != nullptr) {
        enable();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindVertexArray(quadVertexArrayID);

        glViewport(0, 0, viewportWidth, viewportHeight);
        glClear(GL_COLOR_BUFFER_BIT);

        validate();

        glBindBuffer(GL_ARRAY_BUFFER, 1);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // NOLINT
        glBindBuffer(GL_ARRAY_BUFFER, 2);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); // NOLINT
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 3);

        glUniform1i(renderedTextureSamplerID, TEXTURE_MAX);
        renderResult->getColorTexture()->bindTexture(TEXTURE_MAX);

        glDisable(GL_DEPTH_TEST);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // NOLINT
        glEnable(GL_DEPTH_TEST);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glBindVertexArray(vertexArrayID);
    } else
        Log::log << LOG_WARNING << "ToneMapping render result is not set.";
}

void ToneMappingTechnique::setQuadVAO(GLuint quadVertexArrayID) {
    this->quadVertexArrayID = quadVertexArrayID;
}

void ToneMappingTechnique::setVAO(GLuint vertexArrayID) {
    this->vertexArrayID = vertexArrayID;
}

void ToneMappingTechnique::setRenderResult(Framebuffer *renderResult) {
    this->renderResult = renderResult;
}

void ToneMappingTechnique::setViewportSize(int width, int height) {
    viewportWidth = width;
    viewportHeight = height;
}

ToneMappingTechnique::~ToneMappingTechnique() = default;
