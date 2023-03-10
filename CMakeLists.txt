cmake_minimum_required(VERSION 3.20)

project(salamander)
set(CMAKE_CXX_STANDARD 20)

include_directories(sdl/include)
link_directories(sdl/lib/x64)

file(GLOB SRC_FILES src/*.cpp)
add_executable(salamander ${SRC_FILES})
target_link_libraries(salamander SDL2main SDL2)