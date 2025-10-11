cmake_minimum_required(VERSION 3.16)


#set(CMAKE_C_COMPILER "clang")
#set(CMAKE_CXX_COMPILER "clang++")

project(
    Grace 
    VERSION 1.0.0
    DESCRIPTION "High-performance RK4 ODE integrator in C++20 with Kokkos parallelization"
    LANGUAGES CXX
)
set(CPACK_PACKAGE_LICENSE "Apache-2.0")
set(PROJECT_LICENSE "Apache-2.0")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Kokkos REQUIRED)

message(STATUS [LOG] "Kokkos found: ${Kokkos_DIR}")



add_subdirectory(lib)
add_subdirectory(tests)
add_subdirectory(benchmarks)




