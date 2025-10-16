#include <memory>
#include <string>




#include "preferenced.hpp"
#include "fixtures.hpp"
#include "factories.hpp"
#include "generators.hpp"



namespace lib_testing {

    TEST_P(IntegratorPrecisionTest, Step_FinalState_MatchesExpected) {
        ASSERT_EQ(config._n_size, config._y_0.extent(0));
        ASSERT_EQ(config._n_size, config._y_expected_result.extent(0));
    



        auto result = integrator->integrate();
        


        for (size_t i = 0; i < config._n_size; ++i) {
            ASSERT_NEAR(
                result(i), config._y_expected_result(i),
                std::abs(Grace::defaults::EPSILON * config._y_expected_result(i))
            ) << " on i = " << i;
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

        INSTANTIATE_TEST_SUITE_P(ZeroSystem, IntegratorPrecisionTest,
            testing::Values(
                make_for_zero_system(50000, 1000),
                make_for_zero_system(100, 1000, 0.1, gens::index),
                make_for_zero_system(100, 1000, 0.1, gens::index_step2)
            )
        );



        IntegratorPrecisionTestConfiguration make_for_constant_system(
            num_t constant,
            size_t n_size, size_t n_steps, num_t dt = 0.001, 
            gens::generator_function_t y_0_generator = gens::zero
        ) {
            return IntegratorPrecisionTestConfiguration::make(
                "ConstantSystem",
                n_size, n_steps, dt,
                functions::constant_system(constant),
                functions::constant_system_analytical(constant),
                y_0_generator
            );
        }



        INSTANTIATE_TEST_SUITE_P(ConstantSystem, IntegratorPrecisionTest,
            testing::Values(
                //make_for_constant_system(5.0, 100, 1000),
                make_for_constant_system(11.0, 100, 1000),
                make_for_constant_system(-10000.0, 1000, 100),
                make_for_constant_system(50000.0, 1000, 1000, 0.001, gens::index),
                make_for_constant_system(7000.0, 1000, 1000, 0.1, gens::index_step2),
                make_for_constant_system(-10000.0, 1000, 1000, 0.001, gens::index_step2),
                make_for_constant_system(10000.0, 2000, 1000, 0.001, gens::index_step2),
                make_for_constant_system(10000.0, 2000, 10000, 0.0001, gens::index_step2),
                make_for_constant_system(10000.0, 2000, 10000, 0.00001, gens::index_step2)
            )
        );


        IntegratorPrecisionTestConfiguration make_for_logistic_system(
            num_t alpha, num_t carrying_capacity,
            size_t n_size, size_t n_steps, num_t dt = 0.1,
            gens::generator_function_t start_occupancy_generator = gens::zero
        ) {
            using std::to_string;
            return IntegratorPrecisionTestConfiguration::make(
                std::string("LogisticSystem") + 
                    "_Alpha" + to_string(alpha) + 
                    "_CarryingCapacity" + to_string(carrying_capacity),
                n_size, n_steps, dt,
                functions::logistic_system(alpha, carrying_capacity),
                functions::logistic_system_analytical(alpha, carrying_capacity),
                start_occupancy_generator
            );
        }




        INSTANTIATE_TEST_SUITE_P(LogisticSystem, IntegratorPrecisionTest,
            testing::Values(
                make_for_logistic_system(1.0, 100.0, 100, 100, 0.001, gens::constant(10.0)),
                make_for_logistic_system(1.0, 100.0, 100, 100, 0.001, gens::constant(50.0)),
                make_for_logistic_system(1.0, 100.0, 100, 100, 0.001, gens::constant(90.0)),
                make_for_logistic_system(1.0, 100.0, 100, 100, 0.001, gens::constant(99.9)), // big occupancies
                make_for_logistic_system(2.0, 100.0, 100, 100, 0.001, gens::linear  (0.01)), // small occupancies
                make_for_logistic_system(0.1, 50.0, 50, 300, 0.01,    gens::constant(5.0 )), // Slow growth, small capacity
                make_for_logistic_system(5.0, 1000.0, 200, 100, 0.1,  gens::constant(100.0)) // Fast growth, big capacity
            )
        );



    }

} // namespace lib_testing

    






TESTS_PREFERENCED_MAIN();
