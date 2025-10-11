#include <benchmark/benchmark.h>

#include <Grace/Grace.hpp>


namespace Grace::integrator_with_systems
{
    
    static void
    BM_integrate_harmonic_oscillator(benchmark::State& state) {
        //num_t dt = 1.0 / static_cast<num_t>(state.range(0));

        //Grace::vector_t init{"BM_integrate_init", 2};
        //init(0) = 1.0;
        //init(1) = 0.0;
        //auto functor = Grace::systems::harmonic_oscillator(1.0);
        //auto stats = Grace::RK4::statistics{"harmonic_oscillator"};

        //for (auto _ : state) {
        //    auto integrated = Grace::RK4::integrate(
        //        Grace::systems::harmonic_oscillator(1.0), init, 0.0, 12.0, dt, stats
        //    );
        //    Kokkos::fence();
        //    benchmark::DoNotOptimize(integrated);
        //}
    }


    BENCHMARK(Grace::integrator_with_systems::BM_integrate_harmonic_oscillator)
        ->Arg(1)
        ->Arg(10)
        ->Unit(benchmark::kMicrosecond);

    BENCHMARK(Grace::integrator_with_systems::BM_integrate_harmonic_oscillator)
        ->Arg(100)->Iterations(12)->Unit(benchmark::kMicrosecond);

    BENCHMARK(Grace::integrator_with_systems::BM_integrate_harmonic_oscillator)
        ->Arg(1000)->Iterations(4)->Unit(benchmark::kMicrosecond);

} // namespace Grace::integrator_with_systems


int main(int argc, char* argv[]) {
    Grace::initialize(argc, argv);
    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) {
        Grace::finalize();
        return 1;
    }


    benchmark::RunSpecifiedBenchmarks();





    benchmark::Shutdown();
    Grace::finalize();
    return 0;
}

