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

    pointLightCountID = getUniformLocation("pointLightCount");
    for(int i = 0; i < MAX_POINT_LIGHTS; ++i) {
        pointLights[i].positionID = getUniformLocation("pointLights[" + std::to_string(i) + "].position");
        pointLights[i].colorID = getUniformLocation("pointLights[" + std::to_string(i) + "].color");
        pointLights[i].powerID = getUniformLocation("pointLights[" + std::to_string(i) + "].power");
        pointLights[i].attenuation.exponentialID = getUniformLocation("pointLights[" + std::to_string(i) + "].attenuation.exponential");
        pointLights[i].attenuation.linearID = getUniformLocation("pointLights[" + std::to_string(i) + "].attenuation.linear");
        pointLights[i].attenuation.constantID = getUniformLocation("pointLights[" + std::to_string(i) + "].attenuation.constant");
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

void SimpleLightingTechnique::setView(const glm::mat4 &view) {
    glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
}
