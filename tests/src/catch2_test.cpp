
#include <catch2/catch_test_macros.hpp>

unsigned int factorial(unsigned int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

TEST_CASE("factorials are computed", "[factorial]") {
    REQUIRE(factorial(0) == 1);
    REQUIRE(factorial(1) == 1);
    REQUIRE(factorial(2) == 2);
    REQUIRE(factorial(3) == 6);
    REQUIRE(factorial(10) == 3628800);
}

