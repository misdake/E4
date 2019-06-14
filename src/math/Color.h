#pragma once

#include <string>

namespace E4 {

    struct Color {
        float r, g, b, a;

        void set(const std::string& argb);
    };

}
