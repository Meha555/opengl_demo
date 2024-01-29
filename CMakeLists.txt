cmake_minimum_required(VERSION 3.21)
 
project(opengl_demo)

set(CMAKE_CXX_STANDARD 17)
option(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

aux_source_directory(source demo_src)

add_executable(demo1 demo1.cpp ${demo_src})
target_include_directories(demo1 PRIVATE include/)
target_link_libraries(demo1 PRIVATE glfw glad dl)