Zen is a game engine that is designed to be exceptionally easy to use by adding numerous tools for the programmer, without taking any abilities away.

This engine is not supposed to be a replacement for c++ development, but rather a powerful addition that can easily be modified, tweaked, and improved for different projects.

It does so by giving a strong base, which is a GameState management system, letting the programmer jump right into making a game, rather than setting up a window, a renderer, a update cycle, and so on.

It also makes use of multi-threading, although rudimentary, to separate out drawing and updates. (Eventually to use hardware acceleration as well)

## How to Use

### CMake:

In order to implement Zen, you will need to add it to the CMakeLists of the project you are building

An example CMakeLists.txt file is:
```cmake
project(Project VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Zen)

add_executable(Project main.cpp)

target_include_directories(Project PUBLIC Zen)
target_link_libraries(Project Zen)
```

Libraries the Zen engine uses are:
- [SDL3]()
- [SDL3_image](https://github.com/libsdl-org/SDL_image)
- [SDL3_ttf](https://github.com/libsdl-org/SDL_ttf)
