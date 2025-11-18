#ifndef SIMPLE_TESTED_METHOD_TYPES_HPP
#define SIMPLE_TESTED_METHOD_TYPES_HPP

#include <gtest/gtest.h>

namespace Grace::integration::methods {

    // Simple dummy type for testing
    struct DummyMethod {};
    
    using SimpleTestedMethodTypes = ::testing::Types<DummyMethod>;

}

#endif // SIMPLE_TESTED_METHOD_TYPES_HPP