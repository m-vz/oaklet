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
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // fonts
    debugFont = new BitmapFont("../assets/fonts/bitmap/font_bitmap.gif", glm::vec2(8, 8), glm::vec2(8, 8),
                                 {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5',
                                  '6', '7', '8', '9', ':', ';', '#', '%', '+', '-', '/', '\\', '!', '?', '.', ',',
                                  '\'', '*', '(', ')', '[', ']', '{', '}', '<', '>', '"', '=', ' ', '_', '|', '\0'});

    // gui
    framerateTextElement = new TextElement("0.0", debugFont, glm::vec2(10), glm::vec3(1), 4);

    // techniques
    lighting.init();
    shadowing.init();
    toneMapping.init();
    toneMapping.setExposure(&exposure);
    debugText.init();
    debugText.setTextElement(framerateTextElement);
    gBufferDebug.init();

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
    debugText.setViewportSize(width, height);
    gBufferDebug.setViewportSize(width, height);

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

    /*
     * render scene
     */
    shadowing.execute();
    lighting.execute();
    toneMapping.execute();

    /*
     * debug
     */
    debugText.execute();
//    gBufferDebug.execute();

    glfwSwapBuffers(window);
}

void Renderer::setFramerate(float framerate) {
    auto framerateTextBytes = static_cast<size_t>(snprintf(nullptr, 0, "%.1f", framerate)) + 1; // + 1 for the '\0' // NOLINT(bugprone-misplaced-widening-cast)
    auto *framerateText = (char *) malloc(framerateTextBytes);
    snprintf(framerateText, framerateTextBytes, "%.2f", framerate);
    framerateTextElement->setText(framerateText);
}

GLFWwindow* Renderer::getWindow() const {
    return window;
}

Renderer::~Renderer() {
    delete debugFont;
    delete framerateTextElement;

    glDeleteVertexArrays(1, &vertexArrayID);

    glfwTerminate();
}
