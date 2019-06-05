#pragma once

#include <string>
#include "../systems/render/opengl/AttributeSlot.h"
#include "../systems/render/opengl/UniformSlot.h"
#include "../core/Asset.h"
#include "../systems/render/opengl/Texture.h"
#include "../systems/render/opengl/ShortBuffer.h"

namespace E4 {

    class Shader;

    class Material {
    public:
        Shader* shader;
        Texture texture;
    };

    class Mesh {
    public:
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