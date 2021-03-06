#include "File.h"

#include "Log.h"
#include <fstream>
#include <sstream>

std::string E4::readFile(const std::string& folder, const std::string& fileName) {
    return std::move(readFile(folder + "/" + fileName));
}
std::string E4::readFile(const std::string& fileName) {
    Log::debug("file read %s", fileName.c_str());
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

void E4::writeFile(const std::string& folder, const std::string& fileName, const std::string& content) {
    writeFile(folder + "/" + fileName, content);
}
void E4::writeFile(const std::string& fileName, const std::string& content) {
    Log::debug("file write %s", fileName.c_str());
    std::ofstream output(fileName);
    output << content;
    output.close();
}