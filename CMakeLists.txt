cmake_minimum_required(VERSION 3.8)
project(e4)

set(CMAKE_CXX_STANDARD 17)

#SDL2
set(SDL2_FLAGS "-lmingw32 -lSDL2main -lSDL2 -mwindows -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -lsetupapi -lhid")
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} ${SDL2_FLAGS}")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

#Lua,CNtity
include_directories(include)
link_directories(library)

#glbinding
find_package(glbinding REQUIRED)
include_directories(${GLBINDING_INCLUDE_DIRS})

add_executable(app
        src/components/Position.h
        src/components/Drawable.h
        src/systems/render/Renderer.cpp src/systems/render/Renderer.h
        src/systems/render/Texture.cpp src/systems/render/Texture.h
        src/systems/render/opengl/GlRenderer.cpp src/systems/render/opengl/GlRenderer.h
        src/systems/render/opengl/Shader.cpp src/systems/render/opengl/Shader.h
        src/systems/render/opengl/FloatBuffer.cpp src/systems/render/opengl/FloatBuffer.h
        src/systems/render/opengl/AttributeSlot.cpp src/systems/render/opengl/AttributeSlot.h
        src/systems/render/opengl/UniformSlot.cpp src/systems/render/opengl/UniformSlot.h
        src/systems/render/opengl/DataType.h
        src/systems/Asset.cpp src/systems/Asset.h
        src/util/File.cpp src/util/File.h
        src/util/Window.cpp src/util/Window.h
        src/util/InputState.cpp src/util/InputState.h
        src/main.cpp)
target_link_libraries(app lua53 ${SDL2_LIBRARY} ${GLBINDING_LIBRARIES})

#output executable
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/env")
set(outputname "app${CMAKE_BUILD_TYPE}")
set_target_properties(app PROPERTIES OUTPUT_NAME ${outputname})