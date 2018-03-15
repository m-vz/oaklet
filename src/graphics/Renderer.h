//
// Created by Milan van Zanten on 09.02.18.
//

#ifndef BESTEST_GAME_RENDERER_H
#define BESTEST_GAME_RENDERER_H

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>
#include "camera/CirclingCamera.h"
#include "Mesh.h"
#include "Texture.h"

class Renderer {
public:
    CirclingCamera *camera;

    Renderer();
    void init(int width, int height);
    void render(long long int lag);

    GLFWwindow *getWindow() const;

    virtual ~Renderer();

private:
    bool initialized = false;
    GLuint programID;
    GLuint vertexArrayID;
    GLuint modelID, viewID, projectionID;
    GLuint textureSampler;
    GLFWwindow *window;

    Mesh *medievalHouse;
    Texture *medievalHouseTexture;

    GLuint loadShaders(const char *vertexShaderPath, const char *fragmentShaderPath);
};

#endif //BESTEST_GAME_RENDERER_H
