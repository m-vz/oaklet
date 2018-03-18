//
// Created by Milan van Zanten on 09.02.18.
//

#ifndef BESTEST_GAME_RENDERER_H
#define BESTEST_GAME_RENDERER_H

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>
#include "Mesh.h"
#include "Texture.h"
#include "../io/Mouse.h"
#include "camera/FreeCamera.h"
#include "BitmapFont.h"

class Renderer {
public:
    FreeCamera *camera;

    glm::vec3 lightDirectionVector = glm::vec3(-10.0f, 500.0f, 0.0f);
    Mesh *medievalHouse;
    GLuint medievalHouseDiffuseTextureSampler, medievalHouseNormalTextureSampler, medievalHouseSpecularTextureSampler;
    Texture *medievalHouseDiffuseTexture, *medievalHouseNormalTexture, *medievalHouseSpecularTexture;

    BitmapFont *font;

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
    GLuint lightDirectionID;
    GLFWwindow *window;
};

#endif //BESTEST_GAME_RENDERER_H
