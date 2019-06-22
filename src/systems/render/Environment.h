#pragma once

#include <vector>
#include "../../components/Env.h"

namespace E4 {

    struct Environment {
        std::vector<std::pair<Vec3, Light*>> lights; //TODO support Vec4 (x,y,z,1) or (nx,ny,nz,0)
    };

}
