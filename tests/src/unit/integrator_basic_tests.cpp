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



    INSTANTIATE_TEST_SUITE_P(ZeroSystem, IntegratorBasicTest,
        testing::Values(
            IntegratorBasicTestConfiguration::make(
                "ZeroSystem",
                0, 10, 0.1,
                functions::zero_system,
                generators::indexed::zero,
                generators::indexed::zero
            ),
            IntegratorBasicTestConfiguration::make(
                "ZeroSystem",
                1, 100, 0.1,
                functions::zero_system,
                generators::indexed::zero,
                generators::indexed::zero
            ),
            IntegratorBasicTestConfiguration::make(
                "ZeroSystem",
                100, 100, 0.1,
                functions::zero_system,
                generators::indexed::zero,
                generators::indexed::zero
            ),
            IntegratorBasicTestConfiguration::make(
                "ZeroSystem",
                50000, 1000, 0.1,
                functions::zero_system,
                generators::indexed::zero,
                generators::indexed::zero
            ),
            IntegratorBasicTestConfiguration::make(
                "ZeroSystem",
                100, 1000, 0.1,
                functions::zero_system,
                generators::indexed::index,
                generators::indexed::index
            ),
            IntegratorBasicTestConfiguration::make(
                "ZeroSystem",
                100, 1000, 0.1,
                functions::zero_system,
                generators::indexed::step2,
                generators::indexed::step2
            )
        )
    );



} // namespace lib_testing

    






TESTS_PREFERENCED_MAIN();
