#include "Shaders.h"

#include "../Renderer.h"

const char* ShaderBasic_VS = "void main() {\n"
                             "    gl_Position = vec4(aPosition.xyz + uOffset, 1.0);\n"
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

const char* ShaderTexture_VS = "void main() {\n"
                             "    gl_Position = vec4(aPosition.xyz + uOffset, 1.0);\n"
                             "    vTexcoord = aTexcoord;\n"
                             "}";

const char* ShaderTexture_PS = "out vec4 outputColor;\n"
                             "void main()\n"
                             "{\n"
                             "   outputColor = texture(uTexture, vTexcoord);\n"
                             "}";

E4::ShaderTexture::ShaderTexture(GlRenderer& renderer) : Shader(ShaderTexture_VS, ShaderTexture_PS) {
    addVertexAttribute(renderer.attributeSlots.POSITION);
    addVertexAttribute(renderer.attributeSlots.TEXCOORD);
    addVertexUniform(renderer.uniformSlots.OFFSET);
    addPixelUniform(renderer.uniformSlots.TEXTURE);
    addVarying("vTexcoord", E4::ShaderDataType::VEC2);
}
