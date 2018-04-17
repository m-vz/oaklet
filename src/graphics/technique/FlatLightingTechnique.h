//
// Created by Milan van Zanten on 16.04.18.
//

#ifndef BESTEST_GAME_FLATLIGHTINGTECHNIQUE_H
#define BESTEST_GAME_FLATLIGHTINGTECHNIQUE_H

#include "../light/PointLight.h"
#include "LightingTechnique.h"

class FlatLightingTechnique : public LightingTechnique { // NOLINT
public:
    static const unsigned int MAX_POINT_LIGHTS = 8; // TODO: currently, this also needs to be changed in the fragment shader. stupid

    void init() override;
    void setPointLights(unsigned long lightCount, const std::vector<PointLight*> &lights);
    void setView(const glm::mat4 &view);
    void setModel(const glm::mat4 &model) override;
    void setMVP(const glm::mat4 &mvp) override;

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

#endif //BESTEST_GAME_FLATLIGHTINGTECHNIQUE_H
