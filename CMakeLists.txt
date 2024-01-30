cmake_minimum_required(VERSION 3.21)
 
project(opengl_demo)

set(CMAKE_CXX_STANDARD 17)
option(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

macro(gen_executable target target_src)
	add_executable(${target} ${target}.cpp ${target_src})
	target_include_directories(${target} PRIVATE include/)
	target_link_libraries(${target} PRIVATE glfw glad dl)
endmacro()

aux_source_directory(source target_src)

gen_executable(demo1 ${target_src})
gen_executable(demo2 ${target_src})