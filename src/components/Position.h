#pragma once

namespace E4 {

    struct Position {
        float x, y, z;

        uint64_t lastFrame;
        uint64_t parent;
        float worldX, worldY, worldZ;
    };

}