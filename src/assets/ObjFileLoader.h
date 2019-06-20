#pragma once

#include <vector>
#include <string>
#include "../components/Drawable.h"

namespace E4 {

    struct Mtl {
        std::string name;

        float Ka[3];
        float Kd[3];
        float Ks[3];
        float Ns;
        float d;
        float Ni;
        uint8_t illum;
    };
    struct MtlFile {
        std::vector<Mtl> mtls;
    };

    struct Obj {
        std::string name;
        std::string mtlName;
        bool smooth;

        std::vector<float> position;
        std::vector<float> texcoord;
        std::vector<float> normal;
    };
    struct ObjFile {
        std::vector<float> v;
        std::vector<float> vt;
        std::vector<float> vn;

        std::string mtlFileName;
        std::vector<Obj> objs;
    };


    class ObjFileLoader {
    public:
        static void loadObjFile(const std::string& folder, const std::string& filename, ObjFile& objFile);
        static void loadMtlFile(const std::string& folder, const std::string& filename, MtlFile& mtlFile);
    };

}
