#include <SDL.h>

#include "core/App.h"
#include "components/Transform.h"
#include "components/Script.h"

int main(int argc, char* argv[]) {
    std::string folder = ".";
    if (argc == 2) {
        folder = argv[1];
    }
    E4::App app(500, 500, folder);

    app.load([](E4::App& app) {
        E4::Entity e = app.ecs.createEntity();
        app.ecs.createTransform(e);
        E4::Script& script = app.ecs.createScript(e);
        script.loaded = false;
        script.file = app.scripts.get("main.lua");
    });

    app.enterLoop([](E4::App& app, const E4::FrameState& frameState) {
        if (!frameState.inputStatePrev.keys[SDL_SCANCODE_F5] && frameState.inputStateCurr.keys[SDL_SCANCODE_F5]) {
            app.ecs.view<E4::Script>().each([](E4::Script& script) {
                script.file->scriptLoaded = false;
            });
        }
    });

    return 0;
}