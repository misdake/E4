#pragma once

#include <string>

namespace E4 {

    std::string readFile(const std::string& folder, const std::string& fileName);
    std::string readFile(const std::string& fileName);

    void writeFile(const std::string& folder, const std::string& fileName, const std::string& content);
    void writeFile(const std::string& fileName, const std::string& content);

}