cmake_minimum_required(VERSION 3.24)
project(Zengine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)

add_library(Zen
        timer.cpp
        message_bus/var_storage.h
        user_interface/custom_layout.h
        graphics/game_graphics.cpp
        graphics/particle.cpp
        graphics/renderer.cpp
        graphics/sprite.h
        graphics/texture.cpp
        graphics/texture_manager.cpp
        graphics/window.cpp
        state_management/game_state.h
        state_management/game_state.cpp
        state_management/game_state_manager.cpp
        types/color.cpp
        types/function.h
        types/plane_bounded.cpp
        types/rectangle.cpp
        types/vector2.cpp
        types/vector3.cpp
        input/input.cpp
        input/input.h
        types/3d/sphere.cpp
        types/3d/sphere.h
        graphics_3d/engine_3d.cpp
        graphics_3d/engine_3d.h
        logic/math.cpp
        logic/math.h
        graphics_3d/camera.cpp
        graphics_3d/camera.h
        user_interface/button.cpp
        user_interface/button.h
        user_interface/text_box.cpp
        graphics/enums.h
        user_interface/custom_layout.cpp
        zsingal.h
        utility_states/build_state.cpp
        callback.h
        user_interface/text.cpp
        user_interface/text.h
        user_interface/drop_down.cpp
        user_interface/drop_down.h
        logic/utils.h
        user_interface/scroll_view.cpp
        user_interface/scroll_view.h)

target_include_directories(Zen PUBLIC
        ../src
        graphics/
        input/
        message_bus/
        state_management/
        types/)

target_link_libraries(Zen
        PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        PRIVATE
        Threads::Threads
)

find_package(Threads REQUIRED)

add_library(Zen::Zen ALIAS Zen)