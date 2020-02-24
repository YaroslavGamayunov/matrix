//
// Created by Ярослав Гамаюнов on 2020-02-22.
//

#ifndef MATRIX_NUM_THEORY_TEMPLATE_TRICKS_H
#define MATRIX_NUM_THEORY_TEMPLATE_TRICKS_H

template<bool EXPR, typename T, typename F>
struct TernaryOperator {
    enum {
        value = T::value
    };
};

template<typename T, typename F>
struct TernaryOperator<false, T, F> {
    enum {
        value = F::value
    };
};

template<bool V>
struct BooleanValue {
    enum {
        value = V
    };
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

#define IS_PRIME(N) IsPrime<N>::value

#endif //MATRIX_NUM_THEORY_TEMPLATE_TRICKS_H
