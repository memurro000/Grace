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

#ifndef GRACE_INTEGRATION_INTEGRATION_HPP
#define GRACE_INTEGRATION_INTEGRATION_HPP

#include "Grace/defaults.hpp"
#include "defaults.hpp"
#include <Kokkos_Core.hpp>
#include <exception>
#include <stdexcept>
#include <string>



namespace Grace::integration {
using defaults::ode_system;
using defaults::integration_method;
using defaults::integration_parameters;
using defaults::num_t;
using defaults::step_result_handler;
using defaults::vector_t;



template <integration_method Method, ode_system SystemT> class integrator {
    GRACE_DEFAULT_VECTOR_T_OWNER_CONSTRUCTORS(integrator)
  public:

    template <ode_system S>
    integrator(S && system, num_t t_0, num_t t_end, num_t dt, const vector_t & y_0)
    try :
        integrator(std::forward<S>(system), integration_parameters{ t_0, t_end, dt }, y_0) {
    }
    catch (const integration_parameters::invalid_argument & e) {
        rethrow_with_construction_context(e.raw_info());
    }
    catch (...) { rethrow_with_construction_context(); }



    template <ode_system S>
    integrator(S && system, integration_parameters params, const vector_t & y_0)
    try :
          // Constants
          _n_size{ y_0.extent(0) },
          _parameters{ std::move(params) },
          _system{ std::forward<S>(system) },
          // Workers
          _method(_n_size, _parameters),
          _t{ _parameters.t_0() },
          _y("y", _n_size) {

        if (_n_size <= 0)
            throw std::invalid_argument("y_0 must have size > 0, got " +
                std::to_string(_n_size));

        Kokkos::deep_copy(_y, y_0);
    }
    catch (...) { rethrow_with_construction_context(); }



    Kokkos::View<const double *> tie_step_result() const noexcept { return _y; }



    bool step(Kokkos::View<const double *> & step_result) {
        compute_step();
        step_result = _y;
        return _t < _parameters.t_end();
    }


    bool step() {
        compute_step();
        return _t < _parameters.t_end();
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
    [[noreturn]] static void rethrow_with_construction_context(const char * msg = nullptr) {
        try {
            throw;
        }
        catch (const std::exception & e) {
            throw std::runtime_error("integrator construction: " + std::string(msg ? msg : e.what()));
        }
        catch (...) {
            throw std::runtime_error("integrator construction: fail");
        }
    }

    void compute_step() noexcept { _method.step(_system, _y, _t); }


    size_t                 _n_size;
    integration_parameters _parameters;

    SystemT _system;
    Method  _method;

    num_t    _t;
    vector_t _y;
};


} // namespace Grace::integration



#endif // GRACE_INTEGRATION_INTEGRATION_HPP