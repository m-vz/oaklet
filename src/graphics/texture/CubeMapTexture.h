//
// Created by Milan van Zanten on 25.04.18.
//

#ifndef BESTEST_GAME_CUBEMAPTEXTURE_H
#define BESTEST_GAME_CUBEMAPTEXTURE_H

#include <GL/glew.h>
#include <string>
#include <vector>

class CubeMapTexture {
public:
    std::vector<unsigned char*> textureData;
    GLuint textureID;

    explicit CubeMapTexture(int size, bool convertToLinearSpace, int desiredChannelCount = 3);
    explicit CubeMapTexture(const std::vector<std::string> &texturePaths, bool convertToLinearSpace, int desiredChannelCount = 3);
    void bindTexture(int unit);
    void fillTexture(bool filter = true, bool mipmap = false, bool filterBetweenMipmaps = true, GLint clamp = GL_CLAMP_TO_EDGE);
    int getImageSize() const;
    int getChannelCount() const;
    virtual ~CubeMapTexture();

protected:
    int imageSize = 0, channelCount;
    GLint internalFormat;
    GLenum format = GL_RGB;
    GLenum type = GL_UNSIGNED_BYTE;

private:
    bool freeTextureData = false;

    void init(int desiredChannelCount, bool convertToLinearSpace);
};

#endif //BESTEST_GAME_CUBEMAPTEXTURE_H
