cmake_minimum_required(VERSION 3.10)
project(lab5)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP REQUIRED)

file(GLOB_RECURSE UTIL_SOURCES "util/*.cpp")
file(GLOB_RECURSE TEST1_SOURCES "test1/*.cpp")
file(GLOB_RECURSE TEST2_SOURCES "test2/*.cpp")

add_executable(test1 ${UTIL_SOURCES} ${TEST1_SOURCES})
add_executable(test2 ${UTIL_SOURCES} ${TEST2_SOURCES})

target_compile_options(test1 PUBLIC -Ofast)
target_compile_options(test2 PUBLIC -Ofast)

target_link_libraries(test1 PRIVATE OpenMP::OpenMP_CXX)