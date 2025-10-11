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
    integrated = grace.step();
  }



  std::cout << "x: " << integrated(0) << std::endl <<
               "v: " << integrated(1) << std::endl;


  Grace::finalize();
}
```
