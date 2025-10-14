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

#ifndef GRACE_DEFAULTS_HPP
#define GRACE_DEFAULTS_HPP

#include <Kokkos_Core.hpp>

#include <functional>

namespace Grace::defaults {

    using num_t = double;
    using vector_t = Kokkos::View<num_t*>;
    using function_system_t = std::function<vector_t(num_t, const vector_t&)>;



    template <typename Callable>
    concept function_system = 
    requires(Callable function, num_t t, const vector_t& y) {
        { function(t, y) } -> std::same_as<vector_t>;
    };

    static_assert(function_system<function_system_t>, "function_system_t must fulfill function_system concept");





    constexpr num_t EPSILON = 1e-11;




    namespace param
    {
        using vector       = vector_t;
        using vector_in    = const vector&;
        using vector_inout = vector&;
        using vector_out   = vector;
        
    } // namespace param
    


} // namespace Grace::defaults








#endif // GRACE_DEFAULTS_HPP