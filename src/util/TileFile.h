#pragma once

#include <string>
#include <vector>
#include <map>
#include "../systems/render/opengl/DataType.h"

namespace E4 {

    struct Tile {
        std::string name;
        float xoffset;
        float yoffset;
    };

    struct TileFile {
        std::string texture;
        float xcount;
        float ycount;
        std::vector<Tile> tiles;

        explicit TileFile(const std::string& content);

        void write(std::map<std::string, ShaderData>& map);
    };

}
