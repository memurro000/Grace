#ifndef TESTS_GENERATORS_HPP
#define TESTS_GENERATORS_HPP

#include <Grace/defaults.hpp>


#include <random>
#include <map>

namespace lib_testing::generators::indexed
{

    using namespace Grace::defaults;

    num_t zero(size_t i) {
        return num_t{0};
    }

    num_t index(size_t i) {
        return i;
    }


    num_t step2(size_t i) {
        if (i % 2 == 0)
            return i;
        else
            return -i;
    }






} // namespace lib_testing::generators::indexed






#endif // TESTS_GENERATORS_HPP