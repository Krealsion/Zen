cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)

include_directories("../src")

project(TimerExamples)
link_libraries(pthread)
add_executable(TimerExamples timer_examples.cpp ../src/timer.cpp)

project(Vector2Examples)
add_executable(Vector2Examples vector2_examples.cpp ../src/types/vector2.cpp)

project(VarStorageExamples)
add_executable(VarStorageExamples var_storage_examples.cpp ../src/message_bus/var_storage.h ../src/timer.cpp)

project(MessageBusExamples)
add_executable(MessageBusExamples message_bus_example.cpp ../src/message_bus/data_packet.h ../src/message_bus/message.h ../src/message_bus/message_bus.h ../src/message_bus/message_listener.h ../src/timer.cpp)
