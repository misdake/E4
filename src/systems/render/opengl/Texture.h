#pragma once

#include <string>
#include <map>
#include "DataType.h"

namespace E4 {

    class Texture {
    public:
        std::string folder;
        std::string name;
        ShaderData shaderData;
        int w, h;
        bool loading;
        bool loaded;
        std::map<std::string, ShaderData> tiles;

        Texture& load(std::string folder, std::string name);
        bool setTile(const std::string& tile, ShaderData& tiling);
        void unload();
    };

}