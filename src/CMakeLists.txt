INCLUDE(FindPkgConfig)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/Zen/sdl_c_make_files")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_library(Zen
        timer.cpp
        var_storage.h
        graphics/drawable.h
        graphics/game_graphics.cpp
        graphics/particle.cpp
        graphics/renderer.cpp
        graphics/sprite.h
        graphics/texture.cpp
        graphics/texture_manager.cpp
        graphics/window.cpp
        state_management/game_state.h
        state_management/game_state_manager.cpp
        types/color.cpp
        types/function.h
        types/plane_bounded.cpp
        types/rectangle.cpp
        types/vector2.cpp
        types/vector3.cpp
        )

target_include_directories(Zen PUBLIC
        ../src
        graphics/
        state_management/
        types/
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR})

target_link_libraries(Zen
        pthread
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES})
