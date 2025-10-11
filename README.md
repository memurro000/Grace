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
  Grace::initialize(argc, argv);


  vector_t initial_conditions("initial_conditions", 2);
  initial_conditions(0) = 1.0;
  initial_conditions(1) = 0.0;

  auto system = Grace::systems::harmonic_oscillator(1.0);
  auto grace = Grace::RK4::integrator<function_system_t>(system, initial_conditions).dt(0.0001);


  vector_t integrated("integrated", 2);
  for (size_t i = 0; i < 100000; ++i) {
    grace.step();
  }
  integrated = grace.step();


  std::cout << "x: " << integrated(0) << std::endl <<
               "v: " << integrated(1) << std::endl;


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
