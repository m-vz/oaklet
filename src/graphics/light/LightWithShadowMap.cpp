//
// Created by Milan van Zanten on 24.04.18.
//

#include "LightWithShadowMap.h"
#include "../texture/DepthMapCubeTexture.h"
#include "../texture/DepthMapTexture.h"

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

Texture *LightWithShadowMap::getShadowMap() const {
    return shadowMap;
}

bool LightWithShadowMap::isCastingShadows() const {
    return castingShadows;
}

void LightWithShadowMap::setShadowMap(Texture *shadowMap) {
    LightWithShadowMap::shadowMap = shadowMap;
}

void LightWithShadowMap::castShadows(bool castingShadows) {
    LightWithShadowMap::castingShadows = castingShadows;

    if(!castingShadows) {
        if(deleteShadowMap)
            delete shadowMap;

        if(isDepthMapCube())
            shadowMap = new DepthMapCubeTexture(1);
        else
            shadowMap = new DepthMapTexture(1, 1);

        deleteShadowMap = true;
    }
}

LightWithShadowMap::~LightWithShadowMap() {
    if(deleteShadowMap)
        delete shadowMap;
}
