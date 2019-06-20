#include "ObjFileLoader.h"

#include "../util/File.h"
#include <sstream>

void E4::ObjFileLoader::loadObjFile(const std::string& folder, const std::string& filename, E4::ObjFile& objFile) {
    std::string fileContent = readFile(folder, filename);

    std::istringstream fileInput(fileContent);

    std::string line;

    Obj* curr = nullptr;
    while (std::getline(fileInput, line)) {
        std::istringstream lineInput(line);
        std::string w;
        if (!(lineInput >> w)) continue;
        switch (w[0]) {
            case 'm': {
                if (w == "mtllib") {
                    lineInput >> objFile.mtlFileName;
                }
                break;
            }
            case 'o': {
                //TODO write name to curr.
                break;
            }
            case 'u': {
                if (w == "usemtl") {
                    Obj& obj = objFile.objs.emplace_back();
                    obj.smooth = true;
                    curr = &obj;
                    lineInput >> curr->mtlName;
                }
                break;
            }
            case 'v': {
                float a, b, c;
                if (w == "v") {
                    lineInput >> a >> b >> c;
                    objFile.v.push_back(a);
                    objFile.v.push_back(b);
                    objFile.v.push_back(c);
                } else if (w == "vt") {
                    lineInput >> a >> b;
                    objFile.vt.push_back(a);
                    objFile.vt.push_back(b);
                } else if (w == "vn") {
                    lineInput >> a >> b >> c;
                    objFile.vn.push_back(a);
                    objFile.vn.push_back(b);
                    objFile.vn.push_back(c);
                }
                break;
            }
            case 'f': {
                std::string part;
                int count = 0;
                std::vector<int> position;
                std::vector<int> texcoord;
                std::vector<int> normal;

                while (lineInput >> part) {
                    if (part.empty()) continue;
                    count++;
                    uint64_t l = part.find('/');
                    uint64_t r = part.rfind('/');
                    if (l == std::string::npos && r == l) { // position only
                        position.push_back(std::stoi(part));
                    } else if (l == r) { // position + texcoord
                        position.push_back(std::stoi(part.substr(0, l)));
                        texcoord.push_back(std::stoi(part.substr(l + 1)));
                    } else if (l == r - 1) { // position + normal
                        position.push_back(std::stoi(part.substr(0, l)));
                        normal.push_back(std::stoi(part.substr(r + 1)));
                    } else if (l < r - 1) { // position + texcoord + normal
                        position.push_back(std::stoi(part.substr(0, l)));
                        texcoord.push_back(std::stoi(part.substr(l + 1, r)));
                        normal.push_back(std::stoi(part.substr(r + 1)));
                    }
                }

                if (!position.empty()) {
                    for (int i = 2; i < count; i++) {
                        int a = 0, b = i - 1, c = i;
                        a = position[a];
                        b = position[b];
                        c = position[c];
                        if (a < 0) {} //TODO
                        if (b < 0) {} //TODO
                        if (c < 0) {} //TODO
                        a--;
                        b--;
                        c--;
                        curr->position.push_back(objFile.v[a * 3 + 0]);
                        curr->position.push_back(objFile.v[a * 3 + 1]);
                        curr->position.push_back(objFile.v[a * 3 + 2]);
                        curr->position.push_back(objFile.v[b * 3 + 0]);
                        curr->position.push_back(objFile.v[b * 3 + 1]);
                        curr->position.push_back(objFile.v[b * 3 + 2]);
                        curr->position.push_back(objFile.v[c * 3 + 0]);
                        curr->position.push_back(objFile.v[c * 3 + 1]);
                        curr->position.push_back(objFile.v[c * 3 + 2]);
                    }
                }
                if (!texcoord.empty()) {
                    for (int i = 2; i < count; i++) {
                        int a = 0, b = i - 1, c = i;
                        a = texcoord[a];
                        b = texcoord[b];
                        c = texcoord[c];
                        if (a < 0) {} //TODO
                        if (b < 0) {} //TODO
                        if (c < 0) {} //TODO
                        a--;
                        b--;
                        c--;
                        curr->texcoord.push_back(objFile.vt[a * 2 + 0]);
                        curr->texcoord.push_back(objFile.vt[a * 2 + 1]);
                        curr->texcoord.push_back(objFile.vt[b * 2 + 0]);
                        curr->texcoord.push_back(objFile.vt[b * 2 + 1]);
                        curr->texcoord.push_back(objFile.vt[c * 2 + 0]);
                        curr->texcoord.push_back(objFile.vt[c * 2 + 1]);
                    }
                }
                if (!normal.empty()) {
                    for (int i = 2; i < count; i++) {
                        int a = 0, b = i - 1, c = i;
                        a = normal[a];
                        b = normal[b];
                        c = normal[c];
                        if (a < 0) {} //TODO
                        if (b < 0) {} //TODO
                        if (c < 0) {} //TODO
                        a--;
                        b--;
                        c--;
                        curr->normal.push_back(objFile.vn[a * 3 + 0]);
                        curr->normal.push_back(objFile.vn[a * 3 + 1]);
                        curr->normal.push_back(objFile.vn[a * 3 + 2]);
                        curr->normal.push_back(objFile.vn[b * 3 + 0]);
                        curr->normal.push_back(objFile.vn[b * 3 + 1]);
                        curr->normal.push_back(objFile.vn[b * 3 + 2]);
                        curr->normal.push_back(objFile.vn[c * 3 + 0]);
                        curr->normal.push_back(objFile.vn[c * 3 + 1]);
                        curr->normal.push_back(objFile.vn[c * 3 + 2]);
                    }
                }

                break;
            }
            case 's': {
                std::string v;
                lineInput >> v;
                if (v == "1") {
                    curr->smooth = true;
                } else if (v == "off") {
                    curr->smooth = false;
                }
                break;
            }
            default:
                break;
        }
    }

}

void E4::ObjFileLoader::loadMtlFile(const std::string& folder, const std::string& filename, E4::MtlFile& mtlFile) {
    std::string fileContent = readFile(folder, filename);

    std::istringstream fileInput(fileContent);

    std::string line;

    Mtl* curr = nullptr;
    while (std::getline(fileInput, line)) {
        std::istringstream lineInput(line);
        std::string w;
        if (!(lineInput >> w)) continue;
        switch (w[0]) {
            case 'n': {
                if (w == "newmtl") {
                    Mtl& mtl = mtlFile.mtls.emplace_back();
                    lineInput >> mtl.name;
                    curr = &mtl;
                    break;
                }
            }
            case 'N': {
                if (w == "Ns") {
                    lineInput >> curr->Ns;
                } else if (w == "Ni") {
                    lineInput >> curr->Ni;
                }
                break;
            }
            case 'K': {
                if (w == "Ka") {
                    lineInput >> curr->Ka[0] >> curr->Ka[1] >> curr->Ka[2];
                } else if (w == "Kd") {
                    lineInput >> curr->Kd[0] >> curr->Kd[1] >> curr->Kd[2];
                } else if (w == "Ks") {
                    lineInput >> curr->Ks[0] >> curr->Ks[1] >> curr->Ks[2];
                }
                break;
            }
            case 'd': {
                if (w == "d") {
                    lineInput >> curr->d;
                }
                break;
            }
            case 'i': {
                if (w == "illum") {
                    lineInput >> curr->illum;
                }
            }
        }
    }
}

