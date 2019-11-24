cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)

project(TimerExamples)
link_libraries(pthread)
add_executable(TimerExamples TimerExamples.cpp ../src/Timer.cpp)

project(Vector2Examples)
add_executable(Vector2Examples Vector2Examples.cpp ../src/Vector2.cpp)
