cmake_minimum_required(VERSION 3.10)
project(lab9)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CUDA_ARCHITECTURES 52)  # sm_52 for 1050
enable_language(CUDA)

file(GLOB_RECURSE TASK1_SOURCES "task1/*.cu")
add_executable(task1 ${TASK1_SOURCES})
target_include_directories(task1 PRIVATE ${PROJECT_SOURCE_DIR}/task1)
target_compile_options(task1 PUBLIC -O3)

file(GLOB_RECURSE TASK2_SOURCES "task2/*.cu")
add_executable(task2 ${TASK2_SOURCES})
target_include_directories(task2 PRIVATE ${PROJECT_SOURCE_DIR}/task2)
target_compile_options(task2 PUBLIC -O3)