#pragma once

#include <string>
#include <vector>

#include "DataType.h"
#include "../../env/Environment.h"

namespace E4 {
    class AttributeSlot;
    class UniformSlot;
    class GlRenderer;
    class Transform;
    class Drawable;

    typedef std::vector<std::reference_wrapper<AttributeSlot>> AttributeSlotList;
    typedef std::vector<std::reference_wrapper<UniformSlot>> UniformSlotList;
    typedef std::vector<std::pair<std::string, ShaderDataType>> VaryingSlotList;

    class Shader {
    public:
        std::vector<std::pair<AttributeSlot*, uint32_t>> vertexAttributes;
        std::vector<std::pair<UniformSlot*, uint32_t>> vertexUniforms;
        std::vector<std::pair<std::string, ShaderDataType>> varyings;
        std::vector<std::pair<UniformSlot*, uint32_t>> pixelUniforms;

        Shader(
            std::string vsMain, std::string psMain,
            const AttributeSlotList& attributeSlots,
            const UniformSlotList& vertexUniformSlots,
            const VaryingSlotList& varyingSlots,
            const UniformSlotList& pixelUniformSlots
        );
        void use();
        virtual void bind(GlRenderer& renderer, const Transform& transform, const Drawable& drawable, const E4::Environment& environment) = 0;

    private:
        void compile();

        bool compiled = false;
        uint32_t programId = 0;
        std::string vsMain = "";
        std::string psMain = "";
        std::string vsContent = "";
        std::string psContent = "";
    };
}