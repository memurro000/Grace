cmake_minimum_required(VERSION 3.16)



project(
    Grace
    VERSION 1.0.2
    DESCRIPTION     "High-performance extensible library for ODE integration \
                    in C++20 with Kokkos parallelization"
    LANGUAGES CXX
)
set(CPACK_PACKAGE_LICENSE "Apache-2.0")
set(PROJECT_LICENSE "Apache-2.0")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DEFAULT_BUILD_TYPE "RelWithDebInfo")


option(ENABLE_CXX_OPTIONS_SAN "Use sanitizer compile and link options \
    (requires working sanitizer support)" OFF)
option(ENABLE_CXX_OPTIONS_NOLIB_UB_SAN "Enable UB sanitizer without library dependencies - \
    provides detection only producing crash without pretty diagnostics" OFF)
if(ENABLE_CXX_OPTIONS_SAN AND ENABLE_CXX_OPTIONS_NOLIB_UB_SAN)
    message(
        WARNING "Grace: "
        "Both ENABLE_CXX_OPTIONS_SAN and ENABLE_CXX_OPTIONS_NOLIB_UB_SAN are enabled. This may cause conflicts."
    )
endif()

# Disable in-source builds to prevent source tree corruption.
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(
    FATAL_ERROR
      "Grace FATAL: In-source builds are not allowed. \
      You should create a separate directory for build files and delete CMakeCache.txt and CMakeFiles/."
  )
endif()

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Grace: " "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}"
        CACHE STRING "Choose the type of build, \
        options are: Debug, Release, RelWithDebInfo and MinSizeRel."
        FORCE
    )
else()
    message(STATUS "Grace: build type is " ${CMAKE_BUILD_TYPE})
endif()




# clang-tidy is automatically executed
# and reported with message if not accessible
find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY}")
else()
    message(STATUS "Grace: " "clang-tidy not found and therefore not used")
endif()

# clang-format is executed (using .clang-format file)
# with following custom targets:
# check-format, format and show-format
# and reported with message if not accessible
find_program(CLANG_FORMAT clang-format)
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
    message(STATUS "Grace: " "clang-format not found and therefore not used")
endif()



set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(Kokkos REQUIRED)
message(STATUS [LOG] "Kokkos found: ${Kokkos_DIR}")
message(STATUS "Kokkos devices: ${Kokkos_DEVICES}")
message(STATUS "Kokkos options: ${Kokkos_OPTIONS}")


include(GraceBuildFlagsIL.cmake)

add_subdirectory(lib)
if(TRUE)
add_subdirectory(tests)
endif()
if(FALSE)
    add_subdirectory(benchmarks)
endif()
add_subdirectory(apps)


