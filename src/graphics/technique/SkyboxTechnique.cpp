//
// Created by Milan van Zanten on 25.04.18.
//

#include "SkyboxTechnique.h"

void SkyboxTechnique::init() {
    Technique::init();
    addShader(GL_VERTEX_SHADER, "../assets/shaders/skybox_vertex.glsl");
    addShader(GL_FRAGMENT_SHADER, "../assets/shaders/skybox_fragment.glsl");
    finalize();

    vpID = getUniformLocation("vp");

    cubeMapTextureSamplerID = getUniformLocation("cubeMapTextureSampler");
}

void SkyboxTechnique::execute() {
    glDepthMask(GL_FALSE);

    skybox->getMesh()->meshes[0]->bindBuffer(skybox->getMesh()->meshes[0]->vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // NOLINT

    skybox->getMesh()->meshes[0]->bindBuffer(skybox->getMesh()->meshes[0]->indexBuffer, GL_ELEMENT_ARRAY_BUFFER);

    skybox->getCubeMap()->bindTexture(TEXTURE_DIFFUSE);
    glUniform1i(cubeMapTextureSamplerID, TEXTURE_DIFFUSE);

    validate();

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(skybox->getMesh()->meshes[0]->indices.size()), GL_UNSIGNED_INT, 0); // NOLINT

    glDisableVertexAttribArray(0);

    glDepthMask(GL_TRUE);
}

void SkyboxTechnique::setSkybox(Skybox *skybox) {
    this->skybox = skybox;
}

void SkyboxTechnique::setVP(const glm::mat4 &view, const glm::mat4 &projection) {
    glm::mat4 vp = projection * glm::mat4(glm::mat3(view));
    glUniformMatrix4fv(vpID, 1, GL_FALSE, &vp[0][0]);
}
