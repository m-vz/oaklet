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

const glm::mat4 &LightWithShadowMap::getView(int index) const {
    return views[index];
}

const glm::mat4 &LightWithShadowMap::getProjection() const {
    return projection;
}

const glm::mat4 &LightWithShadowMap::getVP(int index) const {
    return vps[index];
}

float LightWithShadowMap::getNear() const {
    return near;
}

float LightWithShadowMap::getFar() const {
    return far;
}

bool LightWithShadowMap::isDepthMapCube() const {
    return depthMapIsCube;
}
