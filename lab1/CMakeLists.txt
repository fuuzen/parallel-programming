cmake_minimum_required(VERSION 3.10)
project(lab1)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE LIB_SOURCES "lib/*.cpp")

add_executable(test main.cpp ${LIB_SOURCES})

find_package(MPI REQUIRED)

target_link_libraries(test PUBLIC MPI::MPI_CXX)

target_compile_options(test PUBLIC -Ofast -DMETHOD1)
