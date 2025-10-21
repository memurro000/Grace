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

#ifndef GRACE_INTEGRATION_FUNCTIONAL_HPP
#define GRACE_INTEGRATION_FUNCTIONAL_HPP

#include "defaults.hpp"


namespace Grace::integration::functional {

using num_t = defaults::num_t;
using namespace defaults::parametric_vector;

inline void linear_combination(vector_inout result, vector_in a, num_t multiplier, vector_in b) {
    Kokkos::parallel_for(
        "linear_combination", a.extent(0),
        KOKKOS_LAMBDA(const size_t i) { result(i) = a(i) + multiplier * b(i); });
}

inline vector_out linear_combination(vector_in a, num_t multiplier, vector_in b) {
    vector done("linear_combination_done", a.extent(0));
    linear_combination(done, a, multiplier, b);
    return done;
}

inline void reconsider_solution(vector_inout result, vector_in prev_solution, vector_in k1,
                                vector_in k2, vector_in k3, vector_in k4, num_t dt) {
    const num_t dt_fract_6 = dt / 6.0;
    Kokkos::parallel_for(
        "reconsider_solution", prev_solution.extent(0), KOKKOS_LAMBDA(const size_t i) {
            result(i) = prev_solution(i) + dt_fract_6 * (k1(i) + 2.0 * (k2(i) + k3(i)) + k4(i));
        });
}

inline vector_out reconsider_solution(vector_in prev_solution, vector_in k1, vector_in k2,
                                      vector_in k3, vector_in k4, num_t dt) {
    vector done("reconsider_solution_done", prev_solution.extent(0));
    reconsider_solution(done, prev_solution, k1, k2, k3, k4, dt);
    return done;
}

} // namespace Grace::integration::functional


#endif // GRACE_FUNCTIONAL_HPP