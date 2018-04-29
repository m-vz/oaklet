//
// Created by Milan van Zanten on 17.04.18.
//

#ifndef BESTEST_GAME_SIMPLELIGHTINGTECHNIQUE_H
#define BESTEST_GAME_SIMPLELIGHTINGTECHNIQUE_H

#include "../light/PointLight.h"
#include "../light/DirectionalLight.h"
#include "../light/SpotLight.h"
#include "Technique.h"
#include "../../world/Scene.h"
#include "SkyboxTechnique.h"

class SimpleLightingTechnique : public Technique {
public:
    // TODO: currently, these also need to be changed in the vertex and fragment shader. stupid
    static const unsigned int MAX_DIRECTIONAL_LIGHTS = 1;
    static const unsigned int MAX_POINT_LIGHTS = 4;
    static const unsigned int MAX_SPOT_LIGHTS = 4;

    void init() override;
    void execute() override;
    void setScene(Scene *scene);
    void setViewportSize(int width, int height);
    ~SimpleLightingTechnique() override;

private:
    int viewportWidth = 0, viewportHeight = 0;
    int shadowMapTextureUnit = 0;

    SkyboxTechnique *skyboxTechnique;

    Scene *scene;

    Texture *blackTexture, *whiteTexture, *outwardsTexture;

    GLuint blaID;

    GLuint viewID;
    GLuint modelID;
    GLuint mvpID;

    GLuint diffuseTextureSamplerID = 0;
    GLuint normalTextureSamplerID = 0;
    GLuint specularTextureSamplerID = 0;

    GLuint directionalLightCountID;
    struct {
        GLuint vpID;
        GLuint shadowMapTextureSamplerID;
        GLuint directionID;
        GLuint colorID;
        GLuint powerID;
    } directionalLights[MAX_DIRECTIONAL_LIGHTS];

    GLuint pointLightCountID;
    struct {
        GLuint farID;
        GLuint shadowMapTextureSamplerID;
        GLuint positionID;
        GLuint colorID;
        GLuint powerID;
        struct {
            GLuint exponentialID;
            GLuint linearID;
            GLuint constantID;
        } attenuation;
    } pointLights[MAX_POINT_LIGHTS];

    GLuint spotLightCountID;
    struct {
        GLuint vpID;
        GLuint shadowMapTextureSamplerID;
        GLuint positionID;
        GLuint directionID;
        GLuint colorID;
        GLuint powerID;
        GLuint cutoffID;
        struct {
            GLuint exponentialID;
            GLuint linearID;
            GLuint constantID;
        } attenuation;
    } spotLights[MAX_POINT_LIGHTS];

    void setDirectionalLights(unsigned long lightCount, const std::vector<DirectionalLight*> &lights);
    void setPointLights(unsigned long lightCount, const std::vector<PointLight*> &lights);
    void setSpotLights(unsigned long lightCount, const std::vector<SpotLight*> &lights);
    void setDiffuseTextureSampler(GLuint textureSampler);
    void setNormalTextureSampler(GLuint textureSampler);
    void setSpecularTextureSampler(GLuint textureSampler);
    void setView(const glm::mat4 &view);
    void setModel(const glm::mat4 &model);
    void setMVP(const glm::mat4 &mvp);
};

#endif //BESTEST_GAME_SIMPLELIGHTINGTECHNIQUE_H
