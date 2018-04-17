//
// Created by Milan van Zanten on 09.02.18.
//

#ifndef BESTEST_GAME_RENDERER_H
#define BESTEST_GAME_RENDERER_H

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>
#include "model/Texture.h"
#include "../io/Mouse.h"
#include "camera/FreeCamera.h"
#include "BitmapFont.h"
#include "model/Model.h"
#include "../world/Scene.h"
#include "technique/SimpleLightingTechnique.h"

class Renderer {
public:
    FreeCamera *camera;

    BitmapFont *font;

    Renderer();
    void init(int width, int height);
    void renderScene(Scene *scene, long long int lag);

    GLFWwindow *getWindow() const;
    virtual ~Renderer();

private:
    bool initialized = false;
    SimpleLightingTechnique lighting;
    GLuint vertexArrayID;
    GLFWwindow *window;
};

#endif //BESTEST_GAME_RENDERER_H
