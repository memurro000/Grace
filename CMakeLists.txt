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
include(cmake/configurations/License.cmake)
include(cmake/configurations/CXX.cmake)
include(cmake/configurations/BuildType.cmake)
include(cmake/configurations/ComponentBuilding.cmake)
include(cmake/configurations/CXXOptionsEnabling.cmake)
include(cmake/configurations/CompileCommandsExporting.cmake)


# Validation
# (build validation and termination)

include(cmake/validation/BuildDirectory.cmake)

# Tools
# (Configuration tools for the project)

include(cmake/tools/ClangTools.cmake)
include(cmake/tools/GraceBuildFlagsIL.cmake)




include(cmake/package_enabling/Kokkos.cmake)


add_subdirectory(lib)

if(GRACE_BUILD_TESTS)
    add_subdirectory(tests)
endif()
if(GRACE_BUILD_BENCHMARKS AND OFF) # TODO create benchmarks
    add_subdirectory(benchmarks)
endif()
if(GRACE_BUILD_APPS)
    add_subdirectory(apps)
endif()
