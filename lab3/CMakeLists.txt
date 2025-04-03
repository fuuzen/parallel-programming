cmake_minimum_required(VERSION 3.10)
project(lab1)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE TEST1_SOURCES "main1.cpp" "lib/gemm_phread.cpp" "lib/input.cpp")

add_executable(test1 ${TEST1_SOURCES})
target_include_directories(test1 PRIVATE ./lib)
target_compile_options(test1 PUBLIC -Ofast -DINTERACTIVE)

add_executable(test1-all ${TEST1_SOURCES})
target_include_directories(test1-all PRIVATE ./lib)
target_compile_options(test1-all PUBLIC -Ofast)

# file(GLOB_RECURSE TEST2_SOURCES "main2.cpp" "lib/input.cpp")

# add_executable(test2 main2.cpp ${LIB_SOURCES})
# target_include_directories(test2 PRIVATE ./lib)
# target_compile_options(test2 PUBLIC -Ofast -DINTERACTIVE)

# add_executable(test2-all main2.cpp ${LIB_SOURCES})
# target_include_directories(test2-all PRIVATE ./lib)
# target_compile_options(test2-all PUBLIC -Ofast)
