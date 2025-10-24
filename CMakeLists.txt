cmake_minimum_required(VERSION 3.16)



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


if(TRUE)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

    find_program(CLANG_FORMAT "clang-format")
    if(CLANG_FORMAT)
        file(
            GLOB_RECURSE ALL_SOURCE_FILES
            "lib/*.cpp" "lib/*.hpp" "lib/*.h"
            "apps/*.cpp" "apps/*.hpp" "apps/*.h" 
            "tests/*.cpp" "tests/*.hpp" "tests/*.h"
        )

        add_custom_target(
            check-format
            COMMAND ${CLANG_FORMAT} --dry-run --Werror -style=file ${ALL_SOURCE_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Checking code formatting"
        )
        
        add_custom_target(
            format
            COMMAND ${CLANG_FORMAT} -i -style=file ${ALL_SOURCE_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Formatting all source files"
        )
        
        add_custom_target(
            show-format
            COMMAND ${CLANG_FORMAT} --output-replacements-xml -style=file ${ALL_SOURCE_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Showing formatting differences"
        )
    else()
        message(WARNING "clang-format not found!")
    endif()
endif()



set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(Kokkos REQUIRED)
message(STATUS [LOG] "Kokkos found: ${Kokkos_DIR}")
message(STATUS "Kokkos devices: ${Kokkos_DEVICES}")
message(STATUS "Kokkos options: ${Kokkos_OPTIONS}")



add_subdirectory(lib)
if(TRUE)
add_subdirectory(tests)
endif()
if(FALSE)
    add_subdirectory(benchmarks)
endif()
add_subdirectory(apps)


