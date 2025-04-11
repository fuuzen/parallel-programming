cmake_minimum_required(VERSION 3.10)
project(lab3)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE UTIL_SOURCES "util/*.cpp")
file(GLOB_RECURSE TEST1_SOURCES "test1/*.cpp")
file(GLOB_RECURSE TEST2_SOURCES "test2/*.cpp")
file(GLOB_RECURSE TEST3_SOURCES "test3/*.cpp")
file(GLOB_RECURSE TEST4_SOURCES "test4/*.cpp")

add_executable(test1 ${UTIL_SOURCES} ${TEST1_SOURCES})
add_executable(test2 ${UTIL_SOURCES} ${TEST2_SOURCES})
add_executable(test3 ${UTIL_SOURCES} ${TEST3_SOURCES})
add_executable(test4 ${UTIL_SOURCES} ${TEST4_SOURCES})

target_compile_options(test1 PUBLIC -Ofast -DINTERACTIVE)
target_compile_options(test2 PUBLIC -Ofast -DINTERACTIVE)
target_compile_options(test3 PUBLIC -Ofast -DINTERACTIVE)
target_compile_options(test4 PUBLIC -Ofast -DINTERACTIVE)
