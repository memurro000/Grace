#ifndef TESTS_FACTORIES_HPP
#define TESTS_FACTORIES_HPP
#include <Grace/defaults.hpp>

namespace lib_testing::vectors
{
    using Grace::defaults::num_t;
    using Grace::defaults::vector_t;
    
    void fill(vector_t& vector, num_t with) {
        Kokkos::parallel_for(
            "zero_system_for1", vector.extent(0),
            KOKKOS_LAMBDA(const size_t i) {
                vector(i) = with;
            });
    }

    vector_t make(const std::string& info, size_t n, num_t filling = num_t{0}) {
        vector_t done(info + " make_vector_done", n);
        fill(done, filling);
        return done;
    }

} // namespace lib_testing::vectors


namespace lib_testing::functions {

    using namespace Grace::defaults;

    using function_system_basic_ptr_t = vector_t(*)(num_t, const vector_t&);


    vector_t zero_system(num_t t, const vector_t& y) {
        return vectors::make("zero_system", y.extent(0));
    }




    class constant_system {
    public:
        constant_system(num_t value) : _filling(value) {}


        vector_t operator()(num_t t, const vector_t& y) {
            return vectors::make("constant_system", y.extent(0), _filling);
        }

    private:
        num_t _filling;
    };

    
} // namespace testing::functions





#endif // TESTS_FACTORIES_HPP