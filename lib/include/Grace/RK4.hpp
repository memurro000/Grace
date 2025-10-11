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

#ifndef GRACE_RK4_HPP
#define GRACE_RK4_HPP



#include "functional.hpp"



namespace Grace::RK4 {
    using namespace functional;
    using namespace defaults;







    template <function_system System>
    class integrator {
    public:

        template <typename S = System>
        integrator(
            S&& system, vector_in y_initial, 
            num_t dt = 0.0001, num_t t_0 = 0.0
            ) :
            _system(std::forward<S>(system)),
            _t{t_0},
            _dt{dt}, _half_dt{dt / 2},

            _size{y_initial.extent(0)},
            _y     ("y"     , _size),
            _k1    ("k1"    , _size),
            _k2    ("k2"    , _size),
            _k3    ("k3"    , _size),
            _k4    ("k4"    , _size),
            _y_temp("y_temp", _size) {
            Kokkos::deep_copy(_y, y_initial);
        }

        integrator& limits(num_t left) { _t = left; return *this; }
        integrator& dt(num_t dt) { _dt = dt; return *this; }


        vector step() {
            _k1 = _system(_t, _y);
            linear_combination(_y_temp, _y, _half_dt, _k1);

            _k2 = _system(_t + _half_dt, _y_temp);
            linear_combination(_y_temp, _y, _half_dt, _k2);

            _k3 = _system(_t + _half_dt, _y_temp);
            linear_combination(_y_temp, _y, _dt, _k3);

            _k4 = _system(_t + _dt, _y_temp);

            reconsider_solution(_y, _y, _k1, _k2, _k3, _k4, _dt);

            return _y;
        }



    private:
        size_t _size;
        size_t _steps_needed;
        num_t _dt;
        num_t _half_dt;
        System _system;

        num_t _t;

        vector _y;       
        vector _k1;
        vector _k2;
        vector _k3;
        vector _k4;
        vector _y_temp;
    };








    // DEPRECATED
    inline
    void verify_integrate_parameters(num_t t_0, num_t t_end, num_t dt) {
        using std::to_string;

        if (dt <= 0.0) {
            throw std::invalid_argument(
                std::string("RK4::vector RK4::integrate needs dt > 0, got ") +
                to_string(dt)
            );
        }

        if (t_end <= t_0) {
            throw std::invalid_argument(
                std::string("RK4::vector RK4::integrate needs t_end > t_0, got ") +
                to_string(t_end) + " and " + to_string(t_0)
            );
        }
    }

};





#endif // GRACE_RK4_HPP