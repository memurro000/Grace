

#include <gtest/gtest.h>
#include <iostream>
#include <Grace/runtime.hpp>
#include <Grace/integration/methods.hpp>
#include <Grace/systems.hpp>
#include <cmath>

class GraceTestEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        std::cout << "Setting up Grace runtime..." << std::endl;
        std::cout.flush();
        Grace::runtime::initialize(0, nullptr);
        std::cout << "Grace runtime initialized" << std::endl;
        std::cout.flush();
    }
    
    void TearDown() override {
        std::cout << "Tearing down Grace runtime..." << std::endl;
        Grace::runtime::finalize();
        std::cout << "Grace runtime finalized" << std::endl;
    }
};

// Register the environment
static ::testing::Environment* const grace_env = 
    ::testing::AddGlobalTestEnvironment(new GraceTestEnvironment);

#include "MethodConvergenceTestConfiguration.hpp"
#include "../../../../../generators.hpp"

namespace Grace::integration::methods {

    template <defaults::integration_method Method>
    class MethodConvergenceTest : public ::testing::Test {
    protected:
        void use_config(MethodConvergenceTestConfiguration config) {
            n_size = config.n_size;
            params = std::make_unique<integration_parameters>(config.params);
            system = config.system;
            y_0 = vector_t("y_0", config.n_size);
            auto generator = config.y_0_generator;
            auto y_0_local = y_0;
            Kokkos::parallel_for(config.n_size, 
              KOKKOS_LAMBDA(const size_t i) {
                y_0_local(i) = generator(i);
            });
            Kokkos::fence();
        }

        vector_t integrate(num_t density_multiplier) {
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
        size_t n_size = 0;
        std::unique_ptr<integration_parameters> params;
        defaults::ode_system_t system;
        vector_t y_0;
    };

    TYPED_TEST_SUITE_P(MethodConvergenceTest);

    TYPED_TEST_P(MethodConvergenceTest, StepIntegration_dtIsDivided_ResultConvergesAccordingly) {
        auto configured_run = [&, this](MethodConvergenceTestConfiguration config) {
            this->use_config(config);

            auto y_coarse = this->integrate(1.0);
            auto y_fine = this->integrate(2.0);
            auto y_finer = this->integrate(4.0);

            num_t error_coarse = 0.0;
            num_t error_fine = 0.0;

            Kokkos::parallel_reduce(
                this->n_size,
                KOKKOS_LAMBDA(const size_t i, num_t & sum) {
                    num_t diff = y_fine(i) - y_coarse(i);
                    sum += diff * diff;
                },
                error_coarse);

            Kokkos::parallel_reduce(
                this->n_size,
                KOKKOS_LAMBDA(const size_t i, num_t & sum) {
                    num_t diff = y_finer(i) - y_fine(i);
                    sum += diff * diff;
                },
                error_fine);

            error_coarse = std::sqrt(error_coarse);
            error_fine = std::sqrt(error_fine);

            ASSERT_GT(error_fine, 0.0) << "Fine error is zero, cannot calculate convergence ratio";
            num_t convergence_ratio = error_coarse / error_fine;
            num_t expected_ratio = std::pow(2.0, this->convergence_order);

            EXPECT_NEAR(convergence_ratio, expected_ratio, 0.5 * expected_ratio)
                << "Convergence order: " << this->convergence_order;
        };

        configured_run({
            2,
            integration_parameters{ 0.0, 10.0, 0.1 },
            Grace::systems::harmonic_oscillator(1.0),
            lib_testing::generators::indexed::constant(1.0)
        });

        configured_run({
            2,
            integration_parameters{ 0.0, 20.0, 0.05 },
            Grace::systems::harmonic_oscillator(2.0),
            lib_testing::generators::indexed::constant(0.5)
        });
    }

    REGISTER_TYPED_TEST_SUITE_P(MethodConvergenceTest, StepIntegration_dtIsDivided_ResultConvergesAccordingly);

    INSTANTIATE_TYPED_TEST_SUITE_P(ForTestedMethodTypes, MethodConvergenceTest, ::testing::Types<RK4>);

}

TEST(BasicMethodTest, BasicTest) {
    EXPECT_TRUE(true);
}