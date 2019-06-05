#pragma once

#include "../core/Asset.h"
#include "../systems/render/opengl/Texture.h"
#include "../systems/render/opengl/FloatBuffer.h"
#include "../systems/render/opengl/ShortBuffer.h"

namespace E4 {

    class Shader;

    struct Material {
        Shader* shader;
        Texture texture;
    };

    struct Mesh {
        FloatBuffer position;
        FloatBuffer texcoord;
        FloatBuffer color;
        ShaderData offset;

        int vertexCount;
        ShortBuffer index;
    };

    struct Drawable {
    public:
        Asset<Mesh> mesh;
        Asset<Material> material;
    };

}