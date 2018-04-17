//
// Created by Milan van Zanten on 17.04.18.
//

#include "SimpleLightingTechnique.h"
#include "../../exception/Exception.h"

void SimpleLightingTechnique::init() {
    LightingTechnique::init();
    addShader(GL_VERTEX_SHADER, "assets/shaders/simple_vertex.glsl");
    addShader(GL_FRAGMENT_SHADER, "assets/shaders/simple_fragment.glsl");
    finalize();

    viewID = getUniformLocation("view");

    modelID = getUniformLocation("model");
    mvpID = getUniformLocation("mvp");

    diffuseTextureSamplerID = getUniformLocation("diffuseTextureSampler");
    normalTextureSamplerID = getUniformLocation("normalTextureSampler");
    specularTextureSamplerID = getUniformLocation("specularTextureSampler");

    directionalLightCountID = getUniformLocation("directionalLightCount");
    for(int i = 0; i < MAX_POINT_LIGHTS; ++i) {
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

void SimpleLightingTechnique::setDirectionalLights(unsigned long lightCount, const std::vector<DirectionalLight *> &lights) {
    if(lightCount > MAX_DIRECTIONAL_LIGHTS)
        throw Exception("Light count cannot be larger than MAX_DIRECTIONAL_LIGHTS."); // NOLINT

    glUniform1i(directionalLightCountID, static_cast<GLint>(lightCount));

    for(int i = 0; i < lightCount; i++) {
        glUniform3f(directionalLights[i].directionID, lights[i]->lightDirection.x, lights[i]->lightDirection.y, lights[i]->lightDirection.z);
        glUniform3f(directionalLights[i].colorID, lights[i]->lightColor.x, lights[i]->lightColor.y, lights[i]->lightColor.z);
        glUniform1f(directionalLights[i].powerID, lights[i]->lightPower);
    }
}

void SimpleLightingTechnique::setPointLights(unsigned long lightCount, const std::vector<PointLight *> &lights) {
    if(lightCount > MAX_POINT_LIGHTS)
        throw Exception("Light count cannot be larger than MAX_POINT_LIGHTS."); // NOLINT

    glUniform1i(pointLightCountID, static_cast<GLint>(lightCount));

    for(int i = 0; i < lightCount; i++) {
        glUniform3f(pointLights[i].positionID, lights[i]->lightPosition.x, lights[i]->lightPosition.y, lights[i]->lightPosition.z);
        glUniform3f(pointLights[i].colorID, lights[i]->lightColor.x, lights[i]->lightColor.y, lights[i]->lightColor.z);
        glUniform1f(pointLights[i].powerID, lights[i]->lightPower);
        glUniform1f(pointLights[i].attenuation.exponentialID, lights[i]->attenuation.exponential);
        glUniform1f(pointLights[i].attenuation.linearID, lights[i]->attenuation.linear);
        glUniform1f(pointLights[i].attenuation.constantID, lights[i]->attenuation.constant);
    }
}

void SimpleLightingTechnique::setSpotLights(unsigned long lightCount, const std::vector<SpotLight *> &lights) {
    if(lightCount > MAX_SPOT_LIGHTS)
        throw Exception("Light count cannot be larger than MAX_SPOT_LIGHTS."); // NOLINT

    glUniform1i(spotLightCountID, static_cast<GLint>(lightCount));

    for(int i = 0; i < lightCount; i++) {
        glUniform3f(spotLights[i].positionID, lights[i]->lightPosition.x, lights[i]->lightPosition.y, lights[i]->lightPosition.z);
        glUniform3f(spotLights[i].directionID, lights[i]->lightDirection.x, lights[i]->lightDirection.y, lights[i]->lightDirection.z);
        glUniform3f(spotLights[i].colorID, lights[i]->lightColor.x, lights[i]->lightColor.y, lights[i]->lightColor.z);
        glUniform1f(spotLights[i].powerID, lights[i]->lightPower);
        glUniform1f(spotLights[i].cutoffID, lights[i]->lightCutoff);
        glUniform1f(spotLights[i].attenuation.exponentialID, lights[i]->attenuation.exponential);
        glUniform1f(spotLights[i].attenuation.linearID, lights[i]->attenuation.linear);
        glUniform1f(spotLights[i].attenuation.constantID, lights[i]->attenuation.constant);
    }
}

void SimpleLightingTechnique::setView(const glm::mat4 &view) {
    glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
}
