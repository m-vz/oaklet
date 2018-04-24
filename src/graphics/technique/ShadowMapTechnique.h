//
// Created by Milan van Zanten on 19.04.18.
//

#ifndef BESTEST_GAME_SHADOWMAPTECHNIQUE_H
#define BESTEST_GAME_SHADOWMAPTECHNIQUE_H

#include <glm/glm.hpp>
#include "Technique.h"
#include "../../world/Scene.h"
#include "../Framebuffer.h"
#include "../light/LightType.h"

class ShadowMapTechnique : public Technique {
public:
    static const int SPOT_LIGHT_SHADOW_MAP_WIDTH = 1024, SPOT_LIGHT_SHADOW_MAP_HEIGHT = 1024;
    static const int DIRECTIONAL_LIGHT_SHADOW_MAP_WIDTH = 8192, DIRECTIONAL_LIGHT_SHADOW_MAP_HEIGHT = 8192;

    void init() override;
    void execute() override;
    virtual void setMVP(const glm::mat4 &mvp);
    void setScene(Scene *scene);
    ~ShadowMapTechnique() override;

private:
    std::unordered_map<LightWithShadowMap*, Framebuffer*> framebuffers;
    Scene *scene;

    GLuint mvpID;

    void renderDepthMap(LightWithShadowMap *light);
    void addFramebuffer(LightWithShadowMap *light, int width, int height);
};

#endif //BESTEST_GAME_SHADOWMAPTECHNIQUE_H
