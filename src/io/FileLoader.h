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
    /// Load a vertex and fragment shader pair from two glsl shader files and return the created program id.
    /// \param vertexShaderPath The path of the vertex shader file.
    /// \param fragmentShaderPath The path of the fragment shader file.
    /// \return The id of the created program.
    static GLuint loadShaders(const char *vertexShaderPath, const char *fragmentShaderPath);
    /// Load vertex, uv and normal data from an .obj file.
    /// \param path The path to the .obj file to load.
    /// \param vertexData A reference to the vertex data vector to fill.
    /// \param normalData A reference to the normal data vector to fill.
    /// \param uvData A reference to the uv data vector to fill.
    /// \param colorData A reference to the color data vector to fill.
    static void loadOBJ(const std::string &path,
                        std::vector<float> &vertexData,
                        std::vector<float> &normalData,
                        std::vector<float> &uvData,
                        std::vector<float> &colorData);
    /// Load an image from file and store the data as unsigned char *. Free the image data again with freeImage().
    /// \param path The path to the image file to load.
    /// \param imageData A pointer to a pointer to an unsigned int * that will be filled with the image data.
    /// \param imageWidth A pointer to an int that will be filled with the width of the image.
    /// \param imageHeight A pointer to an int that will be filled with the height of the image.
    /// \param numberOfChannels A pointer to an int that will be filled with the actual numbers of channels in the image.
    /// \param desiredNumberOfChannels An optional desired number of channels to load into the data.
    static void loadImage(const std::string &path,
                          unsigned char **imageData,
                          int *imageWidth = nullptr, int *imageHeight = nullptr,
                          int *numberOfChannels = nullptr, int desiredNumberOfChannels = 0);
    /// Free image data loaded with loadImage().
    static void freeImage(unsigned char *imageData);
};

#endif //BESTEST_GAME_FILELOADER_H
