cmake_minimum_required(VERSION 3.10)
project(lab1)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE LIB_SOURCES "lib/*.cpp")

find_package(MPI REQUIRED)
find_package(MKL REQUIRED)

add_executable(test1 main.cpp ${LIB_SOURCES})
target_include_directories(test1 PRIVATE ./lib)
target_link_libraries(test1 PUBLIC MPI::MPI_CXX MKL::MKL)
target_compile_options(test1 PUBLIC -Ofast -DMETHOD1)

add_executable(test2 main.cpp ${LIB_SOURCES})
target_include_directories(test2 PRIVATE ./lib)
target_link_libraries(test2 PUBLIC MPI::MPI_CXX MKL::MKL)
target_compile_options(test2 PUBLIC -Ofast -DMETHOD2)