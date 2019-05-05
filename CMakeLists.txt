cmake_minimum_required(VERSION 3.8)
project(e4)

set(CMAKE_CXX_STANDARD 17)

#SDL2
set(SDL2_FLAGS "-lmingw32 -lSDL2main -lSDL2 -mwindows -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -lsetupapi -lhid")
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} ${SDL2_FLAGS}")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

#CNtity
include_directories(include)
link_directories(library)

add_executable(app src/main.cpp)
target_link_directories(app PRIVATE library)
target_link_libraries(app ${SDL2_LIBRARY} lua53 opengl32 glu32)

#output executable
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/env")
set(outputname "app${CMAKE_BUILD_TYPE}")
set_target_properties(app PROPERTIES OUTPUT_NAME ${outputname})