//
// Created by Milan van Zanten on 16.04.18.
//

#include "Technique.h"
#include "../../exception/Exception.h"
#include "../../io/FileLoader.h"
#include "../../util/Log.h"

Technique::Technique() {
    shaderProgram = 0;
}

void Technique::init() {
    shaderProgram = glCreateProgram();
    if(shaderProgram == 0)
        throw Exception("Error creating program."); // NOLINT

    initialized = true;
}

void Technique::enable() {
    if(!initialized)
        throw NotInitialisedException("Technique");

    glUseProgram(shaderProgram);
}

Technique::~Technique() {
    // this is only needed if the technique is deleted before shaders are compiled and finalized
    for(auto shaderObject: shaderObjects)
        glDeleteShader(shaderObject);

    if(shaderProgram != 0)
        glDeleteProgram(shaderProgram);
}

void Technique::addShader(GLenum shaderType, std::string shaderPath) {
    if(!initialized)
        throw NotInitialisedException("Technique");

    // Create the shader
    GLuint shaderObject = glCreateShader(shaderType);
    if(shaderObject == 0)
        throw Exception("Error creating shader."); // NOLINT
    shaderObjects.push_back(shaderObject);

    // READ

    // read the shader codes from file
    std::string shaderCode = FileLoader::loadFileAsString(shaderPath);

    GLint result = GL_FALSE;
    int infoLogLength;

    // COMPILE AND CHECK

    // compile vertex shader
    Log::log << LOG_INFO << "Compiling shader: " << shaderPath;
    char const *sourcePointer = shaderCode.c_str();
    glShaderSource(shaderObject, 1, &sourcePointer , nullptr);
    glCompileShader(shaderObject);

    // check vertex shader
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0) { // the info log contains errors
        std::vector<char> shaderErrorMessage(static_cast<unsigned long>(infoLogLength + 1));
        glGetShaderInfoLog(shaderObject, infoLogLength, nullptr, &shaderErrorMessage[0]);
        Log::log << LOG_ERROR << &shaderErrorMessage[0];
    }

    glAttachShader(shaderProgram, shaderObject);
}

void Technique::finalize() {
    if(!initialized)
        throw NotInitialisedException("Technique");

    GLint result = GL_FALSE;
    int infoLogLength;

    // LINK AND CHECK

    // link the program
    Log::log << LOG_INFO << "Linking shader program.";
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0) {
        std::vector<char> programErrorMessage(static_cast<unsigned long>(infoLogLength + 1));
        glGetProgramInfoLog(shaderProgram, infoLogLength, nullptr, &programErrorMessage[0]);
        Log::log << LOG_ERROR << &programErrorMessage[0];
    }

    // validate the program
    Log::log << LOG_INFO << "Validating shader program.";
    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0) {
        std::vector<char> programErrorMessage(static_cast<unsigned long>(infoLogLength + 1));
        glGetProgramInfoLog(shaderProgram, infoLogLength, nullptr, &programErrorMessage[0]);
        Log::log << LOG_ERROR << &programErrorMessage[0];
    }

    for(auto shaderObject: shaderObjects)
        glDeleteShader(shaderObject);
    shaderObjects.clear();
}

GLuint Technique::getUniformLocation(std::string uniformName) {
    return static_cast<GLuint>(glGetUniformLocation(shaderProgram, uniformName.c_str()));
}

GLint Technique::getProgramParam(GLenum param) {
    GLint value;
    glGetProgramiv(shaderProgram, param, &value);
    return value;
}
