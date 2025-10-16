# Grace
High-performance RK4 ODE integrator in C++20 with Kokkos parallelization

## Features
- RK4 method with Kokkos parallelization
- Modern C++20 interface
- Extensible system architecture

## Quick Start
```cpp
#include <Grace/defaults.hpp>
#include <Grace/Grace.hpp>
#include <Grace/RK4.hpp>
#include <Grace/systems.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
    using namespace Grace::defaults;
    using Grace::systems::harmonic_oscillator;
    using Grace::RK4::make_integrator;

    Grace::initialize(argc, argv);


    vector_t initial_conditions("initial_conditions", 2);
    initial_conditions(0) = 1.0;
    initial_conditions(1) = 0.0;

    auto system = harmonic_oscillator(1.0);
    auto grace  = make_integrator(system, 0.0, 12.5, 0.0001, initial_conditions);
    vector_t current("current", 2);


    while (grace.step(current));


    std::cout << "x: " << current(0) << std::endl <<
                 "v: " << current(1) << std::endl;


    Grace::finalize();
}
```




## License

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.




## Dependencies

### Runtime
- **Kokkos** (BSD-3-Clause) - Parallel programming ecosystem
  - Automatically fetched via CMake FetchContent - not yet
  - Used for GPU/CPU parallelization

### Testing & Benchmarking
- **Google Test** (BSD-3-Clause) - C++ testing framework
- **Google Benchmark** (Apache-2.0) - Microbenchmarking library
