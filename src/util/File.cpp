#include "File.h"

#include <fstream>
#include <sstream>

std::string E4::readFile(const std::string& fileName) {
    std::string content;
    std::ifstream input(fileName, std::ios::in);
    if (input.is_open()) {
        std::stringstream sstr;
        sstr << input.rdbuf();
        content = sstr.str();
        input.close();
    }
    return content;
}