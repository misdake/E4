#pragma once

#include <vector>
#include "../../components/Env.h"
#include "../../components/Transform.h"

namespace E4 {

    struct Environment {
        std::pair<uint32_t, Camera*> camera;
        std::vector<std::pair<Transform*, Light*>> lights;
    };

}
