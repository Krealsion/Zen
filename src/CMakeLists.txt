INCLUDE(FindPkgConfig)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/Zen/sdl_c_make_files")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_library(Zen
        anything_storage.h
        timer.cpp
        graphics/renderer.cpp
        graphics/game_graphics.cpp
        graphics/window.cpp
        graphics/particle.cpp
        graphics/sprite.h
        graphics/texture.cpp
        state_management/game_state_manager.cpp
        state_management/game_state.h
        types/plane_bounded.cpp
        types/function.h
        types/rectangle.cpp
        types/vector2.cpp
        types/vector3.cpp
        types/color.cpp
        graphics/texture_manager.cpp graphics/texture_manager.h graphics/drawable.h)

target_include_directories(Zen PUBLIC
        ../src
        types/
        state_management/
        graphics/
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR})

target_link_libraries(Zen
        pthread
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES})
