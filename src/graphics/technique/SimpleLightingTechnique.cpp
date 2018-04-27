//
// Created by Milan van Zanten on 17.04.18.
//

#include "SimpleLightingTechnique.h"
#include "../../exception/Exception.h"

void SimpleLightingTechnique::init() {
    Technique::init();
    addShader(GL_VERTEX_SHADER, "assets/shaders/simple_vertex.glsl");
    addShader(GL_GEOMETRY_SHADER, "assets/shaders/simple_geometry.glsl");
    addShader(GL_FRAGMENT_SHADER, "assets/shaders/simple_fragment.glsl");
    finalize();

    skyboxTechnique = new SkyboxTechnique();
    skyboxTechnique->init();

    blackTexture = new Texture("assets/images/pixel_black.png", true);
    blackTexture->fillTexture(false);
    whiteTexture = new Texture("assets/images/pixel_white.png", true);
    whiteTexture->fillTexture(false);
    outwardsTexture = new Texture("assets/images/pixel_outwards.png", false);
    outwardsTexture->fillTexture(false);

    viewID = getUniformLocation("view");

    modelID = getUniformLocation("model");
    mvpID = getUniformLocation("mvp");

    diffuseTextureSamplerID = getUniformLocation("diffuseTextureSampler");
    normalTextureSamplerID = getUniformLocation("normalTextureSampler");
    specularTextureSamplerID = getUniformLocation("specularTextureSampler");

    directionalLightCountID = getUniformLocation("directionalLightCount");
    for(int i = 0; i < MAX_POINT_LIGHTS; ++i) {
        directionalLights[i].vpID = getUniformLocation("directionalLights[" + std::to_string(i) + "].vp");
        directionalLights[i].shadowMapTextureSamplerID = getUniformLocation("directionalLights[" + std::to_string(i) + "].shadowMapTextureSampler");
        directionalLights[i].directionID = getUniformLocation("directionalLights[" + std::to_string(i) + "].direction");
        directionalLights[i].colorID = getUniformLocation("directionalLights[" + std::to_string(i) + "].color");
        directionalLights[i].powerID = getUniformLocation("directionalLights[" + std::to_string(i) + "].power");
    }

    pointLightCountID = getUniformLocation("pointLightCount");
    for(int i = 0; i < MAX_POINT_LIGHTS; ++i) {
        pointLights[i].positionID = getUniformLocation("pointLights[" + std::to_string(i) + "].position");
        pointLights[i].colorID = getUniformLocation("pointLights[" + std::to_string(i) + "].color");
        pointLights[i].powerID = getUniformLocation("pointLights[" + std::to_string(i) + "].power");
        pointLights[i].attenuation.exponentialID = getUniformLocation("pointLights[" + std::to_string(i) + "].attenuation.exponential");
        pointLights[i].attenuation.linearID = getUniformLocation("pointLights[" + std::to_string(i) + "].attenuation.linear");
        pointLights[i].attenuation.constantID = getUniformLocation("pointLights[" + std::to_string(i) + "].attenuation.constant");
    }

    spotLightCountID = getUniformLocation("spotLightCount");
    for(int i = 0; i < MAX_POINT_LIGHTS; ++i) {
        spotLights[i].vpID = getUniformLocation("spotLights[" + std::to_string(i) + "].vp");
        spotLights[i].shadowMapTextureSamplerID = getUniformLocation("spotLights[" + std::to_string(i) + "].shadowMapTextureSampler");
        spotLights[i].positionID = getUniformLocation("spotLights[" + std::to_string(i) + "].position");
        spotLights[i].directionID = getUniformLocation("spotLights[" + std::to_string(i) + "].direction");
        spotLights[i].colorID = getUniformLocation("spotLights[" + std::to_string(i) + "].color");
        spotLights[i].powerID = getUniformLocation("spotLights[" + std::to_string(i) + "].power");
        spotLights[i].cutoffID = getUniformLocation("spotLights[" + std::to_string(i) + "].cutoff");
        spotLights[i].attenuation.exponentialID = getUniformLocation("spotLights[" + std::to_string(i) + "].attenuation.exponential");
        spotLights[i].attenuation.linearID = getUniformLocation("spotLights[" + std::to_string(i) + "].attenuation.linear");
        spotLights[i].attenuation.constantID = getUniformLocation("spotLights[" + std::to_string(i) + "].attenuation.constant");
    }
}

