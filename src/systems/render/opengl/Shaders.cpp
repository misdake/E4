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

void E4::ShaderBasic::bind(GlRenderer& renderer, const Transform& transform, const Drawable& drawable, const E4::Environment& environment) {
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

void E4::ShaderTexture::bind(GlRenderer& renderer, const Transform& transform, const Drawable& drawable, const E4::Environment& environment) {
    const Mesh& mesh = drawable.mesh.get();
    const Material& material = drawable.material.get();

    renderer.attributeSlots.position.bind(mesh.position);
    renderer.attributeSlots.texcoord.bind(mesh.texcoord);
    renderer.uniformSlots.world.bind(transform.worldTransform);
    renderer.uniformSlots.texture.bind(material.texture->shaderData);
}

const char* ShaderLight_VS = "void main() {\n"
                             "    gl_Position = uWorld * vec4(aPosition.xyz, 1.0);\n"
                             "    vNormal = normalize((uWorld * vec4(aNormal, 0.0)).xyz);\n"
                             "}";

const char* ShaderLight_PS = "out vec4 outputColor;\n"
                             "void main() {\n"
                             "   vec3 normal = normalize(vNormal);\n"
                             "   float dot = dot(normal, uLight.xyz);\n" //TODO support point light
                             "   outputColor = vec4(dot * uColor.rgb * uLightColor.rgb, uColor.a);\n"
                             "}";

E4::ShaderLight::ShaderLight(GlRenderer& renderer) : Shader(ShaderLight_VS, ShaderLight_PS) {
    addVertexAttribute(renderer.attributeSlots.position);
    addVertexAttribute(renderer.attributeSlots.normal);
    addVertexUniform(renderer.uniformSlots.world);
    addPixelUniform(renderer.uniformSlots.color);
    addPixelUniform(renderer.uniformSlots.light);
    addPixelUniform(renderer.uniformSlots.lightColor);
    addVarying("vNormal", E4::ShaderDataType::VEC3);
}

void E4::ShaderLight::bind(GlRenderer& renderer, const Transform& transform, const Drawable& drawable, const E4::Environment& environment) {
    const Mesh& mesh = drawable.mesh.get();
    const Material& material = drawable.material.get();

    const std::pair<Vec3, Light*>& pair = environment.lights[0];
    Vec3 lightVal = pair.first;
    Light& light = *pair.second;

    renderer.attributeSlots.position.bind(mesh.position);
    renderer.attributeSlots.normal.bind(mesh.normal);
    renderer.uniformSlots.world.bind(transform.worldTransform);
    renderer.uniformSlots.color.bind(material.color);
    renderer.uniformSlots.light.bind(ShaderData(lightVal.x, lightVal.y, lightVal.z));
    renderer.uniformSlots.lightColor.bind(light.color);
}
