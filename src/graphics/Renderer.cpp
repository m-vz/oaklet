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

    camera = new CirclingCamera(width, height, 5, glm::radians(30.0f), 1.0f, glm::vec3(0, 1, 0));

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
    programID = loadShaders("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");

    // VAO and vertex buffer
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    pyramid = new Mesh("assets/meshes/stylized_levi/stylized_levi.obj");
    pyramid->bindVertexBuffer();
    pyramid->fillVertexBuffer();

    // uniforms
    modelID = static_cast<GLuint>(glGetUniformLocation(programID, "model"));
    viewID = static_cast<GLuint>(glGetUniformLocation(programID, "view"));
    projectionID = static_cast<GLuint>(glGetUniformLocation(programID, "projection"));

    // options
    glClearColor(0, 0, 0, 1);

    initialized = true;
}

void Renderer::render(long long int lag) {
    if(!initialized)
        throw NotInitialisedException("Renderer");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);

    glEnableVertexAttribArray(0);

    pyramid->bindVertexBuffer();
    glVertexAttribPointer(
            0, // attribute 0, must match the layout in the shader.
            3, // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0, // stride
            (void*) nullptr // array buffer offset
    );

    glUniformMatrix4fv(modelID, 1, GL_FALSE, &pyramid->model[0][0]);
    glUniformMatrix4fv(viewID, 1, GL_FALSE, &camera->getView()[0][0]);
    glUniformMatrix4fv(projectionID, 1, GL_FALSE, &camera->getProjection()[0][0]);

    glDrawElements(
            GL_TRIANGLES, // mode
            static_cast<GLsizei>(pyramid->vertexIndices.size()), // count
            GL_UNSIGNED_INT, // type
            (void*) nullptr // element array buffer offset
    );

    glDisableVertexAttribArray(0);

    glfwSwapBuffers(window);
}

GLuint Renderer::loadShaders(const char *vertexShaderPath, const char *fragmentShaderPath) {
    // Create the shaders
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // READ

    // read the shader codes from file
    std::string vertexShaderCode = FileLoader::loadFileAsString(vertexShaderPath);
    std::string fragmentShaderCode = FileLoader::loadFileAsString(fragmentShaderPath);

    GLint result = GL_FALSE;
    int infoLogLength;

    // COMPILE AND CHECK

    // compile vertex shader
    Log::log << LOG_INFO << "Compiling shader: " << vertexShaderPath;
    char const *vertexSourcePointer = vertexShaderCode.c_str();
    glShaderSource(vertexShaderID, 1, &vertexSourcePointer , nullptr);
    glCompileShader(vertexShaderID);

    // check vertex shader
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0) {
        std::vector<char> vertexShaderErrorMessage(static_cast<unsigned long>(infoLogLength + 1));
        glGetShaderInfoLog(vertexShaderID, infoLogLength, nullptr, &vertexShaderErrorMessage[0]);
        Log::log << LOG_ERROR << &vertexShaderErrorMessage[0];
    }

    // compile fragment shader
    Log::log << LOG_INFO << "Compiling shader: " << fragmentShaderPath;
    char const * FragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &FragmentSourcePointer , nullptr);
    glCompileShader(fragmentShaderID);

    // check fragment shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0) {
        std::vector<char> fragmentShaderErrorMessage(static_cast<unsigned long>(infoLogLength + 1));
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, nullptr, &fragmentShaderErrorMessage[0]);
        Log::log << LOG_ERROR << &fragmentShaderErrorMessage[0];
    }

    // LINK AND CHECK

    // link the program
    Log::log << LOG_INFO << "Linking shader program.";
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(static_cast<unsigned long>(infoLogLength + 1));
        glGetProgramInfoLog(programID, infoLogLength, nullptr, &ProgramErrorMessage[0]);
        Log::log << LOG_ERROR << &ProgramErrorMessage[0];
    }

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}

GLFWwindow* Renderer::getWindow() const {
    return window;
}

Renderer::~Renderer() {
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &vertexArrayID);

    glfwTerminate();
}
