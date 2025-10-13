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
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(CMAKE_C_COMPILER "clang.exe")
#set(CMAKE_CXX_COMPILER "clang++.exe")

#find_package(OpenMP REQUIRED)
#if(OpenMP_CXX_FOUND)
#    message(STATUS [LOG] "OpenMP found")
#    
#    # Для Windows с Clang
#    if(WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#        #target_link_libraries(your_target_name PRIVATE -lomp -lmingw32 -lmingwex)
#
#    else()
#    endif()
#endif()

find_package(Kokkos REQUIRED)
message(STATUS [LOG] "Kokkos found: ${Kokkos_DIR}")
message(STATUS "Kokkos devices: ${Kokkos_DEVICES}")
message(STATUS "Kokkos options: ${Kokkos_OPTIONS}")



add_subdirectory(lib)
add_subdirectory(tests)
if(FALSE)
    add_subdirectory(benchmarks)
endif()



