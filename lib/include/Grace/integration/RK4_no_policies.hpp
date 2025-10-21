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

#ifndef GRACE_RK4_NO_POLICIES_HPP
#define GRACE_RK4_NO_POLICIES_HPP



#include "functional.hpp"



namespace Grace::RK4_no_policies {
using namespace integration::functional;
using namespace defaults;



template <typename T>
concept step_result_handler = requires(T && t, Kokkos::View<const double *> step_result) {
    { t(step_result) } -> std::same_as<void>;
};



template <function_system System> class integrator {
  public:
    template <typename S = System>
    integrator(S && system, num_t t_0, num_t t_end, num_t dt, vector_in y_0) :
          // Constants
          _system{ std::forward<S>(system) },
          _size{ y_0.extent(0) },
          _parameters{ t_0, t_end, dt },
          _dt{ dt },
          _half_dt{ dt / 2 },
          // Workers
          _t{ t_0 },
          _y("y", _size),
          // Temporaries
          _k1("k1", _size),
          _k2("k2", _size),
          _k3("k3", _size),
          _k4("k4", _size),
          _y_temp("y_temp", _size) {
        // Workers
        Kokkos::deep_copy(_y, y_0);
    }


    Kokkos::View<const double *> tie_step_result() const { return _y; }


    bool step(Kokkos::View<const double *> & step_result) {
        compute_step();
        step_result = _y;
        return _t < _parameters._t_end;
    }


    bool step() {
        compute_step();
        return _t < _parameters._t_end;
    }


    Kokkos::View<const double *> integrate() {
        while (step());
        return tie_step_result();
    }


    template <step_result_handler Handler>
    Kokkos::View<const double *> integrate(Handler && handler) {
        auto step_result = tie_step_result();
        while (step()) { handler(step_result); }
        return step_result;
    }



  private:
    void compute_step() {
        if (_t + _dt > _parameters._t_end) {
            _dt      = _parameters._t_end - _t;
            _half_dt = _dt / 2.0;
        }

        _k1 = _system(_t, _y);
        linear_combination(_y_temp, _y, _half_dt, _k1);
        _k2 = _system(_t + _half_dt, _y_temp);
        linear_combination(_y_temp, _y, _half_dt, _k2);
        _k3 = _system(_t + _half_dt, _y_temp);
        linear_combination(_y_temp, _y, _dt, _k3);
        _k4 = _system(_t + _dt, _y_temp);
        reconsider_solution(_y, _y, _k1, _k2, _k3, _k4, _dt);

        _t += _dt;
    }



    struct {
        num_t _t_0;
        num_t _t_end;
        num_t _dt;
    } _parameters;


    System _system;
    size_t _size;
    num_t  _dt;
    num_t  _half_dt;

    num_t _t;

    vector _y;
    vector _k1;
    vector _k2;
    vector _k3;
    vector _k4;
    vector _y_temp;
};



template <function_system System>
integrator<std::decay_t<System>> make_integrator(System && system, num_t t_0, num_t t_end, num_t dt,
                                                 vector_in y_0) {
    return integrator<std::decay_t<System>>(std::forward<System>(system), t_0, t_end, dt, y_0);
}


}; // namespace Grace::RK4_no_policies



#endif // GRACE_RK4_NO_POLICIES_HPP