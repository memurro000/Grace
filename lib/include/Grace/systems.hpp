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

#ifndef RK4_FUNCTIONS_HPP
#define RK4_FUNCTIONS_HPP

#include "defaults.hpp"



namespace Grace::systems {
    using namespace defaults::param;
    using namespace defaults;




    class harmonic_oscillator {
    public:

        harmonic_oscillator(num_t omega) : _omega{omega} {}


        vector_out operator()(num_t t, vector_in y) const {
            vector derivative("derivative", 2);

            derivative(0) = y(1);
            derivative(1) = -_omega * _omega * y(0);

            return derivative;
        }


    private:
        num_t _omega;


    };  
    static_assert(function_system<harmonic_oscillator>, 
            "harmonic_oscillator must fulfill function_system concept");







} // namespace RK4::functions








#endif // RK4_FUNCTIONS