void SimpleLightingTechnique::execute() {
    glViewport(0, 0, viewportWidth, viewportHeight);

    enable();

    setDirectionalLights(scene->directionalLights.size(), scene->directionalLights);
    setPointLights(scene->pointLights.size(), scene->pointLights);
    setSpotLights(scene->spotLights.size(), scene->spotLights);
    shadowMapTextureUnit = 0;

    setView(scene->activeCamera->getView());

    for(auto entity: scene->entities) {
        setModel(entity->getModel()->modelMatrix);
        setMVP(scene->activeCamera->getProjection()*scene->activeCamera->getView()*entity->getModel()->modelMatrix);

        for(auto mesh: entity->getModel()->meshes) {
            mesh->bindBuffer(mesh->vertexBuffer);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            mesh->bindBuffer(mesh->uvBuffer);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
            if(mesh->containsNormalData()) {
                mesh->bindBuffer(mesh->normalBuffer);
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
            }
            if(mesh->containsTangentData()) {
                mesh->bindBuffer(mesh->tangentBuffer);
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
            }
            if(mesh->containsColorData()) {
                mesh->bindBuffer(mesh->colorBuffer);
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, 0);
            }

            mesh->bindBuffer(mesh->indexBuffer, GL_ELEMENT_ARRAY_BUFFER);

            if(mesh->materialIndex >= 0) {
                std::unordered_map<TextureType, Texture*, std::hash<int>> &material = entity->getModel()->materials[mesh->materialIndex];

                if(material.find(TEXTURE_DIFFUSE) != material.end()) {
                    material[TEXTURE_DIFFUSE]->bindTexture(TEXTURE_DIFFUSE);
                    setDiffuseTextureSampler(TEXTURE_DIFFUSE);
                } else {
                    blackTexture->bindTexture(TEXTURE_DIFFUSE);
                    setDiffuseTextureSampler(TEXTURE_DIFFUSE);
                }
                if(material.find(TEXTURE_NORMAL) != material.end()) {
                    material[TEXTURE_NORMAL]->bindTexture(TEXTURE_NORMAL);
                    setNormalTextureSampler(TEXTURE_NORMAL);
                } else {
                    outwardsTexture->bindTexture(TEXTURE_NORMAL);
                    setNormalTextureSampler(TEXTURE_NORMAL);
                }
                if(material.find(TEXTURE_SPECULAR) != material.end()) {
                    material[TEXTURE_SPECULAR]->bindTexture(TEXTURE_SPECULAR);
                    setSpecularTextureSampler(TEXTURE_SPECULAR);
                } else {
                    whiteTexture->bindTexture(TEXTURE_SPECULAR);
                    setSpecularTextureSampler(TEXTURE_SPECULAR);
                }
            } else {
                blackTexture->bindTexture(TEXTURE_DIFFUSE);
                setDiffuseTextureSampler(TEXTURE_DIFFUSE);
                outwardsTexture->bindTexture(TEXTURE_NORMAL);
                setNormalTextureSampler(TEXTURE_NORMAL);
                whiteTexture->bindTexture(TEXTURE_SPECULAR);
                setSpecularTextureSampler(TEXTURE_SPECULAR);
            }

            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            glDisableVertexAttribArray(3);
            glDisableVertexAttribArray(4);
        }
    }

    glDepthFunc(GL_LEQUAL);

    skyboxTechnique->enable();
    skyboxTechnique->setSkybox(scene->skybox);
    skyboxTechnique->setVP(scene->activeCamera->getView(), scene->activeCamera->getProjection());
    skyboxTechnique->execute();

    glDepthFunc(GL_LESS);
}

void SimpleLightingTechnique::setDirectionalLights(unsigned long lightCount, const std::vector<DirectionalLight *> &lights) {
    if(lightCount > MAX_DIRECTIONAL_LIGHTS)
        throw Exception("Light count cannot be larger than MAX_DIRECTIONAL_LIGHTS.");

    glUniform1i(directionalLightCountID, static_cast<GLint>(lightCount));

    DirectionalLight *light;
    for(int i = 0; i < lightCount; i++) {
        light = lights[i];

        glUniformMatrix4fv(directionalLights[i].vpID, 1, GL_FALSE, &light->getVP()[0][0]);
        light->getShadowMap()->bindTexture(TEXTURE_DEPTH + shadowMapTextureUnit + i);
        glUniform1i(directionalLights[i].shadowMapTextureSamplerID, TEXTURE_DEPTH + shadowMapTextureUnit + i);
        glUniform3f(directionalLights[i].directionID, light->lightDirection.x, light->lightDirection.y, light->lightDirection.z);
        glUniform3f(directionalLights[i].colorID, light->lightColor.x, light->lightColor.y, light->lightColor.z);
        glUniform1f(directionalLights[i].powerID, light->lightPower);

        shadowMapTextureUnit++;
    }
}

