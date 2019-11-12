cmake_minimum_required(VERSION 3.13)
project(TimerExamples)

set(CMAKE_CXX_STANDARD 17)

link_libraries(pthread)

add_executable(TimerExamples Examples/TimerExamples.cpp src/Timer.cpp)

project(Vector2Examples)

set(CMAKE_CXX_STANDARD 17)

add_executable(Vector2Examples Examples/Vector2Examples.cpp src/Vector2.cpp)
