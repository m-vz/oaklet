//
// Created by Milan van Zanten on 09.02.18.
//

#ifndef BESTEST_GAME_RENDERER_H
#define BESTEST_GAME_RENDERER_H

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>

class Renderer {
public:
    Renderer();
    void init();
    void render(long long int lag);

    GLFWwindow *getWindow() const;

    virtual ~Renderer();

private:
    bool initialized = false;
    GLuint programID;
    GLFWwindow *window;

    GLuint loadShaders(const char *vertexShaderPath, const char *fragmentShaderPath);
};

#endif //BESTEST_GAME_RENDERER_H
