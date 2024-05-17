INCLUDE(FindPkgConfig)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/Zen/sdl_c_make_files")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_library(Zen
        timer.cpp
        message_bus/var_storage.h
        graphics/user_interface/custom_layout.h
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
        input/input_manager.cpp
        input/input_manager.h
        types/3d/sphere.cpp
        types/3d/sphere.h
        types/3d/triangle.cpp
        types/3d/triangle.h
        graphics_3d/engine_3d.cpp
        graphics_3d/engine_3d.h
        logic/math.cpp
        logic/math.h
        graphics_3d/camera.cpp
        graphics_3d/camera.h
        graphics/user_interface/button.cpp
        graphics/user_interface/button.h
        graphics/enums.h
        graphics/user_interface/custom_layout.cpp
        zsingal.h
        callback.h
        graphics/user_interface/text.cpp
        graphics/user_interface/text.h)

target_include_directories(Zen PUBLIC
        ../src
        graphics/
        input/
        message_bus/
        state_management/
        types/
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_GFX_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIR})

target_link_libraries(Zen
        pthread
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_GFX_LIBRARIES}
        ${SDL2_TTF_LIBRARIES})
