//
// Created by Milan van Zanten on 25.04.18.
//

#include "CubeMapTexture.h"
#include "../../io/FileLoader.h"
#include "Texture.h"
#include "../../exception/Exception.h"

CubeMapTexture::CubeMapTexture(int size, bool convertToLinearSpace, int desiredChannelCount)
        : imageSize(size), channelCount(desiredChannelCount) {
    textureData.resize(6);

    init(desiredChannelCount, convertToLinearSpace);
}

CubeMapTexture::CubeMapTexture(const std::vector<std::string> &texturePaths, bool convertToLinearSpace, int desiredChannelCount) {
    textureData.resize(6);

    if(texturePaths.size() != 6)
        throw Exception("Must specify 6 side textures for cube map creation.");

    int imageWidth, imageHeight;
    for(GLuint i = 0; i < texturePaths.size(); i++) {
        FileLoader::loadImage(texturePaths[i], &textureData[i], &imageWidth, &imageHeight, &channelCount, desiredChannelCount, false);

        if(imageWidth != imageHeight)
            throw Exception("Cube map texture must be of equal width and height.");
        if(imageSize != 0 && imageWidth != imageSize)
            throw Exception("All cube map side textures must be of equal size.");

        imageSize = imageWidth;
    }

    freeTextureData = true;

    init(desiredChannelCount, convertToLinearSpace);
}

void CubeMapTexture::init(int desiredChannelCount, bool convertToLinearSpace) {
    if(convertToLinearSpace)
        internalFormat = GL_SRGB;
    else
        internalFormat = GL_RGB;

    switch(desiredChannelCount) {
        case 1:
            format = GL_RED;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        default:
            break;
    }

    glGenTextures(1, &textureID);
}

void CubeMapTexture::bindTexture(int unit) {
    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + unit));
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

void CubeMapTexture::fillTexture(bool filter, bool mipmap) {
    bindTexture(0);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    if(filter) {
        if(mipmap) {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        } else {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
    } else {
        if(mipmap) {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        } else {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
    }

    for(GLuint i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, imageSize, imageSize, 0, format, type, textureData[i]);
    }
    if(freeTextureData)
        for(unsigned char *data: textureData)
            FileLoader::freeImage(data);

    if(mipmap)
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

int CubeMapTexture::getImageSize() const {
    return imageSize;
}

int CubeMapTexture::getChannelCount() const {
    return channelCount;
}

CubeMapTexture::~CubeMapTexture() {
    glDeleteTextures(1, &textureID);
}
