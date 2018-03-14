//
// Created by Milan van Zanten on 09.02.18.
//

#ifndef BESTEST_GAME_FILELOADER_H
#define BESTEST_GAME_FILELOADER_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class FileLoader {
public:
    /// Load text from a file.
    /// \param path The relative path of the file to read.
    /// \param size A pointer to a variable to store the file size in or NULL if the file size doesn't matter.
    /// \return The text in the file as a byte vector.
    static std::vector<char> loadFile(std::string path, size_t *size = nullptr);
    /// Load text from a file as a string.
    /// \param path The relative path of the file to read.
    /// \param size A pointer to a variable to store the file size in or NULL if the file size doesn't matter.
    /// \return The text in the file as a string.
    static std::string loadFileAsString(std::string path, size_t *size = nullptr);
    /// Load vertex, uv and normal data from an .obj file.
    /// \param path The path to the .obj file to load.
    /// \param vertexData A reference to the vertex data vector to fill.
    /// \param normalData A reference to the normal data vector to fill.
    /// \param uvData A reference to the uv data vector to fill.
    /// \param colorData A reference to the color data vector to fill.
    /// \param vertexIndices A reference to the vertex index vector to fill.
    /// \param normalIndices A reference to the normal index vector to fill.
    /// \param uvIndices A reference to the uv index vector to fill.
    /// \param colorIndices A reference to the color index vector to fill.
    static void loadOBJ(const std::string &path,
                        std::vector<float> &vertexData,
                        std::vector<glm::vec3> &normalData,
                        std::vector<glm::vec2> &uvData,
                        std::vector<glm::vec3> &colorData,
                        std::vector<unsigned int> &vertexIndices,
                        std::vector<unsigned int> &normalIndices,
                        std::vector<unsigned int> &uvIndices,
                        std::vector<unsigned int> &colorIndices);
};

#endif //BESTEST_GAME_FILELOADER_H
