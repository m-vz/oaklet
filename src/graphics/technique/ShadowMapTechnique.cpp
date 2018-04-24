//
// Created by Milan van Zanten on 19.04.18.
//

#include "ShadowMapTechnique.h"
#include "../../exception/Exception.h"

void ShadowMapTechnique::init() {
    Technique::init();
    addShader(GL_VERTEX_SHADER, "assets/shaders/shadow_map_vertex.glsl");
    addShader(GL_FRAGMENT_SHADER, "assets/shaders/shadow_map_fragment.glsl");
    finalize();

    mvpID = getUniformLocation("mvp");
}

void ShadowMapTechnique::execute() {
    if(scene == nullptr)
        throw NotSetException("Scene");

    enable();

    for(auto light: scene->spotLights)
        renderDepthMap(light);
    for(auto light: scene->directionalLights)
        renderDepthMap(light);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapTechnique::renderDepthMap(LightWithShadowMap *light) {
    Framebuffer *depthBuffer = framebuffers[light];
    depthBuffer->bindFramebuffer();

    glViewport(0, 0, depthBuffer->getFramebufferWidth(), depthBuffer->getFramebufferHeight());

    glClear(GL_DEPTH_BUFFER_BIT);

    light->calculateVP();

    for(auto entity: scene->entities) {
        setMVP(light->getVP()*entity->getModel()->modelMatrix);

        for(auto mesh: entity->getModel()->meshes) {
            mesh->bindBuffer(mesh->vertexBuffer);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // NOLINT

            mesh->bindBuffer(mesh->indexBuffer, GL_ELEMENT_ARRAY_BUFFER);

            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->indices.size()), GL_UNSIGNED_INT, 0); // NOLINT

            glDisableVertexAttribArray(0);
        }
    }

    light->setShadowMap(depthBuffer->getTexture());
}

void ShadowMapTechnique::setMVP(const glm::mat4 &mvp) {
    glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
}

void ShadowMapTechnique::setScene(Scene *scene) {
    ShadowMapTechnique::scene = scene;

    // delete all old framebuffers
    for(auto iterator: framebuffers)
        delete iterator.second;

    for(auto light: scene->spotLights)
        addFramebuffer(light, SPOT_LIGHT_SHADOW_MAP_WIDTH, SPOT_LIGHT_SHADOW_MAP_HEIGHT);
    for(auto light: scene->directionalLights)
        addFramebuffer(light, DIRECTIONAL_LIGHT_SHADOW_MAP_WIDTH, DIRECTIONAL_LIGHT_SHADOW_MAP_HEIGHT);
}

void ShadowMapTechnique::addFramebuffer(LightWithShadowMap *light, int width, int height) {
    auto *depthBuffer = new Framebuffer(width, height, GL_DEPTH_ATTACHMENT);
    depthBuffer->init();
    framebuffers[light] = depthBuffer;
}

ShadowMapTechnique::~ShadowMapTechnique() {
    for(auto iterator: framebuffers)
        delete iterator.second;
}
