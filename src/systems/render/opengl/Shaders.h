#pragma once

#include "Shader.h"

namespace E4 {

    class Renderer;

    class ShaderBasic : public Shader {
    public:
        explicit ShaderBasic(Renderer& renderer);
    };

}
