//
// Created by Ярослав Гамаюнов on 2020-02-22.
//

#ifndef MATRIX_FINITE_TEST_FIXTURE_H
#define MATRIX_FINITE_TEST_FIXTURE_H

#include <gtest/gtest.h>
#include <random>
#include "../src/math_utils.h"

class FiniteTestFixture : public ::testing::Test {
public:
    std::vector<unsigned> primeNumbers;
    std::vector<unsigned> compositeNumbers;

    enum Operation {
        ADD, SUB, MUL, OP_NUMBER
    };

    template<unsigned M>
    void testModPow(std::vector<unsigned> numbersForTest = {}) {
        if (numbersForTest.empty()) {
            testModPow<M>(primeNumbers);
            testModPow<M>(compositeNumbers);
        } else {
            std::mt19937 rnd;
            for (unsigned a : numbersForTest) {
                Finite<M> number(a);
                unsigned power = rnd();

                ASSERT_EQ(Finite<M>::pow(number, power).getValue(), modPow(a, power, M));
            }
        }
    }

    template<unsigned M>
    void testBasicOperations(std::vector<unsigned> numbersForTest = {}) {
        if (numbersForTest.empty()) {
            testBasicOperations<M>(primeNumbers);
            testBasicOperations<M>(compositeNumbers);
        } else {
            std::mt19937 rnd;
            for (unsigned a : numbersForTest) {
                Finite<M> leftOperand(a);
                Finite<M> rightOperand(rnd());

                for (int t = 0; t < 100; t++) {
                    Operation operation = Operation(rnd() % OP_NUMBER);

                    switch (operation) {
                        case ADD: {
                            ASSERT_EQ((leftOperand + rightOperand).getValue(),
                                      modAdd(leftOperand.getValue(), rightOperand.getValue(), M));
                        }
                        case SUB: {
                            if ((leftOperand - rightOperand).getValue() !=
                                modSub(leftOperand.getValue(), rightOperand.getValue(), M)) {
                                auto res = leftOperand - rightOperand;
                                std::cout << "djfjf";
                            }
                            ASSERT_EQ((leftOperand - rightOperand).getValue(),
                                      modSub(leftOperand.getValue(), rightOperand.getValue(), M));
                        }
                        case MUL: {
                            ASSERT_EQ((leftOperand * rightOperand).getValue(),
                                      modMul(leftOperand.getValue(), rightOperand.getValue(), M));
                        }
                    }
                }
            }
        }
    }

protected:

    // Here we generate about 2000 prime numbers and 2000 composite numbers
    void SetUp() override {
        primeNumbers = findPrimesOnSegment(10000);

        // Adding big primes
        primeNumbers.push_back(1'000'000'007);
        for (int i = 0; i < 1000; i++) {
            primeNumbers.push_back(findNextPrime(primeNumbers.back()));
        }

        std::mt19937 rnd;
        while (compositeNumbers.size() < 1000) {
            unsigned num = rnd() % 1000;
            if (!isPrime(num)) {
                compositeNumbers.push_back(num);
            }
        }

        while (compositeNumbers.size() < 1000) {
            unsigned num = rnd();
            if (!isPrime(num)) {
                compositeNumbers.push_back(num);
            }
        }
    }
};

#endif //MATRIX_FINITE_TEST_FIXTURE_H
