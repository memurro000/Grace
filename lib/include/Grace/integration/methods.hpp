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


#include "defaults.hpp"
#include "functional.hpp"

namespace Grace::integration::methods
{

    using defaults::num_t;
    using defaults::vector_t;
    using defaults::integration_method;
    using defaults::integration_parameters;

    using functional::linear_combination;
    using functional::reconsider_solution;




    // Methods

    class RK4 {
    public:
        GRACE_DEFAULT_VECTOR_T_OWNER(RK4)


        RK4(size_t n_size, integration_parameters parameters) :
            _parameters(parameters),
            _dt{parameters._dt},
            _half_dt{_dt / 2.0},
            _k1    ("k1"    , n_size),
            _k2    ("k2"    , n_size),
            _k3    ("k3"    , n_size),
            _k4    ("k4"    , n_size),
            _y_temp("y_temp", n_size)
        {}


        template <function_system SystemT>
        void step(SystemT&& system, vector_t& y, num_t& t) {
            if (t + _dt > _parameters._t_end) {
                _dt = _parameters._t_end - t;
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




    // Traits


    template <typename Method>
    struct method_traits;

    template <typename Method>
    concept described_method = integration_method<Method> && 
        requires {
            typename method_traits<Method>;
            { method_traits<Method>::order             } -> std::convertible_to<int>;
            { method_traits<Method>::convergence_order } -> std::convertible_to<int>;
        };

    template <>
    struct method_traits<RK4> {
        static constexpr int order = 4;
        static constexpr int convergence_order = 4;
    };




    static_assert(described_method<RK4>);


} // namespace Grace::integration::methods







#endif // GRACE_INTEGRATION_METHODS_HPP