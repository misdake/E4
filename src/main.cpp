#include <iostream>
#include <SDL.h>
#include <sstream>

//#include <CNtity/CNtity.hpp>

//#include <sol/sol.hpp>

#include "systems/render/Texture.h"
#include "systems/render/Renderer.h"
#include "systems/render/opengl/Shader.h"
#include "systems/render/opengl/GlRenderer.h"
#include "util/Window.h"
#include "util/File.h"
#include "core/App.h"

#include <glbinding/gl/gl.h>

using namespace gl;


//struct Position {
//    float x;
//    float y;
//};
//struct Velocity {
//    float x;
//    float y;
//};
//struct Health {
//    int max;
//    int current;
//};

//void gldPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
//    // This code is based off the MESA source for gluPerspective
//    // *NOTE* This assumes GL_PROJECTION is the current matrix
//
//
//    GLdouble xmin, xmax, ymin, ymax;
//    GLdouble m[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//
//    ymax = zNear * tan(fovy * M_PI / 360.0);
//    ymin = -ymax;
//
//    xmin = ymin * aspect;
//    xmax = ymax * aspect;
//
//    // Set up the projection matrix
//    m[0 + 0 * 4] = (2.0 * zNear) / (xmax - xmin);
//    m[1 + 1 * 4] = (2.0 * zNear) / (ymax - ymin);
//    m[2 + 2 * 4] = -(zFar + zNear) / (zFar - zNear);
//
//    m[0 + 2 * 4] = (xmax + xmin) / (xmax - xmin);
//    m[1 + 2 * 4] = (ymax + ymin) / (ymax - ymin);
//    m[3 + 2 * 4] = -1.0;
//
//    m[2 + 3 * 4] = -(2.0 * zFar * zNear) / (zFar - zNear);
//
//    // Add to current matrix
//    glMultMatrixd(m);
//}

int main(int argc, char* argv[]) {
    E4::App app;

    std::vector<float> position = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };
    std::vector<float> color = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
    };
    E4::FloatBuffer positionBuffer(position, 3, 3);
    E4::FloatBuffer colorBuffer(color, 3, 3);
    E4::ShaderData offsetData(0.2, 0, 0);

    E4::Program* program;

    E4::Mesh mesh;
    E4::Material material;
    E4::Drawable drawable;

    app.load([&](E4::App& app) {
        positionBuffer.upload();
        colorBuffer.upload();

        mesh.addAttribute(app.renderer.attributeSlots.POSITION, positionBuffer);
        mesh.addAttribute(app.renderer.attributeSlots.COLOR, colorBuffer);
        mesh.addUniform(app.renderer.uniformSlots.OFFSET, offsetData);
        mesh.vertexCount = 3;

        std::string vsContent = E4::readFile("shader_basic_vs.txt");
        std::string psContent = E4::readFile("shader_basic_ps.txt");
        program = new E4::Program(vsContent, psContent);
        program->vertexShader.addAttribute(app.renderer.attributeSlots.POSITION);
        program->vertexShader.addAttribute(app.renderer.attributeSlots.COLOR);
        program->vertexShader.addUniform(app.renderer.uniformSlots.OFFSET);
        program->compile();

        material.program = program;

        drawable.mesh = &mesh;
        drawable.material = &material;
    });

    app.enterLoop([&](E4::App& app, const E4::FrameState& frameState) {
        app.glRenderer.clear();
        app.glRenderer.draw(drawable);
    });

    return 0;
}