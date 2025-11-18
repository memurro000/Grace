#ifndef GRACE_INTEGRATION_METHODS_METHOD_TEST_HPP
#define GRACE_INTEGRATION_METHODS_METHOD_TEST_HPP

#include <memory>

#include <Kokkos_Core.hpp>
#include <cstddef>
#include <gtest/gtest.h>

#include "Grace/integration/defaults.hpp"
#include "MethodConvergenceTestConfiguration.hpp"

#include <Grace/integration/methods.hpp>




namespace Grace::integration::methods {

    template <defaults::integration_method Method>
    class MethodConvergenceTest :
        public ::testing::Test {
      protected:



        // Instead of the SetUp method
        // because of the fact Google Test doesn't support
        // typed tests with real config parameters
        void use_config(MethodConvergenceTestConfiguration config) {
            // used_config = config;
            n_size = config.n_size;
            params = std::make_unique<integration_parameters>(config.params);
            system = config.system;
            y_0 = vector_t("y_0", config.n_size);
            auto generator = config.y_0_generator;
            auto y_0_local = y_0; // For KOKKOS_LAMBDA
                                  // KOKKOS_CLASS_LAMBDA unavailable
                                  // ::testing::Test superclass
                                  // copy constructor is deleted
            Kokkos::parallel_for(config.n_size, 
              KOKKOS_LAMBDA(const size_t i) {
                y_0_local(i) = generator(i);
            });
            Kokkos::fence();
        }

        vector_t integrate(num_t density_multiplier) {
            assert(density_multiplier > 0.0);
            integration_parameters dense_params{ 
              params->t_0(),
              params->t_end(), 
              params->dt() / density_multiplier 
            };
            num_t t = dense_params.t_0();
            vector_t y("y", n_size);
            Kokkos::deep_copy(y, y_0);
            Method method{ n_size, dense_params };
            while (t < dense_params.t_end()) {
                method.step(system, y, t);
            }
            return y;
        }


        static constexpr int convergence_order = method_traits<Method>::convergence_order;

        // MethodConvergenceTestConfiguration used_config;

        size_t n_size = 0;

        // Because integration_parameters' default constructor is deleted
        std::unique_ptr<integration_parameters> params;

        defaults::ode_system_t system;

        vector_t y_0;

    };



} // Grace::integration::methods::convergence





#endif // GRACE_INTEGRATION_METHODS_METHOD_TEST_HPP