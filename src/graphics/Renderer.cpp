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
        throw Exception("Could not initialize glfw."); // NOLINT
}

static void glfwErrorCallback(int error, const char* description) {
    Log::log << LOG_ERROR << description << ". Code: " << error;
    throw Exception(description, error); // NOLINT
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
        throw Exception("Could not open GLFW window."); // NOLINT
    }
    glfwMakeContextCurrent(window);

    // initialize glew
    glewExperimental = static_cast<GLboolean>(true); // necessary in core profile
    if(glewInit() != GLEW_OK)
        throw Exception("Could not initialize glew."); // NOLINT

    // shaders
    shader = FileLoader::loadShaders("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
    flatShader = FileLoader::loadShaders("assets/shaders/flat_vertex.glsl", "assets/shaders/flat_fragment.glsl");

    // VAO and vertex buffer
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // font
    font = new BitmapFont("assets/fonts/bitmap/font_bitmap.bmp", glm::vec2(8, 8), glm::vec2(16, 16),
                          {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                           'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5',
                           '6', '7', '8', '9', ':', ';', '#', '%', '+', '-', '/', '\\', '!', '?', '.', ',',
                           '\'', '*', '(', ')', '[', ']', '{', '}', '<', '>', '"', '=', ' ', '_', '|', '\0'});

    // uniforms
    modelMatrixID = static_cast<GLuint>(glGetUniformLocation(shader, "model"));
    viewID = static_cast<GLuint>(glGetUniformLocation(shader, "view"));
    projectionID = static_cast<GLuint>(glGetUniformLocation(shader, "projection"));
    lightPositionID = static_cast<GLuint>(glGetUniformLocation(shader, "worldspaceLightPosition"));
    diffuseTextureSampler = static_cast<GLuint>(glGetUniformLocation(shader, "diffuseTextureSampler"));
    normalTextureSampler = static_cast<GLuint>(glGetUniformLocation(shader, "normalTextureSampler"));
    specularTextureSampler = static_cast<GLuint>(glGetUniformLocation(shader, "specularTextureSampler"));

    flatModelMatrixID = static_cast<GLuint>(glGetUniformLocation(flatShader, "model"));
    flatViewID = static_cast<GLuint>(glGetUniformLocation(flatShader, "view"));
    flatProjectionID = static_cast<GLuint>(glGetUniformLocation(flatShader, "projection"));
    flatLightPositionID = static_cast<GLuint>(glGetUniformLocation(flatShader, "worldspaceLightPosition"));

    // options
    glClearColor(1, 1, 1, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    initialized = true;
}

void Renderer::render(long long int lag) {
    if(!initialized)
        throw NotInitialisedException("Renderer");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOLINT

    glUseProgram(flatShader);
    glUniformMatrix4fv(flatViewID, 1, GL_FALSE, &camera->getView()[0][0]);
    glUniformMatrix4fv(flatProjectionID, 1, GL_FALSE, &camera->getProjection()[0][0]);
    glUniform3f(flatLightPositionID, lightPositionVector.x, lightPositionVector.y, lightPositionVector.z);
    floor->render(flatModelMatrixID, 0, 0, 0);

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    font->renderTextUppercase("hello world!", glm::vec2(20, windowHeight - 20), glm::vec2(windowWidth, windowHeight), glm::vec3(1, 0, 0.2f), 1.0f);

    glfwSwapBuffers(window);
}

GLFWwindow* Renderer::getWindow() const {
    return window;
}

Renderer::~Renderer() {
    delete floor;

    glDeleteProgram(shader);
    glDeleteProgram(flatShader);
    glDeleteVertexArrays(1, &vertexArrayID);

    glfwTerminate();
}
