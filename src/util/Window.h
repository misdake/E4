#pragma once

#include <string>
#include <tuple>
#include <functional>

#include "InputState.h"
#include "FrameState.h"

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

    class Window {
    private:
        SDL_Window* gWindow;
        SDL_GLContext gGlContext;

        WindowConfig config;

        long prevTime;
        FrameState& frameState;

    private:
        void getInputState();

    public:
        explicit Window(FrameState& frameState);

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

        void enterEventLoop(const std::function<void(const FrameState&)>& onFrame);

        static std::tuple<int, int> getScreenSize();

        ~Window();

    };

}