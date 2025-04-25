cmake_minimum_required(VERSION 3.10)
project(lab6)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP REQUIRED)

file(GLOB_RECURSE UTIL_SOURCES "lib/*.cpp")
file(GLOB_RECURSE TEST_SOURCES "src/*.cpp")
add_executable(heated_plate ${UTIL_SOURCES} ${TEST_SOURCES})
target_include_directories(heated_plate PRIVATE ${PROJECT_SOURCE_DIR}/lib)
target_compile_options(heated_plate PUBLIC -O0)


file(GLOB_RECURSE OMP_VER_SOURCES "omp_ver/*")
add_executable(omp_ver ${OMP_VER_SOURCES})
target_compile_options(omp_ver PUBLIC -Ofast)
target_link_libraries(omp_ver PRIVATE OpenMP::OpenMP_CXX)
