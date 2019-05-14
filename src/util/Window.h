#pragma once

#include <string>
#include <tuple>
#include <iostream>

#include "InputState.h"

struct SDL_Window;
typedef void* SDL_GLContext;

namespace E4 {

    struct WindowConfig {
        std::string title;
        int width;
        int height;
        bool borderless;
        bool mouseTrap;
    };

    struct FrameState {
        FrameState(uint16_t deltatime, InputState& inputStateCurr, InputState& inputStatePrev);
        uint16_t deltatime;
        InputState& inputStateCurr;
        InputState& inputStatePrev;
    };

    class Window {
    private:
        SDL_Window* gWindow;
        SDL_GLContext gGlContext;

        WindowConfig config;

        long prevTime;
        InputState inputStateCurr;
        InputState inputStatePrev;

    private:
        void getInputState();

    public:
        Window();

        bool create(const WindowConfig& inConfig);

        bool isBorderless();
        void setBorderless(bool borderless);

        bool isMouseTrap();
        void setMouseTrap(bool mouseTrap);

        int getWidth();
        int getHeight();
        void setSize(int width, int height);

        std::string getTitle();
        void setTitle(const std::string& title);

        void enterEventLoop(void (* onFrame)(const FrameState& frameState));

        static std::tuple<int, int> getScreenSize();

        ~Window();

    };

}