#include "ScriptFile.h"

#include "../../util/File.h"
#include "ScriptFunctions.h"

E4::ScriptFile& E4::ScriptFile::load(std::string nfolder, std::string nname) {
    this->folder = std::move(nfolder);
    this->name = std::move(nname);
    this->content = readFile(this->folder + "/" + this->name);
    this->functions = nullptr;

    return *this;
}

void E4::ScriptFile::unload() {
    delete this->functions;
}
