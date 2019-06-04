#include "App.h"

void E4::App::load(const std::function<void(App&)>& onLoaded) {
    int width = 640;
    int height = 480;
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

    onLoaded(*this);
}

void E4::App::enterLoop(const std::function<void(E4::App&, const E4::FrameState&)>& onFrame) {
    window.enterEventLoop([&onFrame, this](const FrameState& frameState) -> void {
        onFrame(*this, frameState);

        renderer.run(scene);
    });
}
