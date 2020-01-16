INCLUDE(FindPkgConfig)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/GameEngine/SDLCMakeFiles")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_library(GameEngine
        GameStateManager.cpp
        Rectangle.cpp
        Renderer.cpp
        Timer.cpp
        Vector2.cpp
        Vector3.cpp
        GameGraphics.cpp
        Color.cpp
        Window.cpp
        PlaneBounded.cpp
        Collision2D.cpp
        Line2D.cpp
        Camera.cpp)

target_include_directories(GameEngine PUBLIC
        ../include
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR})

target_link_libraries(GameEngine
        pthread
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES})
