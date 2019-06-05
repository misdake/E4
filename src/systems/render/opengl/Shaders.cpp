#include "Shaders.h"

#include "../Renderer.h"

const char* ShaderBasic_VS = "void main() {\n"
                             "    gl_Position = vec4(aPosition.xyz + uOffset, aPosition.w);\n"
                             "    vColor = aColor;\n"
                             "}";

const char* ShaderBasic_PS = "out vec4 outputColor;\n"
                             "void main()\n"
                             "{\n"
                             "   outputColor = vec4(vColor.rgb, 1.0f);\n"
                             "}";

E4::ShaderBasic::ShaderBasic(GlRenderer& renderer) : Shader(ShaderBasic_VS, ShaderBasic_PS) {
    addVertexAttribute(renderer.attributeSlots.POSITION);
    addVertexAttribute(renderer.attributeSlots.COLOR);
    addVertexUniform(renderer.uniformSlots.OFFSET);
    addVarying("vColor", E4::ShaderDataType::VEC4);
}
