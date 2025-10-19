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

#include "defaults.hpp"



namespace Grace::integration {
    using defaults::num_t;
    using defaults::vector_t;
    using defaults::function_system;
    using defaults::integration_method;
    using defaults::step_result_handler;
    using defaults::integration_parameters;



    template <integration_method Method, function_system SystemT>
    class integrator {
    public:
        GRACE_DEFAULT_VECTOR_T_OWNER(integrator)

        template <typename S = SystemT>
        integrator(
            S&& system,
            num_t t_0, num_t t_end, num_t dt,
            const vector_t& y_0
        ) :
            // Constants
            _system{std::forward<S>(system)}, _n_size{y_0.extent(0)},
            _parameters{t_0, t_end, dt}, 
            // Workers
            _t{t_0},
            _y("y", _n_size),
            _method(_n_size, _parameters)
        {
            // Workers
            Kokkos::deep_copy(_y, y_0);
        }


        Kokkos::View<const double*> tie_step_result() const {
            return _y;
        }


        bool step(Kokkos::View<const double*>& step_result) {
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
        Kokkos::View<const double *> integrate(Handler&& handler) {
            auto step_result = tie_step_result();
            while (step()) {
                handler(step_result);
            }
            return step_result; 
        }


    private:

        void compute_step() {
            _method.step(_system, _y, _t);
        }


        size_t _n_size;
        integration_parameters _parameters;


        Method _method;
        SystemT _system;


        num_t _t;
        vector_t _y;
    };


} // namespace Grace::integration








#endif // GRACE_INTEGRATION_INTEGRATION_HPP