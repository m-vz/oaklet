//
// Created by Milan van Zanten on 25.04.18.
//

#include "CubeTexture.h"
#include "../../io/FileLoader.h"
#include "../../exception/Exception.h"

CubeTexture::CubeTexture(int size, bool convertToLinearSpace, int desiredChannelCount)
        : Texture(size, size, convertToLinearSpace, desiredChannelCount) {
    textureData.resize(6);
    imageWidth = size;
    imageHeight = size;

    init(desiredChannelCount, convertToLinearSpace);
}

CubeTexture::CubeTexture(const std::vector<std::string> &texturePaths, bool convertToLinearSpace, int desiredChannelCount)
        : Texture(0, 0, convertToLinearSpace, desiredChannelCount) {
    textureData.resize(6);

    if(texturePaths.size() != 6)
        throw Exception("Must specify 6 side textures for cube map creation.");

    int imageWidth, imageHeight;
    for(GLuint i = 0; i < 6; i++) {
        FileLoader::loadImage(texturePaths[i], &textureData[i], &imageWidth, &imageHeight, &channelCount, desiredChannelCount, false);

        if(imageWidth != imageHeight)
            throw Exception("Cube map texture must be of equal width and height.");
        if(this->imageWidth != 0 && imageWidth != this->imageWidth)
            throw Exception("All cube map side textures must be of equal size.");

        this->imageWidth = imageWidth;
    }
    this->imageHeight = this->imageWidth;

    freeTextureData = true;

    init(desiredChannelCount, convertToLinearSpace);
}

void CubeTexture::init(int desiredChannelCount, bool convertToLinearSpace) {
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

    channelCount = desiredChannelCount;
    target = GL_TEXTURE_CUBE_MAP;
}

void CubeTexture::fillTexture(bool filter, bool mipmap, bool filterBetweenMipmaps, GLint clamp) {
    bindTexture(0);

    glTexParameteri(target, GL_TEXTURE_WRAP_S, clamp);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, clamp);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, clamp);

    setFilters(filter, mipmap, filterBetweenMipmaps);

    for(GLuint i = 0; i < 6; i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, imageWidth, imageWidth, 0, format, type, textureData[i]);

    if(freeTextureData)
        for(unsigned char *data: textureData)
            FileLoader::freeImage(data);

    if(mipmap)
        glGenerateMipmap(target);
}
