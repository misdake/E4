#include "Color.h"

#include <regex>

void E4::Color::set(const std::string& argb) {
    std::regex pattern("#?([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");

    std::smatch match;
    if (std::regex_match(argb, match, pattern)) {
        auto na = std::stoul(match[1].str(), nullptr, 16);
        auto nr = std::stoul(match[2].str(), nullptr, 16);
        auto ng = std::stoul(match[3].str(), nullptr, 16);
        auto nb = std::stoul(match[4].str(), nullptr, 16);
        float ratio = 1.0f / 255.0f;
        a = na * ratio;
        r = nr * ratio;
        g = ng * ratio;
        b = nb * ratio;
    }
}