void SimpleLightingTechnique::setPointLights(unsigned long lightCount, const std::vector<PointLight *> &lights) {
    if(lightCount > MAX_POINT_LIGHTS)
        throw Exception("Light count cannot be larger than MAX_POINT_LIGHTS.");

    glUniform1i(pointLightCountID, static_cast<GLint>(lightCount));

    PointLight *light;
    for(int i = 0; i < lightCount; i++) {
        light = lights[i];

        glUniform3f(pointLights[i].positionID, light->lightPosition.x, light->lightPosition.y, light->lightPosition.z);
        glUniform3f(pointLights[i].colorID, light->lightColor.x, light->lightColor.y, light->lightColor.z);
        glUniform1f(pointLights[i].powerID, light->lightPower);
        glUniform1f(pointLights[i].attenuation.exponentialID, light->attenuation.exponential);
        glUniform1f(pointLights[i].attenuation.linearID, light->attenuation.linear);
        glUniform1f(pointLights[i].attenuation.constantID, light->attenuation.constant);
    }
}

void SimpleLightingTechnique::setSpotLights(unsigned long lightCount, const std::vector<SpotLight *> &lights) {
    if(lightCount > MAX_SPOT_LIGHTS)
        throw Exception("Light count cannot be larger than MAX_SPOT_LIGHTS.");

    glUniform1i(spotLightCountID, static_cast<GLint>(lightCount));

    SpotLight *light;
    for(int i = 0; i < lightCount; i++) {
        light = lights[i];

        glUniformMatrix4fv(spotLights[i].vpID, 1, GL_FALSE, &light->getVP()[0][0]);
        light->getShadowMap()->bindTexture(TEXTURE_DEPTH + shadowMapTextureUnit + i);
        glUniform1i(spotLights[i].shadowMapTextureSamplerID, TEXTURE_DEPTH + shadowMapTextureUnit + i);
        glUniform3f(spotLights[i].positionID, light->lightPosition.x, light->lightPosition.y, light->lightPosition.z);
        glUniform3f(spotLights[i].directionID, light->lightDirection.x, light->lightDirection.y, light->lightDirection.z);
        glUniform3f(spotLights[i].colorID, light->lightColor.x, light->lightColor.y, light->lightColor.z);
        glUniform1f(spotLights[i].powerID, light->lightPower);
        glUniform1f(spotLights[i].cutoffID, light->lightCutoff);
        glUniform1f(spotLights[i].attenuation.exponentialID, light->attenuation.exponential);
        glUniform1f(spotLights[i].attenuation.linearID, light->attenuation.linear);
        glUniform1f(spotLights[i].attenuation.constantID, light->attenuation.constant);

        shadowMapTextureUnit++;
    }
}

void SimpleLightingTechnique::setDiffuseTextureSampler(GLuint textureSampler) {
    glUniform1i(diffuseTextureSamplerID, textureSampler);
}

void SimpleLightingTechnique::setNormalTextureSampler(GLuint textureSampler) {
    glUniform1i(normalTextureSamplerID, textureSampler);
}

void SimpleLightingTechnique::setSpecularTextureSampler(GLuint textureSampler) {
    glUniform1i(specularTextureSamplerID, textureSampler);
}

void SimpleLightingTechnique::setView(const glm::mat4 &view) {
    glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
}

void SimpleLightingTechnique::setModel(const glm::mat4 &model) {
    glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
}

void SimpleLightingTechnique::setMVP(const glm::mat4 &mvp) {
    glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
}

void SimpleLightingTechnique::setScene(Scene *scene) {
    SimpleLightingTechnique::scene = scene;
}

void SimpleLightingTechnique::setViewportSize(int width, int height) {
    viewportWidth = width;
    viewportHeight = height;
}

SimpleLightingTechnique::~SimpleLightingTechnique() {
    delete blackTexture;
    delete whiteTexture;
    delete outwardsTexture;
}
