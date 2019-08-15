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
        ShaderData ambient;
        ShaderData diffuse;
        ShaderData specular;
        ShaderData specularExp;
        ShaderData tiling;

        void unload();
    };

    struct Mesh {
        FloatBuffer position;
        FloatBuffer texcoord;
        FloatBuffer normal;

        int vertexCount;
        ShortBuffer index;

        void unload();
    };

    struct Drawable {
    public:
        bool visible;
        Asset<Mesh> mesh;
        Asset<Material> material;
    };

}