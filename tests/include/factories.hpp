#ifndef TESTS_FACTORIES_HPP
#define TESTS_FACTORIES_HPP
#include <Grace/defaults.hpp>

namespace lib_testing::vectors
{
    using Grace::defaults::num_t;
    using Grace::defaults::vector_t;
    
    inline
    void fill(vector_t& vector, num_t with) {
        Kokkos::parallel_for(
            "zero_system_for1", vector.extent(0),
            KOKKOS_LAMBDA(const size_t i) {
                vector(i) = with;
            });
    }

    inline
    vector_t make(const std::string& info, size_t n, num_t filling = num_t{0}) {
        vector_t done(info + " make_vector_done", n);
        fill(done, filling);
        return done;
    }

} // namespace lib_testing::vectors


namespace lib_testing::functions {

    using namespace Grace::defaults;

    using function_system_basic_ptr_t = vector_t(*)(num_t, const vector_t&);


    inline
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
    static_assert(function_system<constant_system>, 
        "constant_system must fulfill function_system");





    class logistic_system {
    public:
        logistic_system(num_t alpha, num_t carrying_capacity) 
            : _alpha(alpha), _K(carrying_capacity) 
        {
            assert(alpha >= 0.0);
            assert(carrying_capacity > 0.0);
            assert(std::isfinite(alpha));
            assert(std::isfinite(carrying_capacity));
        }

        vector_t operator()(num_t t, const vector_t& y) {
            num_t alpha = _alpha;
            num_t K = _K;
            vector_t result("logistic_derivative", y.extent(0));
            
            Kokkos::parallel_for("logistic_derivative_compute", y.extent(0), 
                KOKKOS_LAMBDA(const int i) {
                    result(i) = alpha * y(i) * (1.0 - y(i) / K);
                }
            );
            
            return result;
        }

    private:
        num_t _alpha;
        num_t _K;
    };
    static_assert(function_system<logistic_system>, 
        "logistic_system must fulfill function_system concept");



    template <typename Callable>
    concept analytical_solution = requires(
        Callable callable, vector_t y_0, num_t t_0, num_t t_end
    ) {
        { callable(y_0, t_0, t_end) } -> std::convertible_to<vector_t>;
    };


    inline
    vector_t zero_system_analytical(const vector_t& y_0, num_t t_0, num_t t_end) {
        vector_t result("zero_system_analytical_result", y_0.extent(0));
        Kokkos::parallel_for("zero_system_analytical_parallel_for", y_0.extent(0),
            KOKKOS_LAMBDA(const size_t i) {
                result(i) = y_0(i);
            }
        );
        return result;
    }
    static_assert(analytical_solution<decltype(zero_system_analytical)>);



    class constant_system_analytical {
    public:
        constant_system_analytical(num_t value) : _filling{value} {}


        vector_t operator()(const vector_t& y_0, num_t t_0, num_t t_end) {
            const num_t filling = _filling;
            vector_t result("constant_system_analytical_result", y_0.extent(0));
            Kokkos::parallel_for(
                "constant_system_analytical", y_0.extent(0),
                KOKKOS_LAMBDA(const size_t i) {
                    result(i) = y_0(i) + filling * (t_end - t_0);
                }
            );
            return result;
        }

    private:
        num_t _filling;
    };
    static_assert(analytical_solution<constant_system_analytical>, 
        "constant_system_analytical must fulfill analytical_solution concept");




    class logistic_system_analytical {
    public:
        logistic_system_analytical(num_t alpha, num_t carrying_capacity) 
            : _alpha(alpha), _K(carrying_capacity)
            {
            assert(alpha >= 0.0);
            assert(carrying_capacity > 0.0);
            assert(std::isfinite(alpha));
            assert(std::isfinite(carrying_capacity));
        }

        vector_t operator()(const vector_t& y_0, num_t t_0, num_t t_end) {
            const num_t alpha = _alpha;
            const num_t K = _K;
            const num_t dt = t_end - t_0;
            
            vector_t result("logistic_analytical_result", y_0.extent(0));
            Kokkos::parallel_for(
                "logistic_system_analytical", y_0.extent(0),
                KOKKOS_LAMBDA(const size_t i) {
                    num_t ratio = K / y_0(i) - 1.0;
                    result(i) = K / (1.0 + ratio * exp(-alpha * dt));
                }
            );
            return result;
        }

    private:
        num_t _alpha;           // скорость роста
        num_t _K;               // ёмкость среды
    };

    static_assert(analytical_solution<logistic_system_analytical>, 
        "logistic_system_analytical must fulfill analytical_solution concept");
    
} // namespace testing::functions





#endif // TESTS_FACTORIES_HPP