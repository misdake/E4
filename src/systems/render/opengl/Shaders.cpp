#include "Shaders.h"

#include "GlRenderer.h"
#include "../../../components/Transform.h"
#include "../../../components/Drawable.h"

const char* ShaderBasic_VS = "void main() {\n"
                             "    gl_Position = uWorld * vec4(aPosition.xyz, 1.0);\n"
                             "}";

const char* ShaderBasic_PS = "out vec4 outputColor;\n"
                             "void main() {\n"
                             "   outputColor = uColor;\n"
                             "}";

E4::ShaderBasic::ShaderBasic(GlRenderer& renderer) : Shader(ShaderBasic_VS, ShaderBasic_PS) {
    addVertexAttribute(renderer.attributeSlots.position);
    addVertexAttribute(renderer.attributeSlots.color);
    addVertexUniform(renderer.uniformSlots.world);
    addPixelUniform(renderer.uniformSlots.color);
}

void E4::ShaderBasic::bind(GlRenderer& renderer, const Transform& transform, const E4::Drawable& drawable) {
    const Mesh& mesh = drawable.mesh.get();
    const Material& material = drawable.material.get();

    renderer.attributeSlots.position.bind(mesh.position);
    renderer.uniformSlots.world.bind(transform.worldTransform);
    renderer.uniformSlots.color.bind(material.color);
}

const char* ShaderTexture_VS = "void main() {\n"
                               "    gl_Position = uWorld * vec4(aPosition.xyz, 1.0);\n"
                               "    vTexcoord = aTexcoord;\n"
                               "}";

const char* ShaderTexture_PS = "out vec4 outputColor;\n"
                               "void main() {\n"
                               "   outputColor = texture(uTexture, vTexcoord);\n"
                               "}";

E4::ShaderTexture::ShaderTexture(GlRenderer& renderer) : Shader(ShaderTexture_VS, ShaderTexture_PS) {
    addVertexAttribute(renderer.attributeSlots.position);
    addVertexAttribute(renderer.attributeSlots.texcoord);
    addVertexUniform(renderer.uniformSlots.world);
    addPixelUniform(renderer.uniformSlots.texture);
    addVarying("vTexcoord", E4::ShaderDataType::VEC2);
}

void E4::ShaderTexture::bind(GlRenderer& renderer, const Transform& transform, const E4::Drawable& drawable) {
    const Mesh& mesh = drawable.mesh.get();
    const Material& material = drawable.material.get();

    renderer.attributeSlots.position.bind(mesh.position);
    renderer.attributeSlots.texcoord.bind(mesh.texcoord);
    renderer.uniformSlots.world.bind(transform.worldTransform);
    renderer.uniformSlots.texture.bind(material.texture->shaderData);
}
