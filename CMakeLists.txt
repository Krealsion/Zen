cmake_minimum_required(VERSION 3.24)
project(Zen LANGUAGES CXX)

# --- Standard & compile_commands.json ---
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clang-tidy

# --- Options ---
option(ZEN_ENABLE_WARNINGS "Enable strict warnings" ON)
option(ZEN_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(ZEN_ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ZEN_ENABLE_UBSAN "Enable UBSanitizer" OFF)
option(ZEN_ENABLE_LSAN "Enable LeakSanitizer (Linux/Clang)" OFF)

# --- Functions to apply warnings/sanitizers to targets ---
function(zen_enable_warnings target)
  if (NOT ZEN_ENABLE_WARNINGS)
    return()
  endif()

  if (MSVC)
    target_compile_options(${target} PRIVATE
      /W4 /permissive- /Zc:__cplusplus
      /external:W0
    )
    if (ZEN_WARNINGS_AS_ERRORS)
      target_compile_options(${target} PRIVATE /WX)
    endif()
  else()
    target_compile_options(${target} PRIVATE
      -Wall -Wextra -Wpedantic
      -Wconversion -Wsign-conversion
      -Wshadow -Wnon-virtual-dtor
      -Wold-style-cast -Woverloaded-virtual
      -Wnull-dereference -Wdouble-promotion
      -Wformat=2 -Wimplicit-fallthrough
      -Wextra-semi -Wmissing-declarations
    )
    if (ZEN_WARNINGS_AS_ERRORS)
      target_compile_options(${target} PRIVATE -Werror)
    endif()
  endif()
endfunction()

function(zen_enable_sanitizers target)
  if (MSVC)
    if (ZEN_ENABLE_ASAN)
      # MSVC ASan: x64 Debug recommended; requires /Zi and no incremental linking.
      target_compile_options(${target} PRIVATE /fsanitize=address /Zi)
      target_link_options(${target} PRIVATE /fsanitize=address /INCREMENTAL:NO)
    endif()
    # UBSan/LSan not broadly available on MSVC; use clang-cl or LLVM on Windows if needed.
  else()
    set(san_flags "")
    if (ZEN_ENABLE_ASAN)
      list(APPEND san_flags -fsanitize=address)
    endif()
    if (ZEN_ENABLE_UBSAN)
      list(APPEND san_flags -fsanitize=undefined)
    endif()
    if (ZEN_ENABLE_LSAN)
      list(APPEND san_flags -fsanitize=leak)
    endif()

    if (san_flags)
      target_compile_options(${target} PRIVATE ${san_flags} -fno-omit-frame-pointer -fno-optimize-sibling-calls)
      target_link_options(${target} PRIVATE ${san_flags})
    endif()
  endif()
endfunction()

# --- Define your library/executable targets as usual ---
# Example (adjust to your actual targets):
add_library(Zen STATIC
        src/graphics/enums.h
        src/graphics/game_graphics.h
        src/graphics/game_graphics.cpp
        src/graphics/renderer.h
        src/graphics/renderer.cpp
        src/graphics/sprite.h
        src/graphics/texture.h
        src/graphics/texture.cpp
        src/graphics/texture_manager.h
        src/graphics/texture_manager.cpp
        src/graphics/window.h
        src/graphics/window.cpp
        src/graphics_3d/engine_3d.h
        src/graphics_3d/engine_3d.cpp
        src/graphics_3d/camera.h
        src/graphics_3d/camera.cpp
        src/state_management/game_state.h
        src/state_management/game_state.cpp
        src/state_management/game_state_manager.h
        src/state_management/game_state_manager.cpp
        src/types/color.h
        src/types/color.cpp
        src/types/function.h
        src/types/plane_bounded.cpp
        src/types/rectangle.h
        src/types/rectangle.cpp
        src/types/vector2.h
        src/types/vector2.cpp
        src/types/3d/vector3.h
        src/types/3d/vector3.cpp
        src/input/input.h
        src/input/input.cpp
        src/types/3d/sphere.h
        src/types/3d/sphere.cpp
        src/logic/math.h
        src/logic/math.cpp
        src/user_interface/button.h
        src/user_interface/button.cpp
        src/user_interface/text_box.h
        src/user_interface/text_box.cpp
        src/user_interface/custom_layout.h
        src/user_interface/custom_layout.cpp
        src/zsignal.h
        src/message_bus/var_storage.h
        src/message_bus/socket.h
        src/utility_states/build_state.h
        src/utility_states/build_state.cpp
        src/callback.h
        src/user_interface/text.cpp
        src/user_interface/text.h
        src/user_interface/drop_down.cpp
        src/user_interface/drop_down.h
        src/logic/utils.h
        src/user_interface/scroll_view.cpp
        src/user_interface/scroll_view.h
        src/timer.cpp
        src/logger.cpp
        src/logger.h
        src/rttr_wrapper.h)

target_include_directories(Zen PUBLIC
        src/
        lib/
        nlohmann/
        src/graphics/
        src/input/
        src/message_bus/
        src/types/
        src/types/3d/
)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)
find_package(Threads REQUIRED)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(Zen PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf PRIVATE
        Threads::Threads
        nlohmann_json::nlohmann_json
)

add_library(Zen::Zen ALIAS Zen)

# Apply warnings/sanitizers
zen_enable_warnings(Zen)
zen_enable_sanitizers(Zen)

# If you have an examples executable:
add_executable(zen_example examples/vector2_examples.cpp)
target_link_libraries(zen_example PRIVATE Zen)
zen_enable_warnings(zen_example)
zen_enable_sanitizers(zen_example)
