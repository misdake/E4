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

E4::ShaderBasic::ShaderBasic(GlRenderer& renderer) : Shader(
    ShaderBasic_VS, ShaderBasic_PS,
    {
        renderer.attributeSlots.position,
        renderer.attributeSlots.color,
    },
    {
        renderer.uniformSlots.world,
        renderer.uniformSlots.wvp,
    },
    {},
    {
        renderer.uniformSlots.ambient,
    }
) {
}

void E4::ShaderBasic::bind(GlRenderer& renderer, const Transform& transform, const Drawable& drawable, const E4::Environment& environment) {
    const Mesh& mesh = drawable.mesh.get();
    const Material& material = drawable.material.get();

    renderer.attributeSlots.position << mesh.position;
    renderer.uniformSlots.world << transform.world;
    renderer.uniformSlots.wvp << transform.wvp;
    renderer.uniformSlots.ambient << material.ambient;
}

const char* ShaderTexture_VS = "void main() {\n"
                               "    gl_Position = uWVP * vec4(aPosition.xyz, 1.0);\n"
                               "    vTexcoord = aTexcoord;\n"
                               "}";

const char* ShaderTexture_PS = "out vec4 outputColor;\n"
                               "void main() {\n"
                               "   outputColor = texture(uTexture, vTexcoord);\n"
                               "}";

E4::ShaderTexture::ShaderTexture(GlRenderer& renderer) : Shader(
    ShaderTexture_VS, ShaderTexture_PS,
    {
        renderer.attributeSlots.position,
        renderer.attributeSlots.texcoord,
    },
    {
        renderer.uniformSlots.world,
        renderer.uniformSlots.wvp,
    },
    {
        {"vTexcoord", E4::ShaderDataType::VEC2},
    },
    {
        renderer.uniformSlots.texture,
    }
) {
}

void E4::ShaderTexture::bind(GlRenderer& renderer, const Transform& transform, const Drawable& drawable, const E4::Environment& environment) {
    const Mesh& mesh = drawable.mesh.get();
    const Material& material = drawable.material.get();

    renderer.attributeSlots.position << mesh.position;
    renderer.attributeSlots.texcoord << mesh.texcoord;
    renderer.uniformSlots.world << transform.world;
    renderer.uniformSlots.wvp << transform.wvp;
    renderer.uniformSlots.texture << material.texture->shaderData;
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
                             "   vec3 viewDir = normalize(uCameraPos - worldPos);\n"
                             "   vec3 diffuse = max(0.0, dot(normal, lightDir)) * uDiffuse.rgb * uLightDiffuse.rgb;\n"
                             "   vec3 specular = pow(max(0.0, dot(viewDir, reflectDir)), uSpecularExp) * uSpecular.rgb * uLightSpecular.rgb;\n"
                             "   outputColor = vec4(diffuse + specular + uAmbient.rgb * uLightAmbient.rgb, 1);\n"
                             "}";

E4::ShaderLight::ShaderLight(GlRenderer& renderer) : Shader(
    ShaderLight_VS, ShaderLight_PS,
    {
        renderer.attributeSlots.position,
        renderer.attributeSlots.normal,
    },
    {
        renderer.uniformSlots.world,
        renderer.uniformSlots.wvp,
    },
    {
        {"vNormal",   E4::ShaderDataType::VEC3},
        {"vWorldPos", E4::ShaderDataType::VEC4},
    },
    {
        renderer.uniformSlots.cameraPos,
        renderer.uniformSlots.ambient,
        renderer.uniformSlots.diffuse,
        renderer.uniformSlots.specular,
        renderer.uniformSlots.specularExp,
        renderer.uniformSlots.light,
        renderer.uniformSlots.lightAmbient,
        renderer.uniformSlots.lightDiffuse,
        renderer.uniformSlots.lightSpecular,
    }
) {
}

void E4::ShaderLight::bind(GlRenderer& renderer, const Transform& transform, const Drawable& drawable, const E4::Environment& environment) {
    const Mesh& mesh = drawable.mesh.get();
    const Material& material = drawable.material.get();

    const std::pair<Transform*, Light*>& pair = environment.lights[0];
    Light& light = *pair.second;
    Camera& camera = *environment.camera.second;

    renderer.attributeSlots.position << mesh.position;
    renderer.attributeSlots.normal << mesh.normal;
    renderer.uniformSlots.world << transform.world;
    renderer.uniformSlots.wvp << transform.wvp;
    renderer.uniformSlots.cameraPos << camera.pos;
    renderer.uniformSlots.ambient << material.ambient;
    renderer.uniformSlots.diffuse << material.diffuse;
    renderer.uniformSlots.specular << material.specular;
    renderer.uniformSlots.specularExp << material.specularExp;
    renderer.uniformSlots.light << light.world;
    renderer.uniformSlots.lightAmbient << light.ambient;
    renderer.uniformSlots.lightDiffuse << light.diffuse;
    renderer.uniformSlots.lightSpecular << light.specular;
}
