//
// Created by Milan van Zanten on 09.02.18.
//

#include "Renderer.h"
#include "../io/FileLoader.h"
#include "../exception/Exception.h"
#include "../util/Log.h"
#include "model/MeshFactory.h"

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

    // VAO and vertex buffer
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // techniques
    lighting.init();
    shadowing.init();

    // font
    font = new BitmapFont("assets/fonts/bitmap/font_bitmap.bmp", glm::vec2(8, 8), glm::vec2(16, 16),
                          {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                           'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5',
                           '6', '7', '8', '9', ':', ';', '#', '%', '+', '-', '/', '\\', '!', '?', '.', ',',
                           '\'', '*', '(', ')', '[', ']', '{', '}', '<', '>', '"', '=', ' ', '_', '|', '\0'});

    // options
    glClearColor(0, 0, 0, 1);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glDepthFunc(GL_LESS);

    initialized = true;
}

void Renderer::renderScene(Scene *scene, long long int lag) {
    if(!initialized)
        throw NotInitialisedException("Renderer");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shadowing.execute();
//    scene->entities[0]->getModel()->setMeshTexture(1, TEXTURE_DIFFUSE, scene->directionalLights[0]->getShadowMap());

    lighting.execute();

    glfwSwapBuffers(window);
}

GLFWwindow* Renderer::getWindow() const {
    return window;
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &vertexArrayID);

    glfwTerminate();
}
