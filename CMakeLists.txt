cmake_minimum_required(VERSION 3.16)


project(
    Grace
    VERSION 1.0.2
    DESCRIPTION     "High-performance extensible library for ODE integration \
                    in C++20 with Kokkos parallelization"
    LANGUAGES CXX
)


# Configurations
# (options, variables)
include(CMake/configurations/License.cmake)
include(CMake/configurations/CXX.cmake)
include(CMake/configurations/BuildType.cmake)
include(CMake/configurations/ComponentBuilding.cmake)
include(CMake/configurations/CXXOptionsEnabling.cmake)
include(CMake/configurations/CompileCommandsExporting.cmake)

# Validation
# (build validation and termination)

include(CMake/validation/BuildDirectory.cmake)

# Tools
# (Configuration tools for the project)

include(CMake/tools/ClangTools.cmake)
include(CMake/tools/GraceBuildFlagsIL.cmake)




include(CMake/package_enabling/Kokkos.cmake)


add_subdirectory(lib)

if(GRACE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
if(GRACE_BUILD_BENCHMARKS AND OFF) # TODO create benchmarks
    add_subdirectory(benchmarks)
endif()
if(GRACE_BUILD_APPS)
    add_subdirectory(apps)
endif()
