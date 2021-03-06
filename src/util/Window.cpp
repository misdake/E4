#include "Window.h"
#include "Log.h"

#include <SDL.h>
#include <glbinding/glbinding.h>

E4::Window::Window(FrameState& _frameState) : frameState(_frameState) {
    frameState.frameIndex = 0;
    prevTime = -1;

    gWindow = nullptr;
    gGlContext = nullptr;

    // initialize sdl
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        Log::error("SDL cannot init with error %s", SDL_GetError());
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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
        Log::error("Cannot create window with error %s", SDL_GetError());
        return false;
    }
    gGlContext = SDL_GL_CreateContext(gWindow);
    SDL_GL_SetSwapInterval(1);
    if (gGlContext == nullptr) {
        Log::error("Cannot create OpenGL context with error %s", SDL_GetError());
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

void E4::Window::getInputState() {
    InputState& inputStatePrev = frameState.inputStatePrev;
    InputState& inputStateCurr = frameState.inputStateCurr;
    memcpy(&inputStatePrev, &inputStateCurr, sizeof(InputState));

    int mouseX = -1;
    int mouseY = -1;
    Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);

    inputStateCurr.mouseX = mouseX;
    inputStateCurr.mouseY = mouseY;
    inputStateCurr.mouseButton1 = buttons & 0x1u;
    inputStateCurr.mouseButton2 = buttons & 0x2u;
    inputStateCurr.mouseButton3 = buttons & 0x4u;

    const Uint8* keystates = SDL_GetKeyboardState(nullptr);

    memcpy(inputStateCurr.keys, keystates, SDL_NUM_SCANCODES * sizeof(Uint8));

    Uint32 flags = SDL_GetWindowFlags(gWindow);
    if (config.mouseTrap && (flags & SDL_WINDOW_MOUSE_FOCUS)) {
        inputStateCurr.mouseX = mouseX - getWidth() / 2;
        inputStateCurr.mouseY = mouseY - getHeight() / 2;
        SDL_WarpMouseInWindow(gWindow, getWidth() / 2, getHeight() / 2);
    }
}

void E4::Window::enterEventLoop(const std::function<void(const FrameState&)>& onFrame) {
    bool quit = false;
    SDL_Event event{};
    while (!quit) {

        getInputState();

        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEWHEEL: {
                    int wheelx = event.wheel.x;
                    int wheely = event.wheel.y;
                    frameState.inputStateCurr.wheelX += wheelx;
                    frameState.inputStateCurr.wheelY += wheely;
                    break;
                }
                default:
                    break;
            }
        }

        frameState.width = getWidth();
        frameState.height = getHeight();
        frameState.frameIndex++;
        long currTime = SDL_GetTicks();
        if (prevTime < 0) prevTime = currTime;
        long d = currTime - prevTime;
        prevTime = currTime;
        frameState.deltatime = d >= 0 && d < 1000 ? (uint16_t) d : 1000;

        onFrame(frameState);

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
