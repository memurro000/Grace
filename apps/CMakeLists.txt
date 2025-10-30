


add_executable(quick_start quick_start/main.cpp)
target_link_libraries(
    quick_start
    PRIVATE Grace_build_flags
            Grace
)