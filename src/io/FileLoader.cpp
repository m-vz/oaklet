//
// Created by Milan van Zanten on 09.02.18.
//

#include <string>
#include <ios>
#include <fstream>
#include "FileLoader.h"
#include "../exception/Exception.h"

std::vector<char> FileLoader::loadFile(const std::string filename, size_t *size) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if(!file.is_open())
        throw IOException("Could not open file: " + filename);

    size_t fileSize = static_cast<size_t>(file.tellg());
    if(size != NULL)
        *size = fileSize;

    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    buffer.push_back('\0'); // add a null terminator
    file.close();

    return buffer;
}

std::string FileLoader::loadFileAsString(const std::string filename, size_t *size) {
    std::vector<char> data = loadFile(filename, size);
    return std::string(data.begin(), data.end());
}