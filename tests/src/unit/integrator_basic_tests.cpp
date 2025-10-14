#include <memory>
#include <string>




#include "preferenced.hpp"
#include "fixtures.hpp"
#include "factories.hpp"
#include "generators.hpp"



namespace lib_testing {

    TEST_P(IntegratorBasicTest, Step_FinalState_MatchesExpected) {
        ASSERT_EQ(config._n_size, config._y_0.extent(0));
        ASSERT_EQ(config._n_size, config._y_expected_result.extent(0));
    
        vector_t current{config._y_0};


        for (size_t i = 0; i < config._n_steps; ++i) {
            current = integrator->step();
        }

        for (size_t i = 0; i < config._n_size; ++i) {
            ASSERT_NEAR(current(i), config._y_expected_result(i), Grace::defaults::EPSILON) << " on i = " << i;
        }

    }

    namespace {

        namespace gens = generators::indexed;


        IntegratorPrecisionTestConfiguration make_for_zero_system(
            size_t n_size, size_t n_steps, num_t dt = 0.1,
            generators::indexed::generator_function_t vector_generator = gens::zero
        ) {
            return IntegratorPrecisionTestConfiguration::make(
                "ZeroSystem",
                n_size, n_steps, dt,
                functions::zero_system,
                vector_generator, vector_generator
            );
        }

        INSTANTIATE_TEST_SUITE_P(ZeroSystem, IntegratorBasicTest,
            testing::Values(
            )
        );

    }

} // namespace lib_testing

    






TESTS_PREFERENCED_MAIN();
