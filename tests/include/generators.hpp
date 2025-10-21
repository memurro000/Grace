#ifndef TESTS_GENERATORS_HPP
#define TESTS_GENERATORS_HPP

#include <Grace/defaults.hpp>


#include <functional>

namespace lib_testing::generators::indexed
{

    using Grace::defaults::num_t;

    using generator_function_t = std::function<num_t (size_t)>;



    inline
    num_t zero(size_t i) {
        return num_t{0};
    }

    inline
    num_t index(size_t i) {
        return i;
    }

    inline
    num_t index_step2(size_t i) {
        if (i % 2 == 0)
            return i;
        else
            return -i;
    }



    inline
    generator_function_t shifted(const generator_function_t& generator, num_t shift) {
        return [&](size_t i) {
            return shift + generator(i);
        };
    }


    inline
    generator_function_t constant(num_t value) {
        return [value](size_t i) { return value; };
    }

    inline
    generator_function_t linear(num_t step) {
        return [step](size_t i) { return step * i; };
    }







} // namespace lib_testing::generators::indexed






#endif // TESTS_GENERATORS_HPP