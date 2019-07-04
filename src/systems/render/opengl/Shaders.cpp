#include "Shaders.h"

#include "GlRenderer.h"
#include "../../../components/Transform.h"
#include "../../../components/Drawable.h"

const char* ShaderBasic_VS = "void main() {\n"
                             "    gl_Position = uWVP * vec4(aPosition.xyz, 1.0);\n"
                             "}";

const char* ShaderBasic_PS = "out vec4 outputColor;\n"
                             "void main() {\n"
                             "   outputColor = uAmbient;\n"
                             "}";

E4::ShaderBasic::ShaderBasic(GlRenderer& renderer) : Shader(ShaderBasic_VS, ShaderBasic_PS) {
    addVertexAttribute(renderer.attributeSlots.position);
    addVertexAttribute(renderer.attributeSlots.color);
    addVertexUniform(renderer.uniformSlots.world);
    addVertexUniform(renderer.uniformSlots.wvp);
    addPixelUniform(renderer.uniformSlots.ambient);
}

void E4::ShaderBasic::bind(GlRenderer& renderer, const Transform& transform, const Drawable& drawable, const E4::Environment& environment) {
    const Mesh& mesh = drawable.mesh.get();
    const Material& material = drawable.material.get();

    renderer.attributeSlots.position.bind(mesh.position);
    renderer.uniformSlots.world.bind(transform.world);
    renderer.uniformSlots.wvp.bind(transform.wvp);
    renderer.uniformSlots.ambient.bind(material.ambient);
}

const char* ShaderTexture_VS = "void main() {\n"
                               "    gl_Position = uWVP * vec4(aPosition.xyz, 1.0);\n"
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
    addVertexUniform(renderer.uniformSlots.wvp);
    addPixelUniform(renderer.uniformSlots.texture);
    addVarying("vTexcoord", E4::ShaderDataType::VEC2);
}

void E4::ShaderTexture::bind(GlRenderer& renderer, const Transform& transform, const Drawable& drawable, const E4::Environment& environment) {
    const Mesh& mesh = drawable.mesh.get();
    const Material& material = drawable.material.get();

    renderer.attributeSlots.position.bind(mesh.position);
    renderer.attributeSlots.texcoord.bind(mesh.texcoord);
    renderer.uniformSlots.world.bind(transform.world);
    renderer.uniformSlots.wvp.bind(transform.wvp);
    renderer.uniformSlots.texture.bind(material.texture->shaderData);
}

const char* ShaderLight_VS = "void main() {\n"
                             "    gl_Position = uWVP * vec4(aPosition.xyz, 1.0);\n"
                             "    vNormal = normalize((uWorld * vec4(aNormal, 0.0)).xyz);\n"
                             "    vWorldPos = uWorld * vec4(aPosition.xyz, 1.0);"
                             "}";

const char* ShaderLight_PS = "out vec4 outputColor;\n"
                             "void main() {\n"
                             "   vec3 normal = normalize(vNormal);\n"
                             "   vec3 worldPos = vWorldPos.xyz / vWorldPos.w;\n"
                             "   vec3 diff = normalize(uLight.xyz - worldPos);\n"
                             "   vec3 lightDir = uLight.w * diff + (1.0 - uLight.w) * uLight.xyz;\n"
                             "   vec3 reflectDir = normalize(reflect(-lightDir, normal));\n"
                             "   vec3 viewDir = normalize(worldPos - uCameraPos);\n"
                             "   vec3 diffuse = max(0.0, dot(normal, lightDir)) * uDiffuse.rgb * uLightDiffuse.rgb;\n"
                             "   vec3 specular = pow(max(0.0, dot(viewDir, reflectDir)), uSpecularExp) * uSpecular.rgb * uLightSpecular.rgb;\n"
                             "   outputColor = vec4(diffuse + specular + uAmbient.rgb * uLightAmbient.rgb, 1);\n"
                             "}";

E4::ShaderLight::ShaderLight(GlRenderer& renderer) : Shader(ShaderLight_VS, ShaderLight_PS) {
    addVertexAttribute(renderer.attributeSlots.position);
    addVertexAttribute(renderer.attributeSlots.normal);
    addVertexUniform(renderer.uniformSlots.world);
    addVertexUniform(renderer.uniformSlots.wvp);
    addPixelUniform(renderer.uniformSlots.cameraPos);
    addPixelUniform(renderer.uniformSlots.ambient);
    addPixelUniform(renderer.uniformSlots.diffuse);
    addPixelUniform(renderer.uniformSlots.specular);
    addPixelUniform(renderer.uniformSlots.specularExp);
    addPixelUniform(renderer.uniformSlots.light);
    addPixelUniform(renderer.uniformSlots.lightAmbient);
    addPixelUniform(renderer.uniformSlots.lightDiffuse);
    addPixelUniform(renderer.uniformSlots.lightSpecular);
    addVarying("vNormal", E4::ShaderDataType::VEC3);
    addVarying("vWorldPos", E4::ShaderDataType::VEC4);
}

void E4::ShaderLight::bind(GlRenderer& renderer, const Transform& transform, const Drawable& drawable, const E4::Environment& environment) {
    const Mesh& mesh = drawable.mesh.get();
    const Material& material = drawable.material.get();

    const std::pair<Transform*, Light*>& pair = environment.lights[0];
    Light& light = *pair.second;
    Camera& camera = *environment.camera.second;

    renderer.attributeSlots.position.bind(mesh.position);
    renderer.attributeSlots.normal.bind(mesh.normal);
    renderer.uniformSlots.world.bind(transform.world);
    renderer.uniformSlots.wvp.bind(transform.wvp);
    renderer.uniformSlots.cameraPos.bind(camera.pos);
    renderer.uniformSlots.ambient.bind(material.ambient);
    renderer.uniformSlots.diffuse.bind(material.diffuse);
    renderer.uniformSlots.specular.bind(material.specular);
    renderer.uniformSlots.specularExp.bind(material.specularExp);
    renderer.uniformSlots.light.bind(light.world);
    renderer.uniformSlots.lightAmbient.bind(light.ambient);
    renderer.uniformSlots.lightDiffuse.bind(light.diffuse);
    renderer.uniformSlots.lightSpecular.bind(light.specular);
}
