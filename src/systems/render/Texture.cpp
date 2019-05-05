#include "Texture.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>

#include <glbinding/gl/gl.h>

using namespace gl;

void E4::Texture::load() {
    if (this->loaded) return;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // 加载并生成纹理
    int nrChannels;
    stbi_uc* data = stbi_load(this->name.c_str(), &this->w, &this->h, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, this->w, this->h, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        this->loaded = true;
    } else {
        std::cout << "Failed to load texture" << std::endl;
        this->loaded = false;
    }
    stbi_image_free(data);
}

void E4::Texture::unload() {
    glDeleteTextures(1, &this->textureId);
}
