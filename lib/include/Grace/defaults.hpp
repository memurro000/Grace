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
#include <concepts>
#include <cstddef>
#include <functional>

namespace Grace::defaults {

using num_t    = double;
using vector_t = Kokkos::View<num_t *>;

namespace parametric_vector {
    using vector       = vector_t;
    using vector_in    = const vector &;
    using vector_inout = vector &;
    using vector_out   = vector;
} // namespace parametric_vector


using ode_system_t = std::function<num_t(num_t, parametric_vector::vector_in, size_t)>;


template <typename Callable>
concept ode_system =
    requires(Callable function, num_t t, parametric_vector::vector_in y, size_t index) {
        { function(t, y, index) } -> std::same_as<num_t>; // computes derivative component
    };



static_assert(ode_system<ode_system_t>);



constexpr num_t EPSILON = 1e-11;



} // namespace Grace::defaults



#endif // GRACE_DEFAULTS_HPP