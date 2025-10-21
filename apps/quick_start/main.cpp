#include <Grace/Grace.hpp>
#include <Grace/integration/factories.hpp>


#include <iostream>


void print_vector_of_2(Kokkos::View<const double *> what) {
    std::cout << what(0) << "  \t" << what(1) << std::endl;
}


int main(int argc, char* argv[]) {
    using namespace Grace::integration;
    using namespace Grace;



    runtime::context context(argc, argv); // Initialize Grace runtime context


    
    vector_t y_0("y_0", 2);     // Prepare initial conditions 
    y_0(0) = 1.0;               // for integration of ODE system 
    y_0(1) = 0.0;               // with size == 2 for example
    

    

    auto an_integrator = 
        factories::make_integrator<methods::RK4>(   // Create an RK4 integrator
            systems::harmonic_oscillator(1.0),      // to integrate harmonic oscillator system
            0.0, 12.5, 0.1,                         // on interval from 0.0 to 12.5 with dt == 0.1   
            y_0);                                   // You can bring your systems fulfilling a concept                 




    std::cout << "# x \t\t v\n";                            // Mark columns for output clarity

    auto y = an_integrator.integrate(print_vector_of_2);    // Perform the integration
                                                            // using a functor of any kind
                                                            // for trajectory processing (optional)

}
        // There are more different convenient ways 
        // of the integration processing in the library