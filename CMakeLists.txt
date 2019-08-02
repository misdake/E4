cmake_minimum_required(VERSION 3.8)
project(e4)

set(CMAKE_CXX_STANDARD 17)

#SDL2
set(SDL2_FLAGS "-lmingw32 -lSDL2main -lSDL2 -lwinmm -limm32 -lversion -lsetupapi")
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} ${SDL2_FLAGS} -flto -fno-exceptions")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
SET(CMAKE_AR "gcc-ar")
SET(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> qcs <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_C_ARCHIVE_FINISH true)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

#Lua
include_directories(include)
link_directories(library)

#glbinding
find_package(glbinding REQUIRED)
include_directories(${GLBINDING_INCLUDE_DIRS})

IF (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-DDEBUG)
ENDIF()

add_executable(app
        src/main.cpp

        src/core/App.cpp src/core/App.h
        src/core/Asset.cpp src/core/Asset.h
        src/core/ECS.cpp  src/core/ECS.h
        src/core/Scene.cpp src/core/Scene.h
        src/core/Lua.h
        src/core/ScriptBridge.cpp src/core/ScriptBridge.h

        src/assets/MeshLoader_builtin.cpp src/assets/MeshLoader_builtin.h
        src/assets/MeshLoader_obj.cpp src/assets/MeshLoader_obj.h
        src/assets/ObjFileLoader.cpp src/assets/ObjFileLoader.h

        src/components/Components.h
        src/components/Transform.h
        src/components/Drawable.h
        src/components/Script.h
        src/components/Env.h

        src/systems/env/Environment.cpp src/systems/env/Environment.h
        src/systems/env/EnvBuilder.cpp src/systems/env/EnvBuilder.h
        src/systems/transform/Transformer.cpp src/systems/transform/Transformer.h
        src/systems/render/Renderer.cpp src/systems/render/Renderer.h
        src/systems/render/opengl/Texture.cpp src/systems/render/opengl/Texture.h
        src/systems/render/opengl/GlRenderer.cpp src/systems/render/opengl/GlRenderer.h
        src/systems/render/opengl/Shader.cpp src/systems/render/opengl/Shader.h
        src/systems/render/opengl/Shaders.cpp src/systems/render/opengl/Shaders.h
        src/systems/render/opengl/FloatBuffer.cpp src/systems/render/opengl/FloatBuffer.h
        src/systems/render/opengl/ShortBuffer.cpp src/systems/render/opengl/ShortBuffer.h
        src/systems/render/opengl/AttributeSlot.cpp src/systems/render/opengl/AttributeSlot.h
        src/systems/render/opengl/UniformSlot.cpp src/systems/render/opengl/UniformSlot.h
        src/systems/render/opengl/DataType.cpp src/systems/render/opengl/DataType.h
        src/systems/script/ScriptRunner.cpp src/systems/script/ScriptRunner.h
        src/systems/script/ScriptFile.cpp src/systems/script/ScriptFile.h
        src/systems/recycler/Recycler.cpp src/systems/recycler/Recycler.h

        src/util/File.cpp src/util/File.h
        src/util/Window.cpp src/util/Window.h
        src/util/InputState.cpp src/util/InputState.h
        src/util/FrameState.h
        src/util/Log.cpp src/util/Log.h

        src/math/Mat4.cpp src/math/Mat4.h
        src/math/Vec3.cpp src/math/Vec3.h
        src/math/Color.cpp src/math/Color.h
        )
target_link_libraries(app lua53 ${SDL2_LIBRARY} ${GLBINDING_LIBRARIES})

#output executable
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/env")
set(outputname "app${CMAKE_BUILD_TYPE}")
set_target_properties(app PROPERTIES OUTPUT_NAME ${outputname})