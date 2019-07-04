#include <SDL.h>

#include "core/App.h"
#include "components/Transform.h"
#include "components/Script.h"

int main(int argc, char* argv[]) {
    std::string folder = ".";
    if (argc == 2) {
        folder = argv[1];
    }
    E4::App app(800, 500, folder);

    app.load([](E4::App& app) {
        E4::Entity& e = app.scene.newEntity();
        app.scene.createTransform(e);
        app.scene.createScript(e, "main.lua");


        E4::Entity& e2 = app.scene.newEntity();
        E4::Transform& transform = app.scene.createTransform(e2);
        transform.z = 10;
        auto& env =app.ecs.create<E4::Env>(e2);
        env.camera.enabled = true;

//        app.scriptRunner.run(R"(
//            local e = newEntity()
//            createTransform(e)
//            createScript(e, "main.lua")
//        )");
    });

    app.enterLoop([](E4::App& app, const E4::FrameState& frameState) {
        if (!frameState.inputStatePrev.keys[SDL_SCANCODE_F5] && frameState.inputStateCurr.keys[SDL_SCANCODE_F5]) {
            app.ecs.fortype<E4::Script>([](E4::Entity& entity, E4::Script& script) {
                script.file->scriptLoaded = false;
            });
        }

        //static float t = 0;
        //t += 0.001f * frameState.deltatime;
        //auto& env = app.ecs.get<E4::Env>(app.ecs.getEntityByIndex(1));
        //env.camera.target.x = 0.3*std::sin(t);
        //env.camera.target.y = 0.3*std::cos(t);
    });

    return 0;
}