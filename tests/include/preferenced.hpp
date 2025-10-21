#ifndef TESTS_PREFERENCED_HPP
#define TESTS_PREFERENCED_HPP

#include <gtest/gtest.h>

#include <Grace/Grace.hpp>
#include <Grace/runtime.hpp>


namespace tests::preferenced
{
    namespace defaults {
        constexpr bool FLAGS_PRINT_TIME = false;
        constexpr const char* FLAGS_COLOR = "yes";
    } // namespace defaults

    inline
    void set_flags(
        bool print_time_flag = defaults::FLAGS_PRINT_TIME, 
        const char* color_flag = defaults::FLAGS_COLOR
    ) {
        testing::GTEST_FLAG(print_time) = print_time_flag;
        testing::GTEST_FLAG(color) = color_flag;
    }
    
} // namespace tests::preferenced


#define TESTS_PREFERENCED_MAIN()                    \
    int main(int argc, char* argv[]) {              \
        Grace::runtime::initialize(argc, argv);     \
        tests::preferenced::set_flags();            \
        testing::InitGoogleTest(&argc, argv);       \
        int test_result = RUN_ALL_TESTS();          \
        return test_result;                         \
        Grace::runtime::finalize();                 \
    }



#endif // TESTS_PREFERENCED_HPP