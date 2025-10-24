add_library(Grace STATIC src/Grace.cpp)

message(STATUS [LOG] ${CMAKE_BUILD_TYPE})
#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#target_compile_options(
#    Grace
#    PRIVATE
#        -fno-omit-frame-pointer
#        -fsanitize=address,undefined
#        $<$<NOT:$<PLATFORM_ID:Windows>>:-fsanitize=leak>
#)
#target_link_options(
#    Grace
#    PRIVATE
#        -fsanitize=address,undefined
#        $<$<NOT:$<PLATFORM_ID:Windows>>:-fsanitize=leak>
#)
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