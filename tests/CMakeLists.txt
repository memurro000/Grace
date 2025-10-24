enable_testing()
include(FetchContent)



FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)


set(CMAKE_CXX_COMPILER "${CMAKE_CXX_COMPILER}" CACHE FILEPATH "CXX compiler" FORCE)
set(CMAKE_C_COMPILER "${CMAKE_C_COMPILER}" CACHE FILEPATH "C compiler" FORCE)

FetchContent_MakeAvailable(googletest)
include(GoogleTest)




function(register_test TEST_DIR TEST_NAME)
  set(TEST_PATH src/${TEST_DIR}/${TEST_NAME}.cpp)
  set(TEST_EXEC ${TEST_DIR}_${TEST_NAME})
  add_executable(${TEST_EXEC} ${TEST_PATH})
#  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    target_compile_options(
#        ${TEST_EXEC} 
#        PRIVATE
#            -fno-omit-frame-pointer
#            -fsanitize=address,undefined
#            $<$<NOT:$<PLATFORM_ID:Windows>>:-fsanitize=leak>
#    )
#    target_link_options(
#        ${TEST_EXEC} 
#        PRIVATE
#            -fsanitize=address,undefined
#            $<$<NOT:$<PLATFORM_ID:Windows>>:-fsanitize=leak>
#    )
#  endif()
  target_include_directories(
    ${TEST_EXEC}
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/include 
            include
  )
  target_link_libraries(${TEST_EXEC} PRIVATE Grace GTest::gtest_main)
  gtest_discover_tests(${TEST_EXEC})
endfunction()











set(
  DEFAULT_TEST_EXTRA_ARGS
  --gtest_color=yes
  --gtest_print_time=0 
)




#register_test(unit integrator_basic_tests)
register_test(unit integrator_precision_tests)
