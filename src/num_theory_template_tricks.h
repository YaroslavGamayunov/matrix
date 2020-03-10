//
// Created by Ярослав Гамаюнов on 2020-02-22.
//

#ifndef MATRIX_NUM_THEORY_TEMPLATE_TRICKS_H
#define MATRIX_NUM_THEORY_TEMPLATE_TRICKS_H

template<bool EXPR, typename T, typename F>
struct TernaryOperator {
    static const auto value = T::value;
};

template<typename T, typename F>
struct TernaryOperator<false, T, F> {
    static const auto value = F::value;
};

template<bool V>
struct BooleanValue {
    enum {
        value = V
    };
};

template<int V>
struct IntValue {
    static const auto value = V;
};

template<unsigned N, unsigned M>
struct HasNonTrivialDivisors {
    enum {
        value = TernaryOperator<(N % M == 0), BooleanValue<false>,
                TernaryOperator<(M * M <= N), HasNonTrivialDivisors<N, M + 1>, BooleanValue<true>>>::value
    };
};

// This structure was created for compile-time prime number check
template<unsigned N>
struct IsPrime {
    enum {
        value = TernaryOperator<(N < 2), BooleanValue<false>,
                TernaryOperator<(N == 2), BooleanValue<true>, HasNonTrivialDivisors<N, 2>>>::value
    };
};

// finds first 2^k >= N
/*
 * Note: this template can exceed max template instantiation limit,
 * but I hope users will not need to create 50'000 x 50'000 matrices :)
 */
template<int N>
struct NextPowerOfTwo {
    static const int value = TernaryOperator<((N & (N - 1)) == 0), IntValue<N>, NextPowerOfTwo<N + 1>>::value;
};

#define IS_PRIME(N) IsPrime<N>::value
#define NEXT_POWER_OF_TWO(N) NextPowerOfTwo<N>::value

#endif //MATRIX_NUM_THEORY_TEMPLATE_TRICKS_H
