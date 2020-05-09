cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)

include_directories("../src")

project(TimerExamples)
link_libraries(pthread)
add_executable(TimerExamples timer_examples.cpp ../src/timer.cpp)

project(Vector2Examples)
add_executable(Vector2Examples vector2_examples.cpp ../src/types/vector2.cpp)

project(AnythingStorageExamples)
add_executable(AnythingStorageExamples anything_storage_examples.cpp ../src/anything_storage.h)
