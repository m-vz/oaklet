//
// Created by Milan van Zanten on 17.04.18.
//

#ifndef BESTEST_GAME_SIMPLELIGHTINGTECHNIQUE_H
#define BESTEST_GAME_SIMPLELIGHTINGTECHNIQUE_H

#include "LightingTechnique.h"
#include "../light/PointLight.h"
#include "../light/DirectionalLight.h"
#include "../light/SpotLight.h"

class SimpleLightingTechnique : public LightingTechnique { // NOLINT
public:
    // TODO: currently, these also need to be changed in the fragment shader. stupid
    static const unsigned int MAX_DIRECTIONAL_LIGHTS = 1;
    static const unsigned int MAX_POINT_LIGHTS = 4;
    static const unsigned int MAX_SPOT_LIGHTS = 4;

    void init() override;
    void setDirectionalLights(unsigned long lightCount, const std::vector<DirectionalLight*> &lights);
    void setPointLights(unsigned long lightCount, const std::vector<PointLight*> &lights);
    void setSpotLights(unsigned long lightCount, const std::vector<SpotLight*> &lights);
    void setView(const glm::mat4 &view);

private:
    GLuint viewID;

    GLuint directionalLightCountID;
    struct {
        GLuint directionID;
        GLuint colorID;
        GLuint powerID;
    } directionalLights[MAX_DIRECTIONAL_LIGHTS];

    GLuint pointLightCountID;
    struct {
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
};

#endif //BESTEST_GAME_SIMPLELIGHTINGTECHNIQUE_H
