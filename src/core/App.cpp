#include "App.h"

#include "ScriptBridge.h"

E4::App::App(uint16_t width, uint16_t height, const std::string& folder) :
    width(width),
    height(height),
    textures(folder),
    scripts(folder),
    folder(folder) {

}

void E4::App::load(const std::function<void(App&)>& onLoaded) {
    window.create(
        {
            .title = std::string("abc"),
            .width = width,
            .height = height,
            .borderless = false,
            .mouseTrap = false,
        }
    );

    renderer.init();
    renderer.resize(width, height);

    ecs.state = scriptRunner.state;
    ecs.createEntity(); //make the '0' object

    ScriptBridge::load(*this, *ecs.state, ecs);

    onLoaded(*this);
}

void E4::App::enterLoop(const std::function<void(E4::App&, const E4::FrameState&)>& onFrame) {
    window.enterEventLoop([&onFrame, this](const FrameState& frameState) -> void {
        ScriptBridge::update(*this, *ecs.state, ecs);

        onFrame(*this, frameState);

        scriptRunner.run(ecs, frameState);
        transform.run(ecs, frameState);
        renderer.run(ecs, frameState);
    });
}
