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

#ifndef GRACE_RUNTIME_HPP
#define GRACE_RUNTIME_HPP

#include <Kokkos_Core.hpp>

namespace Grace::runtime {


class context {
  public:
    context(int argc, char * argv[]) { Kokkos::initialize(argc, argv); }


    ~context() { Kokkos::finalize(); }

    context(context &&)                  = default;
    context & operator=(context &&)      = default;
    context(const context &)             = delete;
    context & operator=(const context &) = delete;
};

inline void initialize(int argc, char * argv[]) { Kokkos::initialize(argc, argv); }

inline void finalize() { Kokkos::finalize(); }


} // namespace Grace::runtime


#endif // GRACE_RUNTIME_HPP