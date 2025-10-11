include(FetchContent)


set(BUILD_BENCHMARKS_RELEASE ON CACHE BOOL "Build benchmarks in Release mode")

if(BUILD_BENCHMARKS_RELEASE)
    set(OLD_CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE})
    
    set(CMAKE_BUILD_TYPE Release)
endif()

FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.9.4
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark tests")
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Disable installation")
set(BENCHMARK_ENABLE_EXCEPTIONS ON CACHE BOOL "Enable exceptions")
set(BENCHMARK_USE_BUNDLED_GTEST OFF CACHE BOOL "Use external GTest if needed")
#set(BENCHMARK_USE_LIBCXX OFF)          # Don't use libc++ (usually for Clang)
set(BENCHMARK_BUILD_32_BITS OFF)       # Build 64-bit version
FetchContent_MakeAvailable(googlebenchmark)


if(BUILD_BENCHMARKS_RELEASE)
    set(CMAKE_BUILD_TYPE ${OLD_CMAKE_BUILD_TYPE})
endif()





set(BENCHMARK_NAME first_benchmark)

add_executable(${BENCHMARK_NAME} ${BENCHMARK_NAME}/main.cpp)
target_link_libraries(${BENCHMARK_NAME} PRIVATE Grace benchmark::benchmark benchmark::benchmark_main)
target_include_directories(
    ${BENCHMARK_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/include 
)
