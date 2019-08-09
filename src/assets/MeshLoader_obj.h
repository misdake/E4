#pragma once

#include "../core/Asset.h"
#include "../components/Drawable.h"
#include "ObjFileLoader.h"
#include <map>
#include <string>

namespace E4 {

    class App;

    class MeshLoader_obj {
    private:
        std::map<std::string, ObjFile> objCache;
        std::map<std::string, MtlFile> mtlCache;
        const ObjFile& getObjFile(const std::string& folder, const std::string& name);
        const MtlFile& getMtlFile(const std::string& folder, const std::string& name);

    public:
        uint64_t create(const std::string& folder, const std::string& filename, App& app); //TODO add param to enable/disable cache
    };

}
