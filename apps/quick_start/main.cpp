#include <Grace/Grace.hpp>
#include <Grace/integration/factories.hpp>
#include <Grace/systems.hpp>


#include <iostream>


int main(int argc, char* argv[]) {
    using namespace Grace::integration;

    Grace::context context(argc, argv); // Initialize Grace runtime context

    vector_t y_0("y_0", 2); // Prepare initial conditions 
    y_0(0) = 1.0;           // for integration of ODE system with size == 2 for example
    y_0(1) = 0.0;
    
    
    auto an_integrator = factories::make_integrator<methods::RK4>(  // Create an RK4 integrator
        Grace::systems::harmonic_oscillator(1.0),   // to integrate harmonic oscillator system
        0.0, 12.5, 0.1,                             // on interval from 0.0 to 12.5 with dt == 0.001   
        y_0);                                       // You can bring your systems fulfilling a concept                 

    

    std::cout << "# x \t\t v\n";                    // Mark columns for output


    auto y = an_integrator.integrate(               // Peform the integration
        [](const auto y) {                          // using lambda
            std::cout << y(0) << "  \t" <<          // for trajectory output
                         y(1) << '\n';              // with the required formatting
        }
    );                                              // There are more different convenient ways 
                                                    // of the integration processing in the library

    

}