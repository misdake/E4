#pragma once

#include <iostream>
#include <glbinding/gl/gl.h>

using namespace gl;

namespace E4 {

    class GlRenderer {
    public:
        int width = 0;
        int height = 0;

        void resize(int w, int h) {
            width = w;
            height = h;
            glViewport(0, 0, w, h);
        }

        void init() {
            glShadeModel(GL_SMOOTH);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClearDepth(1.0f);
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
            glDepthFunc(GL_LEQUAL);
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

            //TODO move to glstate
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        void clear() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void checkError() {
            GLenum error = glGetError();
            if (error != GL_NO_ERROR) {
                std::cout << "error" << std::endl;
            }
        }

    };

}
