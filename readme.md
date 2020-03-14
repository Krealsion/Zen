This is a Game Engine for c++ designed to be an exceptionally easy to use layer between the programmer and SDL2.

The use of this engine is to allow for quicker development of games by using easy to manage Game States that can be easily created and plugged in to play. These game_states will also have access to a bunch of useful drawing methods in a consistent and stable environment.

The Engine also makes use of Multi-Threading to split the update and draw method functionality, allowing for a smoother framerate.

## How to Use

# CMAKE

In order to implement this GameEngine, you will need to add it to the CMakeLists of the project you are building

An example CMakeLists.txt file is:
```cmake
project(Project VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Zen)

add_executable(Project main.cpp)

target_include_directories(Project PUBLIC Zen)
target_link_libraries(Project Zen)
```
