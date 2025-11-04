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

#ifndef GRACE_INTEGRATION_DEFAULTS_HPP
#define GRACE_INTEGRATION_DEFAULTS_HPP


#include "../defaults.hpp"
#include <initializer_list>
#include <stdexcept>
#include <string>


#ifndef GRACE_DEFAULT_VECTOR_T_OWNER_CONSTRUCTORS
#    define GRACE_DEFAULT_VECTOR_T_OWNER_CONSTRUCTORS(class_name)         \
        /* vector_t ownership and no explicit copy semantics specified */ \
      private:                                                            \
        class_name(const class_name &)             noexcept = default;    \
        class_name & operator=(const class_name &) noexcept = default;    \
      public:                                                             \
        class_name(class_name &&)                  noexcept = default;    \
        class_name & operator=(class_name &&)      noexcept = default;    \
        ~class_name()                              noexcept = default;
#endif // GRACE_DEFAULT_VECTOR_T_OWNER_CONSTRUCTORS



namespace Grace::integration::defaults {
using Grace::defaults::ode_system;
using Grace::defaults::ode_system_t;
using Grace::defaults::num_t;
using Grace::defaults::vector_t;
namespace parametric_vector {
    using namespace Grace::defaults::parametric_vector;
}



// Immutable value type to store integration constraints
// in the instances of integrator and method policies
class integration_parameters {
  public:
    class invalid_argument : public std::invalid_argument {
    // TODO consider refactoring
      public:
        explicit invalid_argument(const std::string & msg)
            : std::invalid_argument("integration_parameters construction: " + msg), _raw_info(msg) {}

        const char * raw_info() const noexcept {
            return _raw_info.c_str();
        }

      private:
        std::string _raw_info;
    };

    integration_parameters(num_t t_0, num_t t_end, num_t dt)
        : _t_0(t_0), _t_end(t_end), _dt(dt) {
        validate();
    }

    integration_parameters(std::initializer_list<num_t> list) {
        validate_list_size(list.size());
        auto it = list.begin();
        _t_0 = *it++;
        _t_end = *it++;
        _dt = *it;
        validate();
    }

    num_t t_0() const { return _t_0; }
    num_t t_end() const { return _t_end; }
    num_t dt() const { return _dt; }

  private:
    num_t _t_0, _t_end, _dt;

    void validate() const {
        if (_dt <= 0)
            throw invalid_argument(
                "dt must be > 0, got "
                + std::to_string(_dt));
        if (_t_end <= _t_0)
            throw invalid_argument(
                "t_0 must be < t_end, got "
                + std::to_string(_t_0) + " and " + std::to_string(_t_end));
        if (_dt >= _t_end - _t_0)
            throw invalid_argument(
                "dt must be < t_end - t_0, got "
                + std::to_string(_dt) + ", " + std::to_string(_t_end) + " - "
                + std::to_string(_t_0));
    }

    void validate_list_size(size_t size) const {
        if (size != 3)
            throw invalid_argument(
                "expected 3 values {t_0, t_end, dt}, got "
                + std::to_string(size));
    }
};



// A concept for policies used to select one of the integration methods
template <typename Policy, typename FunctionSystemT>
concept integration_method_for =
    ode_system<FunctionSystemT> &&
    requires(Policy method, FunctionSystemT & system, vector_t & y,
             num_t & t) { // step method constraint
        { method.step(system, y, t) } -> std::same_as<void>;
    } &&
    requires(size_t n_size, integration_parameters parameters) { // constructor constraint
        { Policy(n_size, parameters) } -> std::same_as<Policy>;
    };

template <typename Policy>
concept integration_method = integration_method_for<Policy, ode_system_t>;



// A concept for a functor used to handle intermediate result of integration
// by instances of integrator class
template <typename FunctorT>
concept step_result_handler = requires(FunctorT && t, Kokkos::View<const double *> step_result) {
    { t(step_result) } -> std::same_as<void>;
};


} // namespace Grace::integration::defaults



#endif // GRACE_INTEGRATION_DEFAULTS_HPP