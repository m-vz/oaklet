//
// Created by Milan van Zanten on 09.02.18.
//

#define TINYOBJLOADER_IMPLEMENTATION

#include <tiny_obj_loader.h>
#include "FileLoader.h"
#include "../exception/Exception.h"
#include "../util/Log.h"

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
                         std::vector<glm::vec3> &vertexData,
                         std::vector<glm::vec3> &normalData,
                         std::vector<glm::vec2> &uvData,
                         std::vector<glm::vec3> &colorData,
                         std::vector<unsigned int> &vertexIndices,
                         std::vector<unsigned int> &normalIndices,
                         std::vector<unsigned int> &uvIndices,
                         std::vector<unsigned int> &colorIndices) {
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

    // vertex data
    for(int i = 0; i + 2 < attrib.vertices.size(); i += 3)
        vertexData.emplace_back(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);

    // normal data
    for(int i = 0; i + 2 < attrib.normals.size(); i += 3)
        normalData.emplace_back(attrib.normals[i], attrib.normals[i + 1], attrib.normals[i + 2]);

    // uv data
    for(int i = 0; i + 1 < attrib.texcoords.size(); i += 2)
        uvData.emplace_back(attrib.texcoords[i], attrib.texcoords[i + 1]);

    // color data
    for(int i = 0; i + 2 < attrib.colors.size(); i += 3)
        colorData.emplace_back(attrib.colors[i], attrib.colors[i + 1], attrib.colors[i + 2]);

    // loop over shapes
    for(auto &shape: shapes) {
        size_t offset = 0; // offset in the index vector

        // loop over faces (triangles by default)
        for(int f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
            int fv = shape.mesh.num_face_vertices[f];

            // loop over vertices (fv is always 3 by default)
            for (int v = 0; v < fv; ++v) {
                index = shape.mesh.indices[offset + v];
                vertexIndices.push_back((unsigned int) index.vertex_index);
                normalIndices.push_back((unsigned int) index.normal_index);
                uvIndices.push_back((unsigned int) index.texcoord_index);
                colorIndices.push_back((unsigned int) index.vertex_index);
            }

            offset += fv;
        }
    }
}
