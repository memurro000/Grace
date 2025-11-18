#ifndef GRACE_TEST_ENVIRONMENT_HPP
#define GRACE_TEST_ENVIRONMENT_HPP

#include <Grace/runtime.hpp>
#include <gtest/gtest.h>

class GraceTestEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        Grace::runtime::initialize(0, nullptr);
    }
    
    void TearDown() override {
        Grace::runtime::finalize();
    }
};

// Register the environment
static ::testing::Environment* const grace_env = 
    ::testing::AddGlobalTestEnvironment(new GraceTestEnvironment);

#endif // GRACE_TEST_ENVIRONMENT_HPP