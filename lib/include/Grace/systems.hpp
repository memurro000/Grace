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

#ifndef GRACE_SYSTEMS_HPP
#define GRACE_SYSTEMS_HPP


#include "defaults.hpp"
#include <Kokkos_Core_fwd.hpp>
#include <Kokkos_Macros.hpp>
#include <cstddef>



namespace Grace::systems {
using namespace defaults::parametric_vector;
using namespace defaults;

#ifndef GRACE_UNUSED_PAR
# define GRACE_UNUSED_PAR(par) (void)par
#endif


class harmonic_oscillator {
  public:

    harmonic_oscillator(num_t omega) : _omega{ omega } {}


    KOKKOS_FUNCTION
    num_t operator()(num_t t, vector_in y, size_t i) const {
        GRACE_UNUSED_PAR(t);
        if (i == 0) {
            return y(1);
        }
        else if (i == 1) {
            return -_omega * _omega * y(0);
        }
    }

    KOKKOS_FUNCTION
    void operator()(num_t t, vector_in y, vector_inout dybydt) const {
        GRACE_UNUSED_PAR(t);
        dybydt(0) = y(1);
        dybydt(1) = -_omega * _omega * y(0);
    }


  private:
    num_t _omega;
};
static_assert(ode_system<harmonic_oscillator>,
              "harmonic_oscillator must fulfill function_system concept");



} // namespace Grace::systems



#endif // GRACE_SYSTEMS_HPP