//
// Created by Milan van Zanten on 09.02.18.
//

#ifndef BESTEST_GAME_RENDERER_H
#define BESTEST_GAME_RENDERER_H

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>
#include "texture/Texture.h"
#include "../io/Mouse.h"
#include "camera/FreeCamera.h"
#include "BitmapFont.h"
#include "model/Model.h"
#include "../world/Scene.h"
#include "technique/SimpleLightingTechnique.h"
#include "technique/ShadowMapTechnique.h"
#include "technique/ToneMappingTechnique.h"

class Renderer {
public:
    SimpleLightingTechnique lighting;
    ShadowMapTechnique shadowing;
    ToneMappingTechnique toneMapping;

    Renderer();
    void init(int width, int height);
    void sizeChanged(int width, int height);
    void renderScene(Scene *scene, long long int lag);

    GLFWwindow *getWindow() const;
    virtual ~Renderer();

private:
    bool initialized = false;
    GLuint vertexArrayID, quadVertexArrayID;
    GLFWwindow *window;
};

#endif //BESTEST_GAME_RENDERER_H
