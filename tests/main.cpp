#include <cstdint>
#include <iostream>
#include <gtest/gtest.h>
#include "../src/compile_time_assert.h"
#include "../src/num_theory_template_tricks.h"
#include "../include/Finite.h"
#include "FiniteTestFixture.h"


TEST_F(FiniteTestFixture, FiniteTest_Power_Test) {

    // small prime modulo
    testModPow<2>();
    testModPow<3>();
    testModPow<5>();
    testModPow<71>();
    testModPow<1109>();
    testModPow<10159>();

    // big prime modulo
    testModPow<1000000007>();
    testModPow<1000000009>();
    testModPow<2006000099>();

    // small composite modulo
    testModPow<4>();
    testModPow<6>();
    testModPow<134>();
    testModPow<2395>();
    testModPow<20400>();
    testModPow<20409>();

    // big composite modulo
    testModPow<2383881>();
    testModPow<2083881914>();
    testModPow<1000000000>();
}

TEST_F(FiniteTestFixture, FiniteTest_BasicOperations_Test) {

    // small prime modulo
    testBasicOperations<2>();
    testBasicOperations<3>();
    testBasicOperations<5>();
    testBasicOperations<71>();
    testBasicOperations<1109>();
    testBasicOperations<10159>();

    // big prime modulo
    testBasicOperations<1000000007>();
    testBasicOperations<1000000009>();
    testBasicOperations<2006000099>();

    // small composite modulo
    testBasicOperations<4>();
    testBasicOperations<6>();
    testBasicOperations<134>();
    testBasicOperations<2395>();
    testBasicOperations<20400>();
    testBasicOperations<20409>();

    // big composite modulo
    testBasicOperations<2383881>();
    testBasicOperations<2083881914>();
    testBasicOperations<1000000000>();
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}