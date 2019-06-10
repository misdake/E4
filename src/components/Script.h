#pragma once

namespace E4 {

    struct Script {
        bool loaded;
        std::string load;
        std::string update;

//        Script() {
//            int x = 0;
//            lua.set_function("beep", [&x] { ++x; });
//            lua.script("beep()");
//            std::cout << x << std::endl;
//        }
    };

}