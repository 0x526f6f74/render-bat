cmake_minimum_required(VERSION 3.18)

project(RenderBat)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib/glad)
add_subdirectory(lib/glfw)
add_subdirectory(lib/nbt)

add_executable(
    RenderBat
    src/buffer.cc
    src/buffer.h
    src/camera.cc
    src/camera.h
    src/constants.h
    src/cubemap.cc
    src/cubemap.h
    src/main.cc
    src/offscreen.cc
    src/offscreen.h
    src/shader.cc
    src/shader.h
    src/state.h
    src/vertex.h
    src/window.cc
    src/window.h
    src/world.cc
    src/world.h
)

target_compile_definitions(RenderBat PRIVATE RB_REAL_TIME GLFW_INCLUDE_NONE)

target_precompile_headers(RenderBat PRIVATE <array> <cstring> <filesystem> <fstream> <functional> <iostream> <map> <string> <unordered_map> <vector> <glm/glm.hpp> <glm/gtc/matrix_transform.hpp>)

target_include_directories(RenderBat PRIVATE lib lib/glfw/include)

target_link_libraries(RenderBat PRIVATE stdc++fs glad glfw nbt png)
