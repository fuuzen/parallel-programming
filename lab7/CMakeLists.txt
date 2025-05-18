cmake_minimum_required(VERSION 3.10)
project(lab7)
set(CMAKE_CXX_STANDARD 20)

find_package(MPI REQUIRED)

file(GLOB_RECURSE TASK1_FFT_SOURCES "task1/fft/*.cpp")
file(GLOB_RECURSE TASK2_TIME_SOURCES "task1/time/*.cpp")

add_executable( task1
  task1/main.cpp
  ${TASK1_FFT_SOURCES}
  ${TASK2_TIME_SOURCES}
)
target_include_directories(task1 PRIVATE ${PROJECT_SOURCE_DIR}/task1)
target_compile_options(task1 PUBLIC -Ofast)
target_link_libraries(task1 PRIVATE MPI::MPI_CXX)


file(GLOB_RECURSE UTIL_SOURCES "task2/lib/*.cpp")
add_executable( task2
  task2/main.cpp
  ${UTIL_SOURCES}
)
target_include_directories(task2 PRIVATE ${PROJECT_SOURCE_DIR}/task2)
target_compile_options(task2 PUBLIC -Ofast)