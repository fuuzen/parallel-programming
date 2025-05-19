cmake_minimum_required(VERSION 3.10)
project(lab7)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP REQUIRED)

add_executable(task1 task1/main.cpp)
target_include_directories(task1 PRIVATE ${PROJECT_SOURCE_DIR}/task1)
target_compile_options(task1 PUBLIC -Ofast)
target_link_libraries(task1 PRIVATE OpenMP::OpenMP_CXX)
