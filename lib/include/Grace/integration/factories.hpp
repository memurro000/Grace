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

#ifndef GRACE_INTEGRATION_FACTORIES_HPP
#define GRACE_INTEGRATION_FACTORIES_HPP

#include "integration.hpp"
#include "methods.hpp"

namespace Grace::integration::factories {

    using defaults::vector_t;
    using defaults::function_system;
    using defaults::integration_method;


    template <integration_method Method> 
    auto make_integrator(
        function_system auto&& system,
        num_t t_0, num_t t_end, num_t dt,
        const vector_t& y_0
    )
    {
        using SystemT = decltype(system);
        return integrator<Method, std::decay_t<SystemT>>(
            std::forward<SystemT>(system),
            t_0, t_end, dt,
            y_0
        );
    }

    
    auto make_RK4_integrator (
        function_system auto&& system,
        num_t t_0, num_t t_end, num_t dt,
        const vector_t& y_0
    )
    {
        using SystemT = decltype(system);
        return make_integrator<methods::RK4, std::decay_t<SystemT>>(
            std::forward<SystemT>(system),
            t_0, t_end, dt,
            y_0
        );
    }


} // namespace Grace::integration::factories






#endif // GRACE_INTEGRATION_FACTORIES_HPP