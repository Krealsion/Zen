cmake_minimum_required(VERSION 3.13)
project(TimerExamples VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

link_libraries(pthread)

add_executable(TimerExamples TimerExamples.cpp ../src/Timer.cpp)
