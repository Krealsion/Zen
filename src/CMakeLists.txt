INCLUDE(FindPkgConfig)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/Zen/sdl_c_make_files")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_library(Zen
        state_management/game_state_manager.cpp
        types/rectangle.cpp
        graphics/renderer.cpp
        timer.cpp
        types/vector2.cpp
        types/vector3.cpp
        graphics/game_graphics.cpp
        types/color.cpp
        graphics/window.cpp types/plane_bounded.cpp graphics/particle.cpp graphics/particle.h types/function.h anything_storage.h)

target_include_directories(Zen PUBLIC
        ../src
        types/
        state_management/
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR})

target_link_libraries(Zen
        pthread
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES})
