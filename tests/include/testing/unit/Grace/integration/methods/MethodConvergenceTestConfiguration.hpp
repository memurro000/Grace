#ifndef GRACE_INTEGRATION_METHODS_METHOD_TEST_CONFIGURATION_HPP
#define GRACE_INTEGRATION_METHODS_METHOD_TEST_CONFIGURATION_HPP


#include <cstddef>


#include <Grace/integration/methods.hpp>

#include "../../../../../generators.hpp"

namespace Grace::integration::methods {


    struct MethodConvergenceTestConfiguration {
        size_t n_size;
        integration_parameters params;
        defaults::ode_system_t system;
        lib_testing::generators::indexed::generator_function_t y_0_generator;
    };
    



}






#endif // GRACE_INTEGRATION_METHODS_METHOD_TEST_CONFIGURATION_HPP