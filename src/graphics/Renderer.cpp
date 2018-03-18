//
// Created by Milan van Zanten on 09.02.18.
//

#include "Renderer.h"
#include "../io/FileLoader.h"
#include "../exception/Exception.h"
#include "../util/Log.h"

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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
    programID = FileLoader::loadShaders("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");

    // VAO and vertex buffer
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    medievalHouseDiffuseTexture = new Texture("assets/meshes/medieval_house/textures/medieval_house_diffuse.png");
    medievalHouseDiffuseTexture->bindTexture(GL_TEXTURE0);
    medievalHouseDiffuseTexture->fillTexture();
    medievalHouseNormalTexture = new Texture("assets/meshes/medieval_house/textures/medieval_house_normal.png");
    medievalHouseNormalTexture->bindTexture(GL_TEXTURE1);
    medievalHouseNormalTexture->fillTexture();
    medievalHouseSpecularTexture = new Texture("assets/meshes/medieval_house/textures/medieval_house_specular.png");
    medievalHouseSpecularTexture->bindTexture(GL_TEXTURE2);
    medievalHouseSpecularTexture->fillTexture();
    medievalHouse = new Mesh("assets/meshes/medieval_house/medieval_house.obj");
    medievalHouse->bindVertexBuffer();
    medievalHouse->fillVertexBuffer();
    medievalHouse->bindUVBuffer();
    medievalHouse->fillUVBuffer();

    // font
    font = new BitmapFont("assets/fonts/bitmap/font_bitmap.bmp", glm::vec2(8, 8), glm::vec2(16, 16),
                          {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                           'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5',
                           '6', '7', '8', '9', ':', ';', '#', '%', '+', '-', '/', '\\', '!', '?', '.', ',',
                           '\'', '*', '(', ')', '[', ']', '{', '}', '<', '>', '"', '=', ' ', '_', '|', '\0'});

    // uniforms
    modelID = static_cast<GLuint>(glGetUniformLocation(programID, "model"));
    viewID = static_cast<GLuint>(glGetUniformLocation(programID, "view"));
    projectionID = static_cast<GLuint>(glGetUniformLocation(programID, "projection"));
    lightDirectionID = static_cast<GLuint>(glGetUniformLocation(programID, "worldspaceLightDirection"));
    medievalHouseDiffuseTextureSampler = static_cast<GLuint>(glGetUniformLocation(programID, "medievalHouseDiffuseTextureSampler"));
    medievalHouseNormalTextureSampler = static_cast<GLuint>(glGetUniformLocation(programID, "medievalHouseNormalTextureSampler"));
    medievalHouseSpecularTextureSampler = static_cast<GLuint>(glGetUniformLocation(programID, "medievalHouseSpecularTextureSampler"));

    // options
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    initialized = true;
}

void Renderer::render(long long int lag) {
    if(!initialized)
        throw NotInitialisedException("Renderer");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);

    medievalHouse->bindVertexBuffer();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0); // NOLINT

    medievalHouse->bindUVBuffer();
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0); // NOLINT

    medievalHouseDiffuseTexture->bindTexture(GL_TEXTURE0);
    glUniform1i(medievalHouseDiffuseTextureSampler, 0);

    medievalHouseNormalTexture->bindTexture(GL_TEXTURE1);
    glUniform1i(medievalHouseNormalTextureSampler, 1);

    medievalHouseSpecularTexture->bindTexture(GL_TEXTURE2);
    glUniform1i(medievalHouseSpecularTextureSampler, 2);

    glUniformMatrix4fv(modelID, 1, GL_FALSE, &medievalHouse->model[0][0]);
    glUniformMatrix4fv(viewID, 1, GL_FALSE, &camera->getView()[0][0]);
    glUniformMatrix4fv(projectionID, 1, GL_FALSE, &camera->getProjection()[0][0]);
    glUniform3f(lightDirectionID, lightDirectionVector.x, lightDirectionVector.y, lightDirectionVector.z);

    glDrawElements(
            GL_TRIANGLES, // mode
            static_cast<GLsizei>(medievalHouse->indices.size()), // count
            GL_UNSIGNED_INT, // type
            (void*) 0 // element array buffer offset // NOLINT
    );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    font->renderTextUppercase("hello world!", glm::vec2(20, windowHeight - 20), glm::vec2(windowWidth, windowHeight), glm::vec3(1, 0, 0.2f), 1.0f);
    font->renderTextUppercase("hello world2!", glm::vec2(20, 20 + font->getCharacterSize().y), glm::vec2(windowWidth, windowHeight), glm::vec3(1, 0, 0.2f), 1.0f);

    glfwSwapBuffers(window);
}

GLFWwindow* Renderer::getWindow() const {
    return window;
}

Renderer::~Renderer() {
    delete medievalHouse;
    delete medievalHouseDiffuseTexture;

    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &vertexArrayID);

    glfwTerminate();
}
