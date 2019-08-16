#include <sstream>
#include "TileFile.h"

E4::TileFile::TileFile(const std::string& content) {
    std::stringstream stream(content);
    std::vector<std::string> lines;
    {
        std::string line;
        while (getline(stream, line, '\n')) {
            lines.push_back(line);
        }
    }

    texture = lines[0];

    std::string::size_type sz;     // alias of size_t
    xcount = std::stof(lines[1], &sz);
    ycount = std::stof(lines[1].substr(sz));

    for (std::vector<std::string>::size_type i = 2; i < lines.size(); i++) {
        auto line = lines[i];
        std::vector<std::string>::size_type f = line.find(' ');
        Tile tile;
        tile.name = line.substr(0, f);
        auto n = line.substr(f + 1);
        tile.xoffset = std::stof(n, &sz);
        tile.yoffset = std::stof(n.substr(sz));
        tiles.push_back(tile);
    }
}

void E4::TileFile::write(std::map<std::string, ShaderData>& map) {
    float xscale = 1.0f / xcount;
    float yscale = 1.0f / ycount;
    for (auto& tile : tiles) {
        map[tile.name] = ShaderData(tile.xoffset, tile.yoffset, xscale, yscale);
    }
}
