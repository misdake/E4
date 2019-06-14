#pragma once

#include "../core/Asset.h"
#include "../systems/render/opengl/Texture.h"
#include "../systems/render/opengl/FloatBuffer.h"
#include "../systems/render/opengl/ShortBuffer.h"

namespace E4 {

    class Shader;

    struct Material {
        Shader* shader;
        Asset<Texture> texture;
        ShaderData color;
    };

    struct Mesh {
        FloatBuffer position;
        FloatBuffer texcoord;

        int vertexCount;
        ShortBuffer index;
    };

    struct Drawable {
    public:
        Asset<Mesh> mesh;
        Asset<Material> material;

        Drawable() :
            mesh(),
            material() {
        }
    };

}