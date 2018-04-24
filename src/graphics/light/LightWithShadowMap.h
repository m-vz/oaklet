//
// Created by Milan van Zanten on 24.04.18.
//

#ifndef BESTEST_GAME_LIGHTWITHSHADOWMAP_H
#define BESTEST_GAME_LIGHTWITHSHADOWMAP_H

#include <glm/ext.hpp>
#include "../texture/Texture.h"

class LightWithShadowMap {
public:
    virtual void calculateVP()= 0;
    Texture *getShadowMap() const;
    void setShadowMap(Texture *shadowMap);
    const glm::mat4 &getView() const;
    const glm::mat4 &getProjection() const;
    const glm::mat4 &getVP() const;

protected:
    glm::mat4 view, projection, vp;

private:
    Texture *shadowMap;
};

#endif //BESTEST_GAME_LIGHTWITHSHADOWMAP_H
