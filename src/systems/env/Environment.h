#pragma once

#include <vector>
#include "../../components/Env.h"
#include "../../components/Transform.h"

namespace E4 {

    struct Environment {
        std::pair<Transform*, Camera*> camera;
        std::vector<std::pair<Transform*, Light*>> lights; //TODO support Vec4 (x,y,z,1) or (nx,ny,nz,0)
    };

}