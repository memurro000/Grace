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
#include "functional.hpp"
#include <Kokkos_Array.hpp>
#include <Kokkos_Macros.hpp>
#include <impl/Kokkos_HostThreadTeam.hpp>


namespace Grace::integration::methods {

using defaults::ode_system;
using defaults::integration_method;
using defaults::integration_parameters;
using defaults::num_t;
using defaults::vector_t;

using functional::linear_combination;
using functional::reconsider_solution;



// Methods

class RK4 {
  public:
    GRACE_DEFAULT_VECTOR_T_OWNER(RK4)


    RK4(size_t n_size, integration_parameters parameters) :
          _parameters(parameters),
          _dt{ parameters._dt },
          _half_dt{ _dt / 2.0 },
          _k1("k1", n_size),
          _k2("k2", n_size),
          _k3("k3", n_size),
          _k4("k4", n_size),
          _y_temp("y_temp", n_size) {}


    template <ode_system SystemT> void step(SystemT && system, vector_t & y, num_t & t) {
        if (t + _dt > _parameters._t_end) {
            _dt      = _parameters._t_end - t;
            _half_dt = _dt / 2.0;
        }

        _k1 = system(t, y);
        linear_combination(_y_temp, y, _half_dt, _k1);
        _k2 = system(t + _half_dt, _y_temp);
        linear_combination(_y_temp, y, _half_dt, _k2);
        _k3 = system(t + _half_dt, _y_temp);
        linear_combination(_y_temp, y, _dt, _k3);
        _k4 = system(t + _dt, _y_temp);
        reconsider_solution(y, y, _k1, _k2, _k3, _k4, _dt);

        t += _dt;
    }



  private:
    integration_parameters _parameters;

    num_t _dt;
    num_t _half_dt;

    vector_t _k1;
    vector_t _k2;
    vector_t _k3;
    vector_t _k4;
    vector_t _y_temp;
};
static_assert(integration_method<RK4>);



class RK4_opt {
  public:
    GRACE_DEFAULT_VECTOR_T_OWNER(RK4_opt)


    RK4_opt(size_t n_size, integration_parameters parameters) :
          _parameters(parameters),
          _dt{ parameters._dt },
          _half_dt{ _dt / 2.0 },
          _k{
            vector_t("k1", n_size),
            vector_t("k2", n_size),
            vector_t("k3", n_size),
            vector_t("k4", n_size)
          },
          _y_temp("y_temp", n_size) {}


    template <ode_system SystemT> void step(SystemT && system, vector_t & y, num_t & t) {
        if (t + _dt > _parameters._t_end) {
            _dt      = _parameters._t_end - t;
            _half_dt = _dt / 2.0;
        }

        Kokkos::parallel_for(
            y.extent(0),
            KOKK (const size_t i) {
                _k[0](i) = y(i);
            }
        );



        system(t           , y      , _k[0]);
        comb(_y_temp, y, _half_dt, _k[0]);
        system(t + _half_dt, _y_temp, _k[1]); comb(_y_temp, y, _half_dt, _k[1]);
        system(t + _half_dt, _y_temp, _k[2]); comb(_y_temp, y, _dt     , _k[2]);
        system(t + _dt     , _y_temp, _k[3]);

        reconsider_solution(y, y, _k, _dt / 6.0);

        t += _dt;
    }


  private:


    KOKKOS_INLINE_FUNCTION
    void comb(vector_t & result, const vector_t & x, num_t a, const vector_t & y) {
        Kokkos::parallel_for(
            x.extent(0),
            KOKKOS_LAMBDA(const size_t i) {
                result(i) = x(i) + a * y(i);
            }
        );
    }



    KOKKOS_INLINE_FUNCTION
    void reconsider_solution(vector_t & result,
        const vector_t & prev,
        const Kokkos::Array<vector_t, 4> & k,
        num_t dt_fract_6) {
        Kokkos::parallel_for(
            prev.extent(0),
            KOKKOS_LAMBDA(const size_t i) {
                result(i) = prev(i) + dt_fract_6 * (k[0](i) + 2.0 * (k[1](i) + k[2](i)) + k[3](i));
            }
        );
    }

    integration_parameters _parameters;

    num_t _dt;
    num_t _half_dt;

    // TODO benchmark std::array alternative
    Kokkos::Array<vector_t, 4> _k;

    vector_t _y_temp;
};
static_assert(integration_method<RK4_opt>);









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