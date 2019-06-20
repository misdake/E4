#pragma once

#include <vector>
#include "../../components/Env.h"

namespace E4 {

    struct Environment {
        std::vector<Light*> lights;
    };

}
