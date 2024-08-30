#include "TextureDatabase.h"
#include <iostream>
#include <algorithm> 

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

std::list<TextureData> TextureDataBase::textureCache;

void TextureDataBase::Initialise() {
    TextureDataBase::textureCache.clear();
    TextureDataBase::retrieveOrLoadTexture("cube", "textures/cube.png");

    TextureDataBase::retrieveOrLoadTexture("mar1", "textures/mario1.png");
    TextureDataBase::retrieveOrLoadTexture("mar2", "textures/mario2.png");
    TextureDataBase::retrieveOrLoadTexture("mar3", "textures/mario3.png");
    TextureDataBase::retrieveOrLoadTexture("mar4", "textures/mario4.png");
    TextureDataBase::retrieveOrLoadTexture("mar5", "textures/mario5.png");
    TextureDataBase::retrieveOrLoadTexture("mar6", "textures/mario6.png");
    TextureDataBase::retrieveOrLoadTexture("mar7", "textures/mario7.png");
    TextureDataBase::retrieveOrLoadTexture("mar8", "textures/mario8.png");
}

GLuint TextureDataBase::loadTexture(const char* filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (data) {
        GLenum format;
        if (channels == 1) format = GL_RED;
        else if (channels == 3) format = GL_RGB;
        else if (channels == 4) format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        return textureID;
    } else {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;
    }
}

GLuint TextureDataBase::retrieveOrLoadTexture(const char* name, const char* path) {
    auto it = std::find_if(textureCache.begin(), textureCache.end(), [&](const TextureData& texData) {
        return (texData.path == path);
    });

    if (it != textureCache.end()) {
        return it->textureID;
    } else {
        GLuint textureID = loadTexture(path);
        if (textureID != 0) {
            textureCache.emplace_back(name, path, textureID);
        }
        return textureID;
    }
}

GLuint TextureDataBase::getTexture(const char* name) {
    auto it = std::find_if(textureCache.begin(), textureCache.end(), [&](const TextureData& texData) {
        return strcmp(texData.name, name) == 0;
    });

    if (it != textureCache.end()) {
        return it->textureID;
    } else {
        std::cerr << "Texture with name " << name << " not found." << std::endl;
        return 0;
    }
}