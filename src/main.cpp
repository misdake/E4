#include <iostream>
#include <SDL.h>
#include <sstream>
#include <iostream>

#include <CNtity/CNtity.hpp>
#include <iostream>

#include <sol/sol.hpp>

#include <glbinding/gl/gl.h>

#include "systems/render/Texture.h"
#include "util/Window.h"

using namespace gl;

struct Position {
    float x;
    float y;
};
struct Velocity {
    float x;
    float y;
};
struct Health {
    int max;
    int current;
};

E4::Texture texture;

void gldPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
    // This code is based off the MESA source for gluPerspective
    // *NOTE* This assumes GL_PROJECTION is the current matrix


    GLdouble xmin, xmax, ymin, ymax;
    GLdouble m[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    ymax = zNear * tan(fovy * M_PI / 360.0);
    ymin = -ymax;

    xmin = ymin * aspect;
    xmax = ymax * aspect;

    // Set up the projection matrix
    m[0 + 0 * 4] = (2.0 * zNear) / (xmax - xmin);
    m[1 + 1 * 4] = (2.0 * zNear) / (ymax - ymin);
    m[2 + 2 * 4] = -(zFar + zNear) / (zFar - zNear);

    m[0 + 2 * 4] = (xmax + xmin) / (xmax - xmin);
    m[1 + 2 * 4] = (ymax + ymin) / (ymax - ymin);
    m[3 + 2 * 4] = -1.0;

    m[2 + 3 * 4] = -(2.0 * zFar * zNear) / (zFar - zNear);

    // Add to current matrix
    glMultMatrixd(m);
}

void Display_InitGL() {
    glShadeModel(GL_SMOOTH);
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

//    texture.name = "street.jpg";
//    texture.name = "favicon.png";
    texture.name = "footprint.png";
    texture.load();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int Display_SetViewport(int width, int height) {
    GLfloat ratio;

    if (height == 0) {
        height = 1;
    }

    ratio = GLfloat(width) / GLfloat(height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gldPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return true;
}

float basex = 0;
float basey = 0;

void Display_Render(const E4::FrameState& frameState) {
//    std::cout << frameState.deltatime << std::endl;
    if (!frameState.inputStatePrev.mouseButton1 && frameState.inputStateCurr.mouseButton1) {
        std::cout << "button1 down" << std::endl;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(basex - 1.5f, basey + 0.0f, -6.0f);

    glDisable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 1.0f, 0.0f);
//    glColor3i(255, 0, 0);
    glVertex3f(-1.0f, -1.0f, 0.0f);
//    glColor3i(255, 0, 0);
    glVertex3f(1.0f, -1.0f, 0.0f);
//    glColor3i(255, 0, 0);
    glEnd();

    glTranslatef(3.0f, 0.0f, 0.0f);

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.textureId);
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
//    glColor3i(255, 0, 0);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
//    glColor3i(255, 0, 0);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
//    glColor3i(255, 0, 0);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
//    glColor3i(255, 0, 0);
    glEnd();
}


int main(int argc, char* argv[]) {
    sol::state lua;
    int x = 0;
    lua.set_function("beep", [&x] { ++x; });
    lua.script("beep()");
    std::cout << x << std::endl;


    //CNtity
    CNtity::CNtity<Position, Velocity, Health, std::string> cntity;

    //Creating entities
    auto chat = cntity.create<std::string>({"chat"});
    cntity.create<std::string>({"chien"});
    cntity.create<std::string, Position>({"velociraptor"}, {25, 70});

    //Adding component, changing values
    auto position = cntity.add<Position>(chat, {50, 50});
    position->x += 50;

    //System 1
    cntity.for_each<std::string, Position>([&cntity](auto entity, auto identity) {
        if (*identity == "chat") {
            auto position = cntity.get<Position>(entity);
            position->x = 200;
            position->y = 70;

            return;
        }
    });

    //System 2
    for (const auto& it: cntity.acquire<std::string, Position>()) {
        if (*cntity.get<std::string>(it) == "chat") {
            auto position = cntity.get<Position>(it);
            std::cout << "position: (" << position->x << ", " << position->y << ")" << std::endl;
        }
    }

    //Removing a component, erasing an entity
    cntity.remove<Position>(chat);
    cntity.erase(chat);


    E4::Window window;

    int width = 640;
    int height = 480;
    window.create({std::string("abc"), width, height, false, false});

    // initialize opengl
    Display_InitGL();
    // set camera
    Display_SetViewport(window.getWidth(), window.getHeight());

    window.enterEventLoop(Display_Render);

    return 0;
}