#ifndef TESTS_PREFERENCED_HPP
#define TESTS_PREFERENCED_HPP

#include <gtest/gtest.h>

#include <Grace/Grace.hpp>


namespace tests::preferenced
{
    namespace defaults {
        constexpr bool FLAGS_PRINT_TIME = false;
        constexpr const char* FLAGS_COLOR = "yes";
    } // namespace defaults

    void set_flags(
        bool print_time_flag = defaults::FLAGS_PRINT_TIME, 
        const char* color_flag = defaults::FLAGS_COLOR
    ) {
        testing::GTEST_FLAG(print_time) = print_time_flag;
        testing::GTEST_FLAG(color) = color_flag;
    }
    
} // namespace tests::preferenced


#define TESTS_PREFERENCED_MAIN()                \
    int main(int argc, char* argv[]) {           \
        Grace::initialize(argc, argv);                      \
        testing::InitGoogleTest(&argc, argv);   \
        tests::preferenced::set_flags();        \
        return RUN_ALL_TESTS();                 \
        Grace::finalize();                        \
    }



#endif // TESTS_PREFERENCED_HPP