#include <SDL.h>

#include "core/App.h"
#include <sstream>

int main(int argc, char* argv[]) {
    std::string folder = ".";
    if (argc == 2) {
        folder = argv[1];
    }
    E4::App app(800, 500, folder);

    E4::Log::setFilter("before load");

    app.load([](E4::App& app) {
        app.scene.start();
    });

    E4::Log::setFilter("");

    app.enterLoop([](E4::App& app, const E4::FrameState& frameState) {
        if (frameState.keyup(SDL_SCANCODE_F5)) {
            app.scene.reloadScript();
        }
        if (frameState.keyup(SDL_SCANCODE_F6)) {
            app.scene.reloadAll();
        }

        static bool showInput = false;
        if (frameState.keyup(SDL_SCANCODE_F4)) {
            showInput = !showInput;
        }
        if (showInput) {
            std::stringstream ss;
            ss << "X: " << frameState.inputStateCurr.mouseX << " Y: " << frameState.inputStateCurr.mouseY << " keys: ";
            for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
                if (frameState.inputStateCurr.keys[i]) {
                    ss << i << ' ';
                }
            }
            app.window.setTitle(ss.str());
        }
    });

    return 0;
}