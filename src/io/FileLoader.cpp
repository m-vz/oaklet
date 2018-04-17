//
// Created by Milan van Zanten on 09.02.18.
//

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "FileLoader.h"
#include "../exception/Exception.h"
#include "../util/Log.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::vector<char> FileLoader::loadFile(const std::string path, size_t *size) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);

    if(!file.is_open())
        throw IOException("Could not open file: " + path);

    auto fileSize = static_cast<size_t>(file.tellg());
    if(size != nullptr)
        *size = fileSize;

    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    buffer.push_back('\0'); // add a null terminator
    file.close();

    return buffer;
}

std::string FileLoader::loadFileAsString(const std::string path, size_t *size) {
    std::vector<char> data = loadFile(path, size);
    return std::string(data.begin(), data.end());
}

GLuint FileLoader::loadShaders(const char *vertexShaderPath, const char *fragmentShaderPath) {
    // Create the shaders
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // READ

    // read the shader codes from file
    std::string vertexShaderCode = loadFileAsString(vertexShaderPath);
    std::string fragmentShaderCode = loadFileAsString(fragmentShaderPath);

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
        std::vector<char> programErrorMessage(static_cast<unsigned long>(infoLogLength + 1));
        glGetProgramInfoLog(programID, infoLogLength, nullptr, &programErrorMessage[0]);
        Log::log << LOG_ERROR << &programErrorMessage[0];
    }

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}

const aiScene *FileLoader::loadModel(Assimp::Importer &importer, const std::string &path) {
    const aiScene* aiScene = importer.ReadFile(path.c_str(),
                                               aiProcess_CalcTangentSpace | // NOLINT
                                               aiProcess_FindInvalidData |
                                               /*aiProcess_FlipUVs |*/ // textures are being flipped in the stb image loader
                                               aiProcess_GenSmoothNormals |
                                               aiProcess_GenUVCoords |
                                               aiProcess_ImproveCacheLocality |
                                               aiProcess_JoinIdenticalVertices |
                                               aiProcess_OptimizeGraph |
                                               aiProcess_OptimizeMeshes |
                                               aiProcess_SortByPType |
                                               aiProcess_TransformUVCoords |
                                               aiProcess_Triangulate);

    if(!aiScene || aiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiScene->mRootNode) // NOLINT
        throw IOException(importer.GetErrorString());
    else
        return aiScene;
}

void FileLoader::loadOBJ(const std::string &path,
                         std::vector<float> &vertexData,
                         std::vector<float> &normalData,
                         std::vector<float> &uvData,
                         std::vector<float> &colorData) {
    tinyobj::index_t index; // NOLINT
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string error;
    bool result = tinyobj::LoadObj(&attrib, &shapes, &materials, &error, path.c_str());

    if(!error.empty())
        Log::log << LOG_ERROR << error;

    if(!result)
        throw IOException("Error while loading .obj file.");

    // loop over shapes
    for(auto &shape: shapes) {
        int offset = 0; // offset in the index vector

        // loop over faces (triangles by default)
        for(int f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
            int fv = shape.mesh.num_face_vertices[f];

            // loop over vertices (fv is always 3 by default)
            for (int v = 0; v < fv; ++v) {
                index = shape.mesh.indices[offset];

                if(index.vertex_index >= 0) {
                    vertexData.push_back(attrib.vertices[3*index.vertex_index]);
                    vertexData.push_back(attrib.vertices[3*index.vertex_index + 1]);
                    vertexData.push_back(attrib.vertices[3*index.vertex_index + 2]);
                }
                if(index.normal_index >= 0) {
                    normalData.push_back(attrib.normals[3*index.normal_index]);
                    normalData.push_back(attrib.normals[3*index.normal_index + 1]);
                    normalData.push_back(attrib.normals[3*index.normal_index + 2]);
                }
                if(index.texcoord_index >= 0) {
                    uvData.push_back(attrib.texcoords[2*index.texcoord_index]);
                    uvData.push_back(attrib.texcoords[2*index.texcoord_index + 1]);
                }
                if(index.vertex_index >= 0) {
                    colorData.push_back(attrib.colors[3*index.vertex_index]);
                    colorData.push_back(attrib.colors[3*index.vertex_index + 1]);
                    colorData.push_back(attrib.colors[3*index.vertex_index + 2]);
                }

                offset++;
            }
        }
    }
}

void FileLoader::loadImage(const std::string &path,
                           unsigned char **imageData,
                           int *imageWidth, int *imageHeight,
                           int *numberOfChannels, int desiredNumberOfChannels) {
    int foo; // used to store the unused information somewhere.
    if(imageWidth == nullptr)
        imageWidth = &foo;
    if(imageHeight == nullptr)
        imageHeight = &foo;
    if(numberOfChannels == nullptr)
        numberOfChannels = &foo;

    stbi_set_flip_vertically_on_load(true);

    *imageData = stbi_load(path.c_str(), imageWidth, imageHeight, numberOfChannels, desiredNumberOfChannels);
    if(*imageData == nullptr)
        throw IOException(stbi_failure_reason());
}

void FileLoader::freeImage(unsigned char *imageData) {
    stbi_image_free(imageData);
}