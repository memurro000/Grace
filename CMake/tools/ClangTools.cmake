

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
