//
// Created by Milan van Zanten on 12.02.18.
//

#ifndef BESTEST_GAME_MESH_H
#define BESTEST_GAME_MESH_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

class Mesh {
public:
    void loadFromOBJ(const std::string &path);

private:
    std::vector<unsigned int> vertexIndices, normalIndices, uvIndices, colorIndices;
    std::vector<glm::vec3> vertexData, normalData, colorData;
    std::vector<glm::vec2> uvData;
};

#endif //BESTEST_GAME_MESH_H
