#ifndef TESTS_FIXTURES_HPP
#define TESTS_FIXTURES_HPP

#include <Grace/defaults.hpp>
#include <Grace/RK4.hpp>
#include <Grace/systems.hpp>


#include <memory>


namespace lib_testing {

    using namespace Grace::defaults;

    struct IntegratorBasicTestConfiguration {
        size_t _n_size;
        size_t _n_steps;

        num_t _dt;
        std::string name;
        function_system_t system;

        vector_t _y_0;
        vector_t y_expected_result;


        IntegratorBasicTestConfiguration& n_size (size_t is) { _n_size = is;  return *this; }
        IntegratorBasicTestConfiguration& n_steps(size_t is) { _n_steps = is; return *this; }
        IntegratorBasicTestConfiguration& dt     (num_t  is) { _dt = is;      return *this; }
        

        std::string to_string() const {
            std::stringstream stream;
            stream << "IntegratorBasicTestConfiguration{"
               << "descriptor: " << descriptor()
               << ", size: " << _n_size
               << ", steps: " << _n_steps
               << ", _dt: " << _dt
               << ", expected: [";
            for (size_t i = 0; i < std::min(size_t{3}, _n_size); ++i) {
                if (i > 0) stream << ", ";
                stream << y_expected_result(i);
            }
            if (_n_size > 3) stream << ", ...";
            stream << "]}";
            return stream.str();
        }

        std::string descriptor() const {
            return name + "_NSize" + std::to_string(_n_size ) + 
                         "_NSteps" + std::to_string(_n_steps);
        }
    };

    std::ostream& operator<<(std::ostream& stream, const IntegratorBasicTestConfiguration& config) {
        return stream << config.to_string();
    }



    class IntegratorBasicTest : public testing::TestWithParam<IntegratorBasicTestConfiguration> {
    public:
        using integrator_t = Grace::RK4::integrator<Grace::defaults::function_system_t>;

    protected:
        void SetUp() override {
            config = GetParam();
            integrator = std::make_unique<integrator_t>(config.system, config._y_0, config._dt);
        }


        IntegratorBasicTestConfiguration config;
        std::unique_ptr<integrator_t> integrator;

    };


} // namespace lib_testing



#endif // TESTS_FIXTURES_HPP