set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(Grace STATIC src/Grace.cpp)

target_include_directories(
    Grace
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Grace PUBLIC Kokkos::kokkos)