/**
 * Simple SDL2 program using OpenGL as rendering pipeline.
 */

#include <iostream>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <sstream>
#include <iostream>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Window* gWindow = nullptr;
SDL_GLContext gGlContext;

void SetOpenGLVersion() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
}

void Display_InitGL() {
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
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

    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return true;
}

float basex = 0;
float basey = 0;

void Display_Render() {
    glLoadIdentity();
    glTranslatef(basex - 1.5f, basey + 0.0f, -6.0f);

    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    glTranslatef(3.0f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glEnd();
}

void close() {
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    SDL_Quit();
}

int main(int argc, char* argv[]) {
    // initialize sdl
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL cannot init with error " << SDL_GetError() << std::endl;
        return -1;
    }

    // set opengl version to use in this program
    SetOpenGLVersion();

    // create window
    gWindow = SDL_CreateWindow("Opengl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        std::cout << "Cannot create window with error " << SDL_GetError() << std::endl;
        return -1;
    }

    // create opengl context
    gGlContext = SDL_GL_CreateContext(gWindow);
    if (gGlContext == nullptr) {
        std::cout << "Cannot create OpenGL context with error " << SDL_GetError() << std::endl;
        return -1;
    }

    // initialize opengl
    Display_InitGL();
    // set camera
    Display_SetViewport(SCREEN_WIDTH, SCREEN_HEIGHT);

    bool quit = false;
    SDL_Event event{};
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
//                    switch (event.key.keysym.scancode) {
//                        case SDL_SCANCODE_LEFT:
//                            basex -= 0.1;
//                            break;
//                        case SDL_SCANCODE_RIGHT:
//                            basex += 0.1;
//                            break;
//                        case SDL_SCANCODE_UP:
//                            basey += 0.1;
//                            break;
//                        case SDL_SCANCODE_DOWN:
//                            basey -= 0.1;
//                            break;
//                        default:
//                            break;
//                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
//                    switch (event.button.button) {
//                        case SDL_BUTTON_LEFT:
//                            SDL_ShowSimpleMessageBox(0, "Mouse", "Left button was pressed!", gWindow);
//                            break;
//                        case SDL_BUTTON_RIGHT:
//                            SDL_ShowSimpleMessageBox(0, "Mouse", "Right button was pressed!", gWindow);
//                            break;
//                        case SDL_BUTTON_MIDDLE:
//                            SDL_ShowSimpleMessageBox(0, "Mouse", "Middle button was pressed!", gWindow);
//                            break;
//                        default:
//                            SDL_ShowSimpleMessageBox(0, "Mouse", "Some other button was pressed!", gWindow);
//                            break;
//                    }
                    break;
                case SDL_MOUSEMOTION:
                    int mouseX = event.motion.x;
                    int mouseY = event.motion.y;

                    std::stringstream ss;
                    ss << "X: " << mouseX << " Y: " << mouseY;

                    SDL_SetWindowTitle(gWindow, ss.str().c_str());
                    break;
            }
        }

        //Get the keystates
        const Uint8* keystates = SDL_GetKeyboardState(nullptr);
        if (keystates[SDL_SCANCODE_UP]) basey += 0.01;
        if (keystates[SDL_SCANCODE_DOWN]) basey -= 0.01;
        if (keystates[SDL_SCANCODE_LEFT]) basex -= 0.01;
        if (keystates[SDL_SCANCODE_RIGHT]) basex += 0.01;
        if (keystates[SDL_SCANCODE_W]) basey += 0.01;
        if (keystates[SDL_SCANCODE_S]) basey -= 0.01;
        if (keystates[SDL_SCANCODE_A]) basex -= 0.01;
        if (keystates[SDL_SCANCODE_D]) basex += 0.01;

        // clear bg color and depth buffer
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render stuff here
        Display_Render();

        // swap window inorder to update opengl
        SDL_GL_SwapWindow(gWindow);
    }

    // clear resource
    close();

    return 0;
}