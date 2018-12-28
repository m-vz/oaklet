//
// Created by Milan van Zanten on 21.05.18.
//

#include "ToneMappingTechnique.h"
#include "../../util/Log.h"
#include "../model/MeshFactory.h"

void ToneMappingTechnique::init() {
    Technique::init();
    addShader(GL_VERTEX_SHADER, "../assets/shaders/tone_mapping_vertex.glsl");
    addShader(GL_FRAGMENT_SHADER, "../assets/shaders/tone_mapping_fragment.glsl");
    finalize();

    MeshFactory::addQuad(&quad);

    renderedTextureSamplerID = getUniformLocation("renderedTextureSampler");

    exposureID = getUniformLocation("exposure");
}

void ToneMappingTechnique::execute() {
    if(renderResult != nullptr) {
        enable();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, viewportWidth, viewportHeight);
        glClear(GL_COLOR_BUFFER_BIT);

        validate();

        Mesh *quadMesh = quad.meshes[0];
        quadMesh->bindBuffer(quadMesh->vertexBuffer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // NOLINT
        quadMesh->bindBuffer(quadMesh->uvBuffer);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); // NOLINT
        quadMesh->bindBuffer(quadMesh->indexBuffer, GL_ELEMENT_ARRAY_BUFFER);

        glUniform1f(exposureID, *exposure);

        glUniform1i(renderedTextureSamplerID, TEXTURE_MAX);
        renderResult->getColorTexture(0)->bindTexture(TEXTURE_MAX);

        glDisable(GL_DEPTH_TEST);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(quadMesh->indices.size()), GL_UNSIGNED_INT, 0); // NOLINT

        glEnable(GL_DEPTH_TEST);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    } else
        Log::log << LOG_WARNING << "ToneMapping render result is not set.";
}

void ToneMappingTechnique::setRenderResult(Framebuffer *renderResult) {
    this->renderResult = renderResult;
}

void ToneMappingTechnique::setExposure(float *exposure) {
    this->exposure = exposure;
}

void ToneMappingTechnique::setViewportSize(int width, int height) {
    viewportWidth = width;
    viewportHeight = height;
}

ToneMappingTechnique::~ToneMappingTechnique() = default;
