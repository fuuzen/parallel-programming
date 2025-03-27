cmake_minimum_required(VERSION 3.10)
project(lab1)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE LIB_SOURCES "lib/gemm.cpp" "lib/input.cpp" "lib/MartixParams.cpp")

find_package(MPI REQUIRED)
find_package(MKL REQUIRED)

add_executable(test1 main.cpp ${LIB_SOURCES})
target_include_directories(test1 PRIVATE ./lib)
target_link_libraries(test1 PUBLIC MPI::MPI_CXX)
target_compile_options(test1 PUBLIC -O0 -DMETHOD1 -DINTERACTIVE)

add_executable(test2 main.cpp ${LIB_SOURCES} lib/mkl_gemm.cpp)
target_include_directories(test2 PRIVATE ./lib)
target_link_libraries(test2 PUBLIC MPI::MPI_CXX MKL::MKL)
target_compile_options(test2 PUBLIC -O0 -DMETHOD2 -DINTERACTIVE)

add_executable(test1-all main.cpp ${LIB_SOURCES})
target_include_directories(test1-all PRIVATE ./lib)
target_link_libraries(test1-all PUBLIC MPI::MPI_CXX)
target_compile_options(test1-all PUBLIC -O0 -DMETHOD1)

add_executable(test2-all main.cpp ${LIB_SOURCES} lib/mkl_gemm.cpp)
target_include_directories(test2-all PRIVATE ./lib)
target_link_libraries(test2-all PUBLIC MPI::MPI_CXX MKL::MKL)
target_compile_options(test2-all PUBLIC -O0 -DMETHOD2)