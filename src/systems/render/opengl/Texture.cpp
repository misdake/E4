#include "Texture.h"
#include "../../../util/TileFile.h"
#include "../../../util/File.h"

#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>
#include <glbinding/gl/gl.h>

using namespace gl;

E4::Texture& E4::Texture::load(std::string nfolder, std::string nname) {
    if (this->loading) return *this;
    this->loading = true;
    this->folder = std::move(nfolder);
    this->name = std::move(nname);
    Log::debug("texture: load %s", name.c_str());
    std::string filepath = this->folder + "/" + this->name;

    bool mipmap = true;
    if (name.rfind(".txt") != std::string::npos) {
        TileFile tileFile(readFile(folder, name));
        tileFile.write(tiles);
        filepath = this->folder + "/" + tileFile.texture;
        mipmap = false;
    }

    stbi_set_flip_vertically_on_load(true);
    uint32_t& textureId = shaderData.textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    int nrChannels;
    stbi_uc* data = stbi_load(filepath.c_str(), &this->w, &this->h, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, this->w, this->h, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        if (mipmap) glGenerateMipmap(GL_TEXTURE_2D);
        this->loaded = true;
    } else {
        Log::error("Failed to load texture %s", filepath.c_str());
        this->loaded = false;
    }
    stbi_image_free(data);

    return *this;
}

bool E4::Texture::setTile(const std::string& tile, ShaderData& tiling) {
    auto iterator = tiles.find(tile);
    if (iterator != tiles.end()) {
        ShaderData& data = iterator->second;
        tiling = data;
        return true;
    } else {
        return false;
    }
}

void E4::Texture::unload() {
    if (this->loading) {
        Log::debug("texture: unload %s", name.c_str());
        glDeleteTextures(1, &shaderData.textureId);
        folder = "";
        name = "";
        loading = false;
        loaded = false;
    }
}
