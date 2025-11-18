#include <gtest/gtest.h>
#include <iostream>

TEST(MinimalTest, BasicTest) {
    EXPECT_TRUE(true);
}

int main(int argc, char* argv[]) {
    std::cout << "Starting minimal test..." << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "Google Test initialized" << std::endl;
    int result = RUN_ALL_TESTS();
    std::cout << "Tests completed with result: " << result << std::endl;
    return result;
}