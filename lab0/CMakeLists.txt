cmake_minimum_required(VERSION 3.10)
project(lab0)
set(CMAKE_CXX_STANDARD 20)

# test1 : naive
add_executable(test1 gemm.h gemm.cpp methods/method0.cpp)
target_compile_options(test1 PRIVATE -O0 -DMETHOD0)

# test2 : 调整循环顺序
add_executable(test2 gemm.h gemm.cpp methods/method1.cpp)
target_compile_options(test2 PRIVATE -O0 -DMETHOD1)

# test3 : 循环展开
add_executable(test3 gemm.h gemm.cpp methods/method2.cpp)
target_compile_options(test3 PRIVATE -O0 -DMETHOD2)

# test4 : 编译优化
add_executable(test4 gemm.h gemm.cpp methods/method0.cpp)
target_compile_options(test4 PRIVATE -Ofast -DMETHOD0)

# test5 : Intel MKL
add_executable(test5 gemm.h gemm.cpp methods/method3.cpp)
target_compile_options(test5 PRIVATE -DMETHOD3)
find_package(MKL REQUIRED)
target_link_libraries(test5 PRIVATE MKL::MKL)

