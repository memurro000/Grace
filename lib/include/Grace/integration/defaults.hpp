/**
 * Copyright 2025 memurro000 (https://github.com/memurro000)
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GRACE_INTEGRATION_DEFAULTS_HPP
#define GRACE_INTEGRATION_DEFAULTS_HPP


#include "../defaults.hpp"


namespace Grace::integration::defaults
{
    using Grace::defaults::num_t;
    using Grace::defaults::vector_t;
    using Grace::defaults::function_system;

    namespace parametric_vector = Grace::defaults::param;




    // An aggregate used to store integration constraints 
    // in the instances of integrator and policy classes
    struct integration_parameters { num_t _t_0, _t_end, _dt; };

    

    // A concept for policies used to select one of the integration methods
    template <typename Policy, typename FunctionSystemT>
    concept integration_method =
        requires(Policy method, FunctionSystemT& system, vector_t& y, num_t& t) { // step method constraint
            requires function_system<FunctionSystemT>;
            { method.step(system, y, t) } -> std::same_as<void>;
        } &&
        requires(size_t n_size, integration_parameters parameters) {           // constructor constraint
            { Policy(n_size, parameters) } -> std::same_as<Policy>;
        };




    // A concept for a functor used to handle intermediate result of integration 
    // by instances of integrator class
    template <typename FunctorT>
    concept step_result_handler =
        requires(FunctorT&& t, Kokkos::View<const double *> step_result) {
            { t(step_result) } -> std::same_as<void>;
        };


} // namespace Grace::integration::defaults






#endif // GRACE_INTEGRATION_DEFAULTS_HPP