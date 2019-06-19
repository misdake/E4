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
        app.scriptRunner.run(R"(
            local e = createEntity()
            createTransform(e)
            createScript(e, "main.lua")
        )");
    });

    app.enterLoop([](E4::App& app, const E4::FrameState& frameState) {
        if (!frameState.inputStatePrev.keys[SDL_SCANCODE_F5] && frameState.inputStateCurr.keys[SDL_SCANCODE_F5]) {
            app.ecs.fortype<E4::Script>([](E4::Entity& entity, E4::Script& script) {
                script.file->scriptLoaded = false;
            });
        }
    });

    return 0;
}