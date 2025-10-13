#ifndef TESTS_GENERATORS_HPP
#define TESTS_GENERATORS_HPP

#include <Grace/defaults.hpp>


namespace lib_testing::generators::indexed
{

    using namespace Grace::defaults;

    num_t index(size_t i) {
        return static_cast<num_t>(i);
    }



} // namespace lib_testing::generators::indexed






#endif // TESTS_GENERATORS_HPP