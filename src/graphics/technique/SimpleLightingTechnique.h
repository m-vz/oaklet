//
// Created by Milan van Zanten on 17.04.18.
//

#ifndef BESTEST_GAME_SIMPLELIGHTINGTECHNIQUE_H
#define BESTEST_GAME_SIMPLELIGHTINGTECHNIQUE_H

#include "LightingTechnique.h"
#include "../light/PointLight.h"

class SimpleLightingTechnique : public LightingTechnique { // NOLINT
public:
    static const unsigned int MAX_POINT_LIGHTS = 8; // TODO: currently, this also needs to be changed in the fragment shader. stupid

    void init() override;
    void setPointLights(unsigned long lightCount, const std::vector<PointLight*> &lights);
    void setView(const glm::mat4 &view);

private:
    GLuint viewID;

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
};

#endif //BESTEST_GAME_SIMPLELIGHTINGTECHNIQUE_H
