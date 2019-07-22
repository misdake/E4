#include "Recycler.h"

#include <iostream>

void E4::Recycler::run(E4::Ecs& ecs, const E4::FrameState& state) {
    std::cout << state.frameIndex << std::endl;
    ecs.fortype<Script>([](E4::Script& script) {
        std::cout << script.file->name << std::endl;
    });
}
