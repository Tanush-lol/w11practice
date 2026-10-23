cmake_minimum_required(VERSION 3.20)

project(roti LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)

pkg_check_modules(WAYLAND REQUIRED IMPORTED_TARGET wayland-client)
pkg_check_modules(CAIRO REQUIRED IMPORTED_TARGET cairo)

add_executable(roti
    src/main.cpp
    src/shm.cpp
    src/cairo.cpp
    src/xdg_surface.cpp
    src/buffer.cpp
    src/xdg-shell-protocol.o
)

target_include_directories(roti PRIVATE include)

target_link_libraries(roti PRIVATE
    PkgConfig::WAYLAND
    PkgConfig::CAIRO
)

target_compile_options(roti PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
