#ifndef TESTS_FIXTURES_HPP
#define TESTS_FIXTURES_HPP

#include <Grace/defaults.hpp>
#include <Grace/RK4.hpp>
#include <Grace/systems.hpp>

#include <factories.hpp>
#include <generators.hpp>

#include <memory>


namespace lib_testing {

    using namespace Grace::defaults;
    using functions::analytical_solution;

    struct IntegratorPrecisionTestConfiguration {
        using indexed_generator_t = generators::indexed::generator_function_t;

        size_t _n_size;
        size_t _n_steps;

        num_t _dt;
        std::string _name;
        function_system_t _system;

        vector_t _y_0;
        vector_t _y_expected_result;




        template <function_system System>
        static IntegratorPrecisionTestConfiguration make_empty(
            std::string&& name,
            size_t n_size, size_t n_steps, num_t dt,
            System&& system
        ) {
            return IntegratorPrecisionTestConfiguration{
                n_size, n_steps,
                dt, name, std::forward<System>(system),
                vector_t("IntegratorPrecisionTestConfiguration._y_0", n_size),
                vector_t("IntegratorPrecisionTestConfiguration._y_expected_result", n_size)
            };
        }
        
        template <function_system System>
        static IntegratorPrecisionTestConfiguration make(
            std::string name,
            size_t n_size, size_t n_steps, num_t dt,
            System&& system,
            indexed_generator_t y_0_generator,
            indexed_generator_t y_expected_result_generator
        ) {
            
            auto config = make_empty(std::move(name), n_size, n_steps, dt, std::forward<System>(system));

            Kokkos::parallel_for("IntegratorPrecisionTestConfiguration::make_parallel_for", n_size,
                KOKKOS_LAMBDA(const size_t i) {
                    config._y_0(i)               = y_0_generator(i);
                    config._y_expected_result(i) = y_expected_result_generator(i);
                }
            );

            return config;
        }

        template <function_system System, analytical_solution Solution>
        static IntegratorPrecisionTestConfiguration make(
            std::string name,
            size_t n_size, size_t n_steps, num_t dt,
            System&& system,
            Solution&& solution,
            indexed_generator_t y_0_generator
        ) {
            auto config = make_empty(std::move(name), n_size, n_steps, dt, std::forward<System>(system));

            Kokkos::parallel_for("IntegratorPrecisionTestConfiguration::make_parallel_for", n_size,
                KOKKOS_LAMBDA(const size_t i) {
                    config._y_0(i)               = y_0_generator(i);
                }
            );
            config._y_expected_result = solution(config._y_0, 0, n_steps * dt);

            return config;
        }



        std::string to_string() const {
            std::stringstream stream;
            stream << "IntegratorPrecisionTestConfiguration{"
               << "descriptor: " << descriptor()
               << ", size: " << _n_size
               << ", steps: " << _n_steps
               << ", _dt: " << _dt
               << ", expected: [";
            for (size_t i = 0; i < std::min(size_t{3}, _n_size); ++i) {
                if (i > 0) stream << ", ";
                stream << _y_expected_result(i);
            }
            if (_n_size > 3) stream << ", ...";
            stream << "]}";
            return stream.str();
        }

        std::string descriptor() const {
            return _name + "_NSize" + std::to_string(_n_size ) + 
                          "_NSteps" + std::to_string(_n_steps);
        }
    };





    std::ostream& operator<<(std::ostream& stream, const IntegratorPrecisionTestConfiguration& config) {
        return stream << config.to_string();
    }



    class IntegratorBasicTest : public testing::TestWithParam<IntegratorPrecisionTestConfiguration> {
    public:
        using integrator_t = Grace::RK4::integrator<Grace::defaults::function_system_t>;

    protected:
        void SetUp() override {
            config = GetParam();
            integrator = std::make_unique<integrator_t>(config._system, config._y_0, config._dt);
        }


        IntegratorPrecisionTestConfiguration config;
        std::unique_ptr<integrator_t> integrator;

    };





    class IntegratorPrecisionTest : public testing::TestWithParam<IntegratorPrecisionTestConfiguration> {
    public:
        using integrator_t = Grace::RK4::integrator<Grace::defaults::function_system_t>;

    protected:
        void SetUp() override {
            config = GetParam();
            integrator = std::make_unique<integrator_t>(config._system, config._y_0, config._dt);
        }


        IntegratorPrecisionTestConfiguration config;
        std::unique_ptr<integrator_t> integrator;

    };


} // namespace lib_testing



#endif // TESTS_FIXTURES_HPP