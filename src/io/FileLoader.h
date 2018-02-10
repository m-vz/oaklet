//
// Created by Milan van Zanten on 09.02.18.
//

#ifndef BESTEST_GAME_FILELOADER_H
#define BESTEST_GAME_FILELOADER_H

#include <vector>
#include <string>

class FileLoader {
public:
    /// Load text from a file.
    /// \param filename The relative path of the file to read.
    /// \param size A pointer to a variable to store the file size in or NULL if the file size doesn't matter.
    /// \return The text in the file as a byte vector.
    static std::vector<char> loadFile(std::string filename, size_t *size = nullptr);
    /// Load text from a file as a string.
    /// \param filename The relative path of the file to read.
    /// \param size A pointer to a variable to store the file size in or NULL if the file size doesn't matter.
    /// \return The text in the file as a string.
    static std::string loadFileAsString(std::string filename, size_t *size = nullptr);
};

#endif //BESTEST_GAME_FILELOADER_H
