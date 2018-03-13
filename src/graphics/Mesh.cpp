//
// Created by Milan van Zanten on 12.02.18.
//

#include "Mesh.h"
#include "../io/FileLoader.h"

void Mesh::loadFromOBJ(const std::string &path) {
    FileLoader::loadOBJ(path, vertexData, normalData, uvData, colorData, vertexIndices, normalIndices, uvIndices, colorIndices);
}
