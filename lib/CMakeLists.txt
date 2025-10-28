add_library(Grace STATIC src/Grace.cpp)

message(STATUS "Grace: build type is " ${CMAKE_BUILD_TYPE})

message(STATUS "Grace: WARNING - Build flag system is incomplete and subject to change")
message(STATUS "Grace: - Using PUBLIC visibility for compiler flags")
message(STATUS "Grace: - Warnings and sanitizers will propagate to dependent targets")
message(STATUS "Grace: - Use PRIVATE for target-specific optimization flags")

target_compile_options(
    Grace
    PUBLIC  # TODO reconsider
        # GNU, Clang, AppleClang
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleCLang>:
            # Common
                # Warnings
                -Wall
                -Wextra
                -Wpedantic
                -Wshadow
                -Wnon-virtual-dtor
                # -Wnull-dereference # TODO reconsider 
                # -Wdouble-promotion # TODO reconsider

                # Version-specific warnings
                # $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,8>:-Wduplicated-cond>     # TODO reconsider 
                # $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,6>:-Wduplicated-branches> # TODO reconsider 

            # Debug
            $<$<CONFIG:Debug>:
                -O0
                -g3
                -D_GLIBCXX_DEBUG
                -D_GLIBCXX_ASSERTIONS
                -fno-inline
                -fno-omit-frame-pointer
                -fstack-protector-strong
                -fstack-clash-protection 
                # Sanitizer
                -fno-sanitize-recover=undefined
                -fsanitize-address-use-after-scope
                -fsanitize=address,undefined
                $<$<NOT:$<PLATFORM_ID:Windows,Darwin>>:
                    -fsanitize=leak
                >
            >

            # Release
            $<$<CONFIG:Release>:
                -O3
                -DNDEBUG
                
                # Performance
                -funroll-loops
                -fomit-frame-pointer
                -flto  # Link Time Optimization
                -march=native
                -mtune=native
                
                # TODO benchmark
                # Security
                -D_FORTIFY_SOURCE=2
                -fstack-protector-strong
            >

            # RelWithDebInfo
            $<$<CONFIG:RelWithDebInfo>:
                -O2
                -g
                -DNDEBUG
                -funroll-loops
                -fomit-frame-pointer
                -flto
            >

            # MinSizeRel
            $<$<CONFIG:MinSizeRel>:
                -Os
                -DNDEBUG
                -fomit-frame-pointer
                -flto
            >

        >

        # MSVC
        $<$<CXX_COMPILER_ID:MSVC>:
            # Common
            /W4
            /permissive-

            # Debug
            $<$<CONFIG:Debug>:
                /Od
                /Zi
                /RTC1
                /MDd
            >

            # Release
            $<$<CONFIG:Release>:
                /O2
                /Ob2
                /Gy
                /DNDEBUG
                /GL
                /MD
            >

            # RelWithDebInfo
            $<$<CONFIG:RelWithDebInfo>:
                /O2
                /Ob1
                /Zi
                /DNDEBUG
                /MD
            >

        >

)
target_link_options(
    Grace
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:
            $<$<CONFIG:Debug>:
                -fno-sanitize-recover=undefined
                -fsanitize-address-use-after-scope  
                -fsanitize=address,undefined
                $<$<NOT:$<PLATFORM_ID:Windows,Darwin>>:
                    -fsanitize=leak
                >
            >
        >
)
#endif()

target_include_directories(
    Grace
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
    Grace 
    PUBLIC 
        Kokkos::kokkos 
)


install(
    TARGETS Grace
    EXPORT GraceTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
install(DIRECTORY include/ DESTINATION include)
install(
    EXPORT GraceTargets
    FILE GraceTargets.cmake
    NAMESPACE Grace::
    DESTINATION lib/cmake/Grace
)






include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/GraceConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/GraceConfig.cmake"
    INSTALL_DESTINATION lib/cmake/Grace
)


write_basic_package_version_file(
    "GraceConfigVersion.cmake"
    VERSION ${Grace_VERSION}
    COMPATIBILITY AnyNewerVersion
)
install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/GraceConfigVersion.cmake" 
    "${CMAKE_CURRENT_BINARY_DIR}/GraceConfig.cmake" 
    DESTINATION lib/cmake/Grace
)