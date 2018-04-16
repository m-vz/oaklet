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

class Renderer {
public:
    FreeCamera *camera;

    glm::vec3 lightPositionVector = glm::vec3(-5.0f, 10.0f, 5.0f);
    Model *floor;

    BitmapFont *font;

    Renderer();
    void init(int width, int height);
    void render(long long int lag);

    GLFWwindow *getWindow() const;
    virtual ~Renderer();

private:
    bool initialized = false;
    GLuint shader, flatShader;
    GLuint vertexArrayID;
    GLuint modelMatrixID, viewID, projectionID, lightPositionID, diffuseTextureSampler, normalTextureSampler, specularTextureSampler;
    GLuint flatModelMatrixID, flatViewID, flatProjectionID, flatLightPositionID;
    GLFWwindow *window;
};

#endif //BESTEST_GAME_RENDERER_H
