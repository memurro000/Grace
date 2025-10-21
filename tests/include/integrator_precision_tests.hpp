#ifndef TESTS_FIXTURES_HPP
#define TESTS_FIXTURES_HPP

#include "factories.hpp"
#include "generators.hpp"

#include <Grace/defaults.hpp>
#include <Grace/integration/RK4_no_policies.hpp>
#include <Grace/integration/integration.hpp>
#include <Grace/integration/methods.hpp>
#include <Grace/systems.hpp>
#include <gtest/gtest.h>
#include <memory>


namespace lib_testing {

using namespace Grace::defaults;
namespace integration = Grace::integration;

using functions::analytical_solution;

struct IntegratorPrecisionTestConfiguration {
    using indexed_generator_t = generators::indexed::generator_function_t;

    std::string       _name;
    function_system_t _system;

    size_t _n_size;
    size_t _n_steps;
    num_t  _dt;


    vector_t _y_0;
    vector_t _y_expected_result;


    template <function_system System>
    static IntegratorPrecisionTestConfiguration
        make_empty(std::string && name, size_t n_size, size_t n_steps, num_t dt, System && system) {
        return IntegratorPrecisionTestConfiguration{
            name,
            std::forward<System>(system),
            n_size,
            n_steps,
            dt,
            vector_t("IntegratorPrecisionTestConfiguration._y_0", n_size),
            vector_t("IntegratorPrecisionTestConfiguration._y_expected_result", n_size)
        };
    }

    template <function_system System, analytical_solution Solution>
    static IntegratorPrecisionTestConfiguration
        make(std::string name, size_t n_size, size_t n_steps, num_t dt, System && system,
             Solution && solution, indexed_generator_t y_0_generator) {
        auto config =
            make_empty(std::move(name), n_size, n_steps, dt, std::forward<System>(system));

        Kokkos::parallel_for(
            "IntegratorPrecisionTestConfiguration::make_parallel_for", n_size,
            KOKKOS_LAMBDA(const size_t i) { config._y_0(i) = y_0_generator(i); });
        config._y_expected_result = solution(config._y_0, 0.0, n_steps * dt);

        return config;
    }


    std::string descriptor() const {
        return _name + "_NSize" + std::to_string(_n_size) + "_NSteps" + std::to_string(_n_steps);
    }
};

inline std::string to_string(const IntegratorPrecisionTestConfiguration & config) {
    std::stringstream stream;
    stream << "IntegratorPrecisionTestConfiguration{\n"
              "\tdescriptor: "
           << config.descriptor() << '\n'
           << "\tsize:       " << config._n_size << '\n'
           << "\tsteps:      " << config._n_steps << '\n'
           << "\tdt:         " << config._dt << '\n'
           << "\texpected:   [" << ' ';
    for (size_t i = 0; i < std::min(size_t{ 7 }, config._n_size); ++i) {
        stream << config._y_expected_result(i);
        if (i != 7) stream << ", ";
    }
    if (config._n_size > 7) stream << "...";
    stream << " ]\n}";
    return stream.str();
}

inline std::ostream & operator<<(std::ostream &                               stream,
                                 const IntegratorPrecisionTestConfiguration & config) {
    return stream << to_string(config);
}



class IntegratorPrecisionTest :
      public testing::TestWithParam<IntegratorPrecisionTestConfiguration> {
  public:
    using integrator_t = integration::integrator<integration::methods::RK4, function_system_t>;
    // using integrator_t = Grace::RK4_no_policies::integrator<function_system_t>;

  protected:
    void SetUp() override {
        config     = GetParam();
        integrator = std::make_unique<integrator_t>(config._system, 0, config._dt * config._n_steps,
                                                    config._dt, config._y_0);
    }


    IntegratorPrecisionTestConfiguration config;
    std::unique_ptr<integrator_t>        integrator;
};


} // namespace lib_testing



#endif // TESTS_FIXTURES_HPP