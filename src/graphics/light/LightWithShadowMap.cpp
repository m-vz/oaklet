//
// Created by Milan van Zanten on 24.04.18.
//

#include "LightWithShadowMap.h"

Texture *LightWithShadowMap::getShadowMap() const {
    return shadowMap;
}

void LightWithShadowMap::setShadowMap(Texture *shadowMap) {
    LightWithShadowMap::shadowMap = shadowMap;
}

const glm::mat4 &LightWithShadowMap::getView() const {
    return view;
}

const glm::mat4 &LightWithShadowMap::getProjection() const {
    return projection;
}

const glm::mat4 &LightWithShadowMap::getVP() const {
    return vp;
}
