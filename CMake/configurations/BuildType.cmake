
set(GRACE_DEFAULT_BUILD_TYPE "Release")

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Grace: " "Setting build type to '${GRACE_DEFAULT_BUILD_TYPE}' as none was specified.")
    set(
        CMAKE_BUILD_TYPE "${GRACE_DEFAULT_BUILD_TYPE}"
        CACHE STRING "Choose the type of build, \
        options are: Debug, Release, RelWithDebInfo and MinSizeRel."
        FORCE
    )
else()
    message(STATUS "Grace: build type is " ${CMAKE_BUILD_TYPE})
endif()
