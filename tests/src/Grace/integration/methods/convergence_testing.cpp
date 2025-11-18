#include "catch2/interfaces/catch_interfaces_config.hpp"
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

#include <Grace/runtime.hpp>
#include <Grace/systems.hpp>
#include <Grace/integration/methods.hpp>

#include <cstddef>

namespace Grace {
    class test_run_listener : public Catch::EventListenerBase {
    public:
        test_run_listener(const Catch::IConfig * config)
            : Catch::EventListenerBase(config),
            Grace_context(0, nullptr)
        {}

        using EventListenerBase::EventListenerBase;


        Grace::runtime::context Grace_context;
    };
}

CATCH_REGISTER_LISTENER(Grace::test_run_listener);








namespace Grace::integration::methods {


    struct MethodConvergenceTestConfiguration {
        size_t n_size;
        integration_parameters params;
        defaults::ode_system_t system;
        // lib_testing::generators::indexed::generator_function_t y_0_generator;
    };






    template <integration_method Method>
    vector_t integrate_with_multiplier(num_t density_multiplier, 
        const integration_parameters & params, 
        const vector_t & y0,
        const defaults::ode_system_t & system) {

        size_t n_size = y0.extent(0);

        assert(density_multiplier > 0.0);
        integration_parameters dense_params{ 
            params.t_0(),
            params.t_end(), 
            params.dt() / density_multiplier 
        };
        num_t t = dense_params.t_0();
        vector_t y("y", n_size);

        Kokkos::deep_copy(y, y0);
        Method method{ n_size, dense_params };
        while (t < dense_params.t_end()) {
            method.step(system, y, t);
        }
        return y;
    }


    vector_t generate_vector(std::string label, size_t n_size, auto && generator) {
        vector_t result(label, n_size);
        Kokkos::parallel_for(
            "generate_vector with label " + label,
            n_size, 
            KOKKOS_LAMBDA(const size_t i) {
            result(i) = generator(i);
        });
        Kokkos::fence();
        return result;
    }



    TEMPLATE_TEST_CASE("built-in methods have right convergence order",
        "[methods][convergence]", RK4) {

        static constexpr int convergence_order = 4;
        // static constexpr int convergence_order = method_traits<TestType>::convergence_order;

        MethodConvergenceTestConfiguration config{
            2,
            integration_parameters{ 0.0, 10.0, 0.1 },
            systems::harmonic_oscillator(1.0)
        };

        const vector_t y0 = generate_vector("y0", config.n_size,
            [](size_t i) {
                return i == 0 ? 1.0 : 0.0;
            });

        auto integrate = [&](num_t multiplier) {
            return integrate_with_multiplier<TestType>(multiplier, config.params, y0, config.system);
        };




        // Test convergence by comparing solutions with different step sizes
        const vector_t y_coarse = integrate(1.0);
        const vector_t y_fine   = integrate(2.0);
        const vector_t y_finest = integrate(4.0);

        // Calculate errors
        num_t error_coarse = 0.0, error_fine = 0.0;
        Kokkos::parallel_reduce("error_calculation", config.n_size,
            KOKKOS_LAMBDA(const size_t i, num_t & local_error_coarse, num_t & local_error_fine) {
                local_error_coarse += std::abs(y_coarse(i) - y_finest(i));
                local_error_fine += std::abs(y_fine(i) - y_finest(i));
            }, error_coarse, error_fine);

        // Verify convergence order: error should reduce by ~2^convergence_order when halving step size
        const num_t convergence_ratio = error_coarse / error_fine;
        const num_t expected_ratio = std::pow(2.0, convergence_order);
        
        REQUIRE(convergence_ratio > expected_ratio * 0.5);
        REQUIRE(convergence_ratio < expected_ratio * 2.0);
    }


}



