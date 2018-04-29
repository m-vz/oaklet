//
// Created by Milan van Zanten on 24.04.18.
//

#ifndef BESTEST_GAME_LIGHTWITHSHADOWMAP_H
#define BESTEST_GAME_LIGHTWITHSHADOWMAP_H

#include <glm/ext.hpp>
#include <vector>
#include "../texture/Texture.h"

class LightWithShadowMap {
public:
    glm::vec3 lightPosition;

    virtual void calculateVP()= 0;
    Texture *getShadowMap() const;
    void setShadowMap(Texture *shadowMap);
    const glm::mat4 &getView(int index = 0) const;
    const glm::mat4 &getProjection() const;
    const glm::mat4 &getVP(int index = 0) const;
    float getNear() const;
    float getFar() const;
    bool isDepthMapCube() const;

protected:
    bool depthMapIsCube = false;
    float near, far;
    glm::mat4 projection;
    std::vector<glm::mat4> views, vps;

private:
    Texture *shadowMap;
};

#endif //BESTEST_GAME_LIGHTWITHSHADOWMAP_H
