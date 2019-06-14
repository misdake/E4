#include "ScriptFile.h"

#include "../../util/File.h"

E4::ScriptFile& E4::ScriptFile::load(std::string nfolder, std::string nname) {
    this->folder = std::move(nfolder);
    this->name = std::move(nname);
    this->scriptIndex = 0;
    this->scriptLoaded = false;

    return *this;
}

void E4::ScriptFile::unload() {
    //TODO
}
