#include <memory>
#include <string>




#include "preferenced.hpp"
#include "fixtures.hpp"
#include "factories.hpp"



namespace lib_testing {

    TEST_P(IntegratorBasicTest, Step_FinalState_MatchesExpected) {
        ASSERT_EQ(config._n_size, config._y_0.extent(0));
        ASSERT_EQ(config._n_size, config.y_expected_result.extent(0));
    
        vector_t current{config._y_0};

        for (size_t i = 0; i < config._n_steps; ++i) {
            current = integrator->step();
        }

        for (size_t i = 0; i < config._n_size; ++i) {
            ASSERT_NEAR(current(i), config.y_expected_result(i), Grace::defaults::EPSILON);
        }

    }



    INSTANTIATE_TEST_SUITE_P(ZeroSystem, IntegratorBasicTest,
        testing::Values(
            IntegratorBasicTestConfiguration{
                0, 10,
                0.1,
                "ZeroSystem_Empty", functions::zero_system,
                vectors::make("_y_0"              , 0),
                vectors::make("y_expected_result", 0)
            },
            IntegratorBasicTestConfiguration{
                1, 10,
                0.1,
                "ZeroSystem_Scalar", functions::zero_system,
                vectors::make("_y_0"              , 1, 0.0),
                vectors::make("y_expected_result", 1, 0.0)
            },
            IntegratorBasicTestConfiguration{
                5, 100,
                0.1,
                "ZeroSystem", functions::zero_system,
                vectors::make("_y_0"              , 5, 1.0),
                vectors::make("y_expected_result", 5, 1.0)
            },
            IntegratorBasicTestConfiguration{
                100, 1000,
                0.1,
                "ZeroSystem", functions::zero_system,
                vectors::make("_y_0"              , 100, 1.0),
                vectors::make("y_expected_result", 100, 1.0)
            },
            IntegratorBasicTestConfiguration{
                50000, 1000,
                0.1,
                "ZeroSystem", functions::zero_system,
                vectors::make("_y_0"              , 50000, 1.0),
                vectors::make("y_expected_result", 50000, 1.0)
            }
        )
    );










} // namespace lib_testing

    






TESTS_PREFERENCED_MAIN();
