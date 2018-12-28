//
// Created by Milan van Zanten on 2018-12-29.
//

#include "TextureDebugTechnique.h"
#include "../../util/Log.h"
#include "../model/MeshFactory.h"

void TextureDebugTechnique::init() {
    Technique::init();
    addShader(GL_VERTEX_SHADER, "../assets/shaders/texture_debug_vertex.glsl");
    addShader(GL_FRAGMENT_SHADER, "../assets/shaders/texture_debug_fragment.glsl");
    finalize();

    renderedTextureSamplerID = getUniformLocation("renderedTextureSampler");
}

void TextureDebugTechnique::execute() {
    if(!quads.meshes.empty()) {
        enable();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, viewportWidth, viewportHeight);
        glDisable(GL_DEPTH_TEST);

        validate();

        for(Mesh *mesh: quads.meshes) {
            mesh->bindBuffer(mesh->vertexBuffer);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // NOLINT
            mesh->bindBuffer(mesh->uvBuffer);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); // NOLINT
            mesh->bindBuffer(mesh->indexBuffer, GL_ELEMENT_ARRAY_BUFFER);

            quads.materials[mesh->materialIndex][TEXTURE_DIFFUSE]->bindTexture(TEXTURE_DIFFUSE);
            glUniform1i(renderedTextureSamplerID, TEXTURE_MAX);

            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->indices.size()), GL_UNSIGNED_INT, 0); // NOLINT
        }

        glEnable(GL_DEPTH_TEST);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    } else
        Log::log << LOG_WARNING << "No texture quads to render with TextureDebugTechnique.";
}

void TextureDebugTechnique::addTexture(Texture *texture) {
    MeshFactory::addQuad(&quads, glm::vec2(0.5f*quads.meshes.size() - 0.75f, -0.75f), 0.5f, 0.5f);
    quads.setMeshTexture(static_cast<int>(quads.meshes.size() - 1), TEXTURE_DIFFUSE, texture);
}

void TextureDebugTechnique::setViewportSize(int width, int height) {
    viewportWidth = width;
    viewportHeight = height;
}

TextureDebugTechnique::~TextureDebugTechnique() = default;
