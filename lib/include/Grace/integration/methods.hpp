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


#ifndef GRACE_INTEGRATION_METHODS_HPP
#define GRACE_INTEGRATION_METHODS_HPP


#include "Grace/defaults.hpp"
#include "defaults.hpp"

#include <Kokkos_Array.hpp>
#include <Kokkos_Core.hpp>
#include <Kokkos_Macros.hpp>
#include <impl/Kokkos_HostThreadTeam.hpp>


namespace Grace::integration::methods {

using defaults::integration_method;
using defaults::integration_parameters;
using defaults::num_t;
using defaults::ode_system;
using defaults::vector_t;



// Methods


class RK4 {
    GRACE_DEFAULT_VECTOR_T_OWNER_CONSTRUCTORS(RK4);

  public:
    RK4(size_t n_size, integration_parameters parameters) :
          _parameters(parameters),
          _dt{ parameters.dt() },
          _half_dt{ _dt / 2.0 },
          _k{ vector_t("k1", n_size), vector_t("k2", n_size), vector_t("k3", n_size),
              vector_t("k4", n_size) },
          _y_temp{ vector_t("y_temp0", n_size), vector_t("y_temp1", n_size),
                   vector_t("y_temp2", n_size) } {}


    template <ode_system SystemT> void step(SystemT && system, vector_t & y, num_t & t) {
        if (t + _dt > _parameters.t_end()) {
            _dt      = _parameters.t_end() - t;
            _half_dt = _dt / 2.0;
        }

        Kokkos::parallel_for(
            y.extent(0), KOKKOS_CLASS_LAMBDA(const size_t i) {
                _k[0](i)      = system(t, y, i);
                _y_temp[0](i) = comb(y(i), _half_dt, _k[0](i));
            });

        // TODO consider space-dependent fences
        Kokkos::fence("RK4_opt step fence 1");

        Kokkos::parallel_for(
            y.extent(0), KOKKOS_CLASS_LAMBDA(const size_t i) {
                _k[1](i)      = system(t + _half_dt, _y_temp[0], i);
                _y_temp[1](i) = comb(y(i), _half_dt, _k[1](i));
            });

        Kokkos::fence("RK4_opt step fence 2");

        Kokkos::parallel_for(
            y.extent(0), KOKKOS_CLASS_LAMBDA(const size_t i) {
                _k[2](i)      = system(t + _half_dt, _y_temp[1], i);
                _y_temp[2](i) = comb(y(i), _dt, _k[2](i));
            });

        Kokkos::fence("RK4_opt step fence 3");

        Kokkos::parallel_for(
            y.extent(0),
            KOKKOS_CLASS_LAMBDA(const size_t i) { _k[3](i) = system(t + _dt, _y_temp[2], i); });

        Kokkos::fence("RK4_opt step fence before solution recomp");

        Kokkos::parallel_for(
            y.extent(0), KOKKOS_CLASS_LAMBDA(const size_t i) {
                y(i) += _dt / 6.0 * (_k[0](i) + 2.0 * (_k[1](i) + _k[2](i)) + _k[3](i));
            });

        t += _dt;
    }


  private:
    KOKKOS_FORCEINLINE_FUNCTION
    num_t comb(num_t x, num_t a, num_t y) const noexcept { return x + a * y; }


    integration_parameters _parameters;

    num_t _dt;
    num_t _half_dt;

    Kokkos::Array<vector_t, 4> _k;

    Kokkos::Array<vector_t, 3> _y_temp;
};
static_assert(integration_method<RK4>);



// Traits


template <typename Method> struct method_traits;

template <typename Method>
concept described_method = integration_method<Method> && requires {
    typename method_traits<Method>;
    { method_traits<Method>::order } -> std::convertible_to<int>;
    { method_traits<Method>::convergence_order } -> std::convertible_to<int>;
};



template <> struct method_traits<RK4> {
    static constexpr int order             = 4;
    static constexpr int convergence_order = 4;
};
static_assert(described_method<RK4>);


} // namespace Grace::integration::methods



#endif // GRACE_INTEGRATION_METHODS_HPP