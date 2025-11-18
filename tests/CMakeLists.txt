enable_testing()
include(FetchContent)



# Force static build for Catch2
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.11.0
)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)



function(add_test_executable test_name test_src)
    add_executable(${test_name} ${test_src})
    target_link_libraries(
        ${test_name}
        PRIVATE
            Catch2::Catch2WithMain
            Grace
            Grace_build_flags
    )
    
    # Use simple add_test instead of catch_discover_tests
    add_test(NAME ${test_name} COMMAND ${test_name})
    
    # Set PATH for test execution
    if(WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set_tests_properties(${test_name} PROPERTIES
            ENVIRONMENT "PATH=D:/Program Files/msys64/mingw64/bin;$ENV{PATH}")
    endif()
endfunction()

add_test_executable(catch2_test src/catch2_test.cpp)
add_test_executable(methods_convergence_testing src/Grace/integration/methods/convergence_testing.cpp)











# if(WIN32)
# 
#     message(STATUS "Grace: " "mettle installation: " "installing for Windows using WSL")
# 
#     set(METTLE_SRC_DIR "${CMAKE_BINARY_DIR}/mettle_src")
#     
#     # Install dependencies and build mettle in one WSL session
#     execute_process(
#         COMMAND wsl bash -c "
#             # Update and install dependencies
#             sudo apt update && sudo apt install -y python3-pip ninja-build git
#             
#             # Install bfg9000
#             pip3 install bfg9000 --user
#             
#             # Add user bin to PATH
#             export PATH=\$PATH:~/.local/bin
#             
#             # Clone and build mettle
#             git clone https://github.com/jimporter/mettle.git ${METTLE_SRC_DIR}
#             cd ${METTLE_SRC_DIR}
#             python3 -m bfg9000 configure build --vendorize
#             cd build
#             ninja install
#         "
#         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#         RESULT_VARIABLE BUILD_RESULT
#     )
# 
#     if(NOT BUILD_RESULT EQUAL 0)
#         message(STATUS "Grace: " "mettle installation: " "finished successfully")
#     else()
#         message(STATUS "Grace: " "mettle installation: " "failed")
#         message(FATAL_ERROR "Grace: " "mettle uninstalled, tests unavailable")
#     endif()
# else()
# 
# endif()




# # Detect if we're on Windows and use WSL
# if(WIN32)
# 
#     execute_process(
#       COMMAND wsl bash -c "pip3 install bfg9000 --user"
#       RESULT_VARIABLE INSTALL_RESULT
#     )
# 
#     if(NOT INSTALL_RESULT EQUAL 0)
#         message(FATAL_ERROR "Failed to install bfg9000 in WSL")
#     endif()
# 
#     # Build mettle using WSL
#     execute_process(
#         COMMAND wsl git clone https://github.com/jimporter/mettle.git ${CMAKE_BINARY_DIR}/mettle_src
#         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#     )
#     
#     execute_process(
#         COMMAND wsl cd ${CMAKE_BINARY_DIR}/mettle_src && 9k build --vendorize
#         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#     )
#     
#     execute_process(
#         COMMAND wsl cd ${CMAKE_BINARY_DIR}/mettle_src/build && ninja install
#         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#     )
#     
#     # Find the installed mettle
#     find_path(METTLE_INCLUDE_DIR mettle.hpp PATHS /usr/local/include)
#     find_library(METTLE_LIBRARY mettle PATHS /usr/local/lib)
# endif()




# ExternalProject_Add(
#   mettle
#     GIT_REPOSITORY https://github.com/jimporter/mettle.git
#     GIT_TAG master
#     PREFIX ${CMAKE_BINARY_DIR}/mettle
#     CONFIGURE_COMMAND pip install bfg9000 &&
#         cd <SOURCE_DIR> &&
#         9k <BINARY_DIR> --vendorize
#     BUILD_COMMAND cd <BINARY_DIR> && ninja install
#     BUILD_IN_SOURCE FALSE
#     INSTALL_COMMAND ""  # Installation is handled by ninja install
# )
# 














# 
# FetchContent_Declare(
#   googletest
#   GIT_REPOSITORY https://github.com/google/googletest.git
#   GIT_TAG v1.17.0
#   DOWNLOAD_EXTRACT_TIMESTAMP TRUE
# )
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
# 
# 
# set(CMAKE_CXX_COMPILER "${CMAKE_CXX_COMPILER}" CACHE FILEPATH "CXX compiler" FORCE)
# set(CMAKE_C_COMPILER "${CMAKE_C_COMPILER}" CACHE FILEPATH "C compiler" FORCE)
# 
# FetchContent_MakeAvailable(googletest)
# include(GoogleTest)
# 
# 
# 
# 
# function(register_test TEST_DIR TEST_NAME)
#   set(TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_DIR}/${TEST_NAME}.cpp)
#   set(TEST_EXEC ${TEST_NAME}) # TODO fix naming ambiguity
#   add_executable(${TEST_EXEC} ${TEST_PATH})
#   target_include_directories(
#     ${TEST_EXEC}
#     PRIVATE ${CMAKE_SOURCE_DIR}/lib/include 
#             include
#   )
#   target_link_libraries(
#     ${TEST_EXEC} 
#     PRIVATE Grace_build_flags 
#             Grace
#             GTest::gtest_main
#   )
#   add_test(NAME ${TEST_EXEC} COMMAND ${TEST_EXEC})
# endfunction()
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# set(
#   DEFAULT_TEST_EXTRA_ARGS
#   --gtest_color=yes
#   --gtest_print_time=0 
# )
# 
# 
# 
# 
# register_test(src/unit integrator_precision_tests)
# register_test(include/testing/unit/Grace/integration/methods MethodConvergenceTests)
# register_test(include/testing/unit/Grace/integration/methods MinimalTest)
# 

function(add_grace_test_executable test_name test_src)
    add_executable(${test_name} ${test_src})
    target_link_libraries(
        ${test_name}
        PRIVATE
            Grace
            Catch2::Catch2WithMain
    )
    
    if(WIN32)
        set(TEST_PATH "$ENV{PATH}")
        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            set(TEST_PATH "D:/Program Files/msys64/mingw64/bin;${TEST_PATH}")
        endif()
        
        catch_discover_tests(${test_name}
            PROPERTIES ENVIRONMENT "PATH=${TEST_PATH}"
        )
    else()
        catch_discover_tests(${test_name})
    endif()
endfunction()