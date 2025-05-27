cmake_minimum_required(VERSION 3.10)
project(lab10)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CUDA_ARCHITECTURES 52)  # sm_52 for 1050
enable_language(CUDA)

file(GLOB_RECURSE SOURCES "src/*.cu")

find_package(OpenMP REQUIRED)

add_executable(test ${SOURCES})
target_include_directories(test PRIVATE ${PROJECT_SOURCE_DIR}/common)
target_compile_options(test PUBLIC
    -O3  # nvcc can't use Ofast
    -Xcompiler=-fopenmp  # for OpenMP
)
target_link_libraries(test
    -lcublas  # link CUBLAS
    OpenMP::OpenMP_CXX  # link OpenMP
)  
