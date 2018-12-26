//
// Created by Milan van Zanten on 09.02.18.
//

#include "Renderer.h"
#include "../io/FileLoader.h"
#include "../exception/Exception.h"
#include "../util/Log.h"
#include "model/MeshFactory.h"
#include "texture/HDRTexture.h"
#include "texture/DepthStencilTexture.h"

Renderer::Renderer() {
    if(!glfwInit())
        throw Exception("Could not initialize glfw.");
}

static void glfwErrorCallback(int error, const char* description) {
    Log::log << LOG_ERROR << description << ". Code: " << error;
    throw Exception(description, error);
}

void Renderer::init(int width, int height) {
    glfwSetErrorCallback(glfwErrorCallback);

    // set window hints
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
#ifdef __APPLE__ // MacOS requires at least OpenGL version 3.2 core profile with forward compatibility
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
#ifdef BESTEST_GAME_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    // open a window and create its OpenGL context
    window = glfwCreateWindow(width, height, "bestest game", nullptr, nullptr);
    if(window == nullptr) {
        glfwTerminate();
        throw Exception("Could not open GLFW window.");
    }
    glfwMakeContextCurrent(window);

    // initialize glew
    glewExperimental = static_cast<GLboolean>(true); // necessary in core profile
    if(glewInit() != GLEW_OK)
        throw Exception("Could not initialize glew.");

    // VAOs
    glGenVertexArrays(1, &quadVertexArrayID);
    glBindVertexArray(quadVertexArrayID);
    // TODO: use a mesh object to do this (needs work on mesh class):
    float quadVertices[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f
    };
    float quadUVs[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f
    };
    unsigned int quadIndices[] = {
            0, 1, 3,
            0, 3, 2
    };
    GLuint quadVBOs[3]; // vertex data, uv data, index data
    glGenBuffers(3, quadVBOs);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadUVs), quadUVs, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadVBOs[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // techniques
    lighting.init();
    shadowing.init();
    toneMapping.init();
    toneMapping.setQuadVAO(quadVertexArrayID);
    toneMapping.setVAO(vertexArrayID);
    toneMapping.setExposure(&exposure);

    // options
    glClearColor(0, 0, 0, 1);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glDepthFunc(GL_LESS);

    initialized = true;
}

void Renderer::sizeChanged(int width, int height) {
    lighting.setViewportSize(width, height);
    toneMapping.setViewportSize(width, height);

    auto *lightingTargetTexture = new HDRTexture(width, height);
    lightingTargetTexture->fillTexture(false, false, false, GL_CLAMP_TO_EDGE);
    auto *lightingTargetDepthStencilTexture = new DepthStencilTexture(width, height);
    lightingTargetDepthStencilTexture->fillTexture(false, false, false, GL_CLAMP_TO_EDGE);
    auto *lightingTarget = new Framebuffer(lightingTargetTexture, GL_COLOR_ATTACHMENT0);
    lightingTarget->addTexture(lightingTargetDepthStencilTexture, GL_DEPTH_STENCIL_ATTACHMENT);
    lightingTarget->init(true, true);
    lighting.setRenderTarget(lightingTarget);
    toneMapping.setRenderResult(lightingTarget);
}

void Renderer::renderScene(Scene *scene, long long int lag) {
    if(!initialized)
        throw NotInitialisedException("Renderer");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vertexArrayID);

    shadowing.execute();
    lighting.execute();
    toneMapping.execute();

    glfwSwapBuffers(window);
}

GLFWwindow* Renderer::getWindow() const {
    return window;
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &vertexArrayID);

    glfwTerminate();
}
