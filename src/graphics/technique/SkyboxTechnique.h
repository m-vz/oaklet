//
// Created by Milan van Zanten on 25.04.18.
//

#ifndef BESTEST_GAME_SKYBOXTECHNIQUE_H
#define BESTEST_GAME_SKYBOXTECHNIQUE_H

#include "Technique.h"
#include "../../world/Skybox.h"
#include <glm/glm.hpp>

class SkyboxTechnique : public Technique {
public:
    void init() override;
    void execute() override;
    void setSkybox(Skybox *skybox);
    void setVP(const glm::mat4 &view, const glm::mat4 &projection);

private:
    Skybox *skybox;

    GLuint vpID;
    GLuint cubeMapTextureSamplerID = 0;
};

#endif //BESTEST_GAME_SKYBOXTECHNIQUE_H
