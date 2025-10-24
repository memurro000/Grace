# Grace

High-performance RK4 ODE integrator in C++20 with Kokkos parallelization

## Features

- RK4 method with Kokkos parallelization
- Modern C++20 interface
- Extensible system architecture

## Getting started

### Quick start

Here's a minimal example that integrates a harmonic oscillator with trajectory output 
using the RK4 method:

```cpp
// main.cpp
#include <Grace/Grace.hpp>
#include <Grace/integration/factories.hpp>
#include <iostream>


void print_vector_of_2(Kokkos::View<const double *> what) {
    std::cout << what(0) << "  \t" << what(1) << std::endl;
}


int main(int argc, char * argv[]) {
    using namespace Grace::integration;
    using namespace Grace;



    runtime::context context(argc, argv); // Initialize Grace runtime context



    vector_t y_0("y_0", 2); // Prepare initial conditions
    y_0(0) = 1.0;           // for integration of ODE system
    y_0(1) = 0.0;           // with size == 2 for example



    auto an_integrator = factories::make_integrator<methods::RK4>( // Create an RK4 integrator
        systems::harmonic_oscillator(1.0), // to integrate harmonic oscillator system
        0.0, 12.5, 0.1,                    // on interval from 0.0 to 12.5 with dt == 0.1
        y_0);                              // You can bring your systems fulfilling a concept



    std::cout << "# x \t\t v\n"; // Mark columns for output clarity

    auto y = an_integrator.integrate(print_vector_of_2); // Perform the integration
                                                         // using a functor of any kind
                                                         // for trajectory processing (optional)
}
// There are more different convenient ways
// of the integration processing in the library
```

Output:

```dat
# x              v
0.995004        -0.0998333
0.980067        -0.198669
0.955337        -0.29552
0.921061        -0.389418
...
0.893201        0.449656
0.933629        0.358238
0.964729        0.263241
0.98619         0.165614
0.997797        0.0663322
```

## License

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    <http://www.apache.org/licenses/LICENSE-2.0>

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
