#include <SDL.h>

#include "core/App.h"

int main(int argc, char* argv[]) {
    std::string folder = ".";
    if (argc == 2) {
        folder = argv[1];
    }
    E4::App app(800, 500, folder);

    app.load([](E4::App& app) {
        app.scene.start();
    });

    app.enterLoop([](E4::App& app, const E4::FrameState& frameState) {
        if (frameState.keyup(SDL_SCANCODE_F5)) {
            app.scene.reloadScript();
        }
        if (frameState.keyup(SDL_SCANCODE_F6)) {
            app.scene.reloadAll();
        }
    });

    return 0;
}