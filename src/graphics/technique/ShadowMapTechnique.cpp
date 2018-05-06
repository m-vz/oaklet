//
// Created by Milan van Zanten on 19.04.18.
//

#include "ShadowMapTechnique.h"
#include "../../exception/Exception.h"
#include "../texture/DepthMapTexture.h"
#include "../texture/DepthMapCubeTexture.h"
#include "../framebuffer/CubeFramebuffer.h"

void ShadowMapTechnique::init() {
    Technique::init();
    addShader(GL_VERTEX_SHADER, "assets/shaders/shadow_map_vertex.glsl");
    addShader(GL_GEOMETRY_SHADER, "assets/shaders/shadow_map_geometry.glsl");
    addShader(GL_FRAGMENT_SHADER, "assets/shaders/shadow_map_fragment.glsl");
    finalize();

    modelID = getUniformLocation("model");
    for(int i = 0; i < 6; ++i)
        vpIDs[i] = getUniformLocation("vps[" + std::to_string(i) + "]");
    worldspaceLightPositionID = getUniformLocation("worldspaceLightPosition");
    cubeID = getUniformLocation("cube");
    farID = getUniformLocation("far");
}

void ShadowMapTechnique::execute() {
    if(scene == nullptr)
        throw NotSetException("Scene");

    enable();

    for(auto iterator: framebuffers)
        renderDepthMap(iterator.first);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapTechnique::renderDepthMap(LightWithShadowMap *light) {
    Framebuffer *depthBuffer = framebuffers[light];
    depthBuffer->bindFramebuffer(false);

    glViewport(0, 0, depthBuffer->getFramebufferWidth(), depthBuffer->getFramebufferHeight());

    glClear(GL_DEPTH_BUFFER_BIT);

    light->calculateVP();
    setLight(*light);

    validate();

    for(auto entity: scene->meshEntities) {
        setModel(entity->getModel()->modelMatrix);

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

void ShadowMapTechnique::setModel(const glm::mat4 &model) {
    glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
}

void ShadowMapTechnique::setLight(const LightWithShadowMap &light) {
    if(light.isDepthMapCube())
        for(int i = 0; i < 6; ++i)
            glUniformMatrix4fv(vpIDs[i], 1, GL_FALSE, &light.getVP(i)[0][0]);
    else { // TODO: test if this branch is needed.
        glm::mat4 zeroes(0);
        glUniformMatrix4fv(vpIDs[0], 1, GL_FALSE, &light.getVP(0)[0][0]);
        for(int i = 1; i < 6; ++i) // note that we start at one here
            glUniformMatrix4fv(vpIDs[i], 1, GL_FALSE, &zeroes[0][0]);
    }
    glUniform3f(worldspaceLightPositionID, light.getLightPosition().x, light.getLightPosition().y, light.getLightPosition().z);
    glUniform1i(cubeID, light.isDepthMapCube());
    glUniform1f(farID, light.getFar());
}

void ShadowMapTechnique::setScene(Scene *scene) {
    ShadowMapTechnique::scene = scene;

    // delete all old framebuffers
    for(auto iterator: framebuffers)
        delete iterator.second;

    for(auto light: scene->directionalLights)
        if(light->isCastingShadows())
            addFramebuffer(light, DIRECTIONAL_LIGHT_SHADOW_MAP_WIDTH, DIRECTIONAL_LIGHT_SHADOW_MAP_HEIGHT);
    for(auto light: scene->pointLights)
        if(light->isCastingShadows())
            addFramebuffer(light, POINT_LIGHT_SHADOW_MAP_SIZE, POINT_LIGHT_SHADOW_MAP_SIZE);
    for(auto light: scene->spotLights)
        if(light->isCastingShadows())
            addFramebuffer(light, SPOT_LIGHT_SHADOW_MAP_WIDTH, SPOT_LIGHT_SHADOW_MAP_HEIGHT);
}

void ShadowMapTechnique::addFramebuffer(LightWithShadowMap *light, int width, int height) {
    Texture *depthMap; // will be deleted in the framebuffer, so we don't need to worry about that here.
    Framebuffer *depthBuffer;
    if(light->isDepthMapCube()) {
        depthMap = new DepthMapCubeTexture(width);
        depthBuffer = new CubeFramebuffer(depthMap, GL_DEPTH_ATTACHMENT);
    } else {
        depthMap = new DepthMapTexture(width, height);
        depthBuffer = new Framebuffer(depthMap, GL_DEPTH_ATTACHMENT);
    }

    depthBuffer->init(false, false);
    framebuffers[light] = depthBuffer;
}

ShadowMapTechnique::~ShadowMapTechnique() {
    for(auto iterator: framebuffers)
        delete iterator.second;
}
