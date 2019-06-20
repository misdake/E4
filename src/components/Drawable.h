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
        FloatBuffer normal;

        int vertexCount;
        ShortBuffer index;

        void print() {
            if (index.countIndices > 0) {
                Log::debug("indexed %d", index.countIndices);
            } else {
                Log::debug("vertices %d", vertexCount);
            }

            if (position.countVertex > 0) {
                Log::debug("position: id %d, countVertex %d, floatPerVertex %d, len %d", position.bufferId, position.countVertex, position.floatPerVertex, position.array.size());
            }

            if (texcoord.countVertex > 0) {
                Log::debug("texcoord: id %d, countVertex %d, floatPerVertex %d, len %d", texcoord.bufferId, texcoord.countVertex, texcoord.floatPerVertex, texcoord.array.size());
            }
        }
    };

    struct Drawable {
    public:
        Asset<Mesh> mesh;
        Asset<Material> material;
    };

}