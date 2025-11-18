#ifndef GRACE_INTEGRATION_METHODS_METHODS_HPP
#define GRACE_INTEGRATION_METHODS_METHODS_HPP



#include <Grace/integration/methods.hpp>
#include <gtest/gtest.h>


namespace Grace::integration::methods {

    using TestedMethodTypes = ::testing::Types<RK4>;


    class TestedMethodTypeNames {
      public:
        template <typename T>
        static std::string GetName() {
            if constexpr (std::is_same_v<T, RK4>)
                return "RK4";

            return "UnknownMethod";
        }
    };



}





#endif // GRACE_INTEGRATION_METHODS_METHODS_HPP