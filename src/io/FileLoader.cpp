//
// Created by Milan van Zanten on 09.02.18.
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
//

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "FileLoader.h"
#include "../exception/Exception.h"
#include "../util/Log.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>

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

void FileLoader::loadPNG(const std::string &path,
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

void FileLoader::freePNG(unsigned char *imageData) {
    stbi_image_free(imageData);
}

#pragma clang diagnostic pop