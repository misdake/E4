#include "Window.h"

#include <SDL.h>
#include <glbinding/glbinding.h>

E4::Window::Window() {
    gWindow = nullptr;
    gGlContext = nullptr;

    // initialize sdl
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL cannot init with error " << SDL_GetError() << std::endl;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
}

bool E4::Window::create(const E4::WindowConfig& inConfig) {
    config = inConfig;
    gWindow = SDL_CreateWindow(config.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.width, config.height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        std::cout << "Cannot create window with error " << SDL_GetError() << std::endl;
        return false;
    }
    gGlContext = SDL_GL_CreateContext(gWindow);
    if (gGlContext == nullptr) {
        std::cout << "Cannot create OpenGL context with error " << SDL_GetError() << std::endl;
        return false;
    }

    setBorderless(config.borderless);
    setMouseTrap(config.mouseTrap);

    glbinding::initialize([](const char* name) -> void (*)() { return (void (*)()) SDL_GL_GetProcAddress(name); }, true);

    return true;
}

bool E4::Window::isBorderless() {
    return config.borderless;
}

void E4::Window::setBorderless(bool borderless) {
    config.borderless = borderless;
    SDL_SetWindowBordered(gWindow, !borderless ? SDL_TRUE : SDL_FALSE);
}

bool E4::Window::isMouseTrap() {
    return config.mouseTrap;
}

void E4::Window::setMouseTrap(bool mouseTrap) {
    config.mouseTrap = mouseTrap;
    if (mouseTrap) {
        SDL_ShowCursor(0);
        SDL_SetWindowGrab(gWindow, SDL_TRUE);
//        TODO every frame
//        SDL_WarpMouseInWindow(gWindow, getWidth() / 2, getHeight() / 2);
    } else {
        SDL_ShowCursor(1);
        SDL_SetWindowGrab(gWindow, SDL_FALSE);
    }
}

int E4::Window::getWidth() {
    return config.width;
}

int E4::Window::getHeight() {
    return config.height;
}

void E4::Window::setSize(int width, int height) {
    config.width = width;
    config.height = height;
    SDL_SetWindowSize(gWindow, width, height);
}

std::string E4::Window::getTitle() {
    return config.title;
}

void E4::Window::setTitle(const std::string& title) {
    config.title = title;
    SDL_SetWindowTitle(gWindow, title.c_str());
}

std::tuple<int, int> E4::Window::getScreenSize() {
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    return {DM.w, DM.h};
}

void E4::Window::enterEventLoop(void (*onFrame)()) {
    bool quit = false;
    SDL_Event event{};
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                default:
                    break;
            }
        }

        //TODO inputstate

        onFrame();

        SDL_GL_SwapWindow(gWindow);
    }
}

E4::Window::~Window() {
    if (gWindow) SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    if (gGlContext) SDL_GL_DeleteContext(gGlContext);
    gGlContext = nullptr;

    SDL_Quit();
}
