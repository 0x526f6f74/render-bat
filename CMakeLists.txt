cmake_minimum_required(VERSION 3.18)

project(RenderBat)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glad)

add_executable(
    RenderBat
    buffer.cpp
    buffer.h
    camera.cpp
    camera.h
    main.cpp
    shader.cpp
    shader.h
    cubemap.cpp
    cubemap.h
    offscreen.cpp
    offscreen.h
    state.cpp
    state.h
    window.cpp
    window.h
    lib/glad/glad.h
    lib/glfw/include/GLFW/glfw3.h
    lib/stb/stb_image.h
)

target_include_directories(RenderBat PRIVATE lib lib/glfw/include)

target_link_libraries(RenderBat PRIVATE glad glfw png)
