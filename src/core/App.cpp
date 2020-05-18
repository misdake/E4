#include "App.h"

#include "ScriptBridge.h"

E4::App::App(uint16_t width, uint16_t height, const std::string& folder) :
    frameState(),
    window(frameState),
    scene(),
    width(width),
    height(height),
    folder(folder),
    textures(folder),
    scripts(folder) {
    memset(&frameState, 0, sizeof(frameState));
}

void E4::App::load(const std::function<void(App&)>& onLoaded) {
    window.create(
        {
            .title = std::string("E4"),
            .width = width,
            .height = height,
            .borderless = false,
            .mouseTrap = false,
        }
    );

    renderer.init();
    renderer.resize(width, height);

    scene.init(this, scriptRunner.state, &ecs);

    ScriptBridge::load(*this, *scriptRunner.state, ecs);

    onLoaded(*this);
}

void E4::App::enterLoop(const std::function<void(E4::App&, const E4::FrameState&)>& onFrame) {
    window.enterEventLoop([&onFrame, this](const FrameState& frameState) -> void {
        ScriptBridge::update(*this, *scriptRunner.state, ecs, frameState);

        onFrame(*this, frameState);
        scriptRunner.run(ecs, frameState);
        recycler.run(*this, ecs, frameState);

        envBuilder.run(ecs, frameState);
        transform.run(ecs, frameState, envBuilder.environment);
        renderer.run(ecs, frameState, envBuilder.environment);
    });
}
