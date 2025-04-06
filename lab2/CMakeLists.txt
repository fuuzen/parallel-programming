cmake_minimum_required(VERSION 3.10)
project(lab1)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE LIB_SOURCES "lib/gemm.cpp" "lib/input.cpp" "lib/MartixParams.cpp" "verify.cpp")

find_package(MPI REQUIRED)
find_package(MKL REQUIRED)

add_executable(test main.cpp ${LIB_SOURCES})
target_include_directories(test PRIVATE ./lib)
target_link_libraries(test PUBLIC MPI::MPI_CXX)
target_compile_options(test PUBLIC -Ofast -DMETHOD1)

add_executable(cannon cannon.cpp ${LIB_SOURCES})
target_include_directories(cannon PRIVATE ./lib)
target_link_libraries(cannon PUBLIC MPI::MPI_CXX)
target_compile_options(cannon PUBLIC -Ofast)

add_executable(test-mkl main.cpp ${LIB_SOURCES} lib/mkl_gemm.cpp)
target_include_directories(test-mkl PRIVATE ./lib)
target_link_libraries(test-mkl PUBLIC MPI::MPI_CXX MKL::MKL)
target_compile_options(test-mkl PUBLIC -Ofast -DMETHOD2)

add_executable(cannon-verify cannon.cpp ${LIB_SOURCES})
target_include_directories(cannon-verify PRIVATE ./lib)
target_link_libraries(cannon-verify PUBLIC MPI::MPI_CXX)
target_compile_options(cannon-verify PUBLIC -Ofast -DVERIFY)