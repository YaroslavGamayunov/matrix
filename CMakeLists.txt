cmake_minimum_required(VERSION 3.14)
project(Matrix)

set(CMAKE_CXX_STANDARD 17)

add_executable(Matrix tests/main.cpp include/Rational.h include/Finite.h include/BigInteger.h)