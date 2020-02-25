//
// Created by Ярослав Гамаюнов on 2020-02-18.
//

#ifndef MATRIX_FINITE_H
#define MATRIX_FINITE_H

#include "../src/compile_time_assert.h"
#include "../src/num_theory_template_tricks.h"

template<unsigned M>
class Finite {
public:
    static Finite pow(const Finite &a, unsigned n) {
        if (n == 0) {
            return Finite(1);
        }

        if (n % 2 == 0) {
            Finite res = pow(a, n / 2);
            return res * res;
        }

        return pow(a, n - 1) * a;
    }

    Finite divideModulo(Finite<M> &other) {
        COMPILE_ASSERT(IS_PRIME(M));
        return (*this) * other.getMulInverse();
    }

    Finite(const Finite<M> &other) {
        this->value = other.value;
    }

    Finite &operator=(const Finite<M> &other) {
        this->value = other.value;
        return *this;
    }

    explicit Finite(unsigned x) {
        this->value = x % M;
    }

    Finite &operator+=(const Finite<M> &other) {
        long long result = (long long) value + (long long) other.value;
        if (result >= M) {
            result -= M;
        }
        value = result;
        return *this;
    }

    Finite &operator*=(const Finite<M> &other) {
        long long result = ((long long) value * (long long) other.value) % M;
        value = result;
        return *this;
    }

    Finite &operator-=(const Finite<M> &other) {
        long long result = (((long long) value - (long long) other.value) % M + M) % M;
        value = result;
        return *this;
    }

    Finite getMulInverse() {
        COMPILE_ASSERT(IS_PRIME(M))
        return pow(*this, M - 2);
    }

    unsigned getValue() {
        return value;
    }

private:
    unsigned value;
};

template<unsigned M>
Finite<M> operator+(const Finite<M> &a, const Finite<M> &b) {
    Finite<M> result = a;
    result += b;
    return result;
}

template<unsigned M>
Finite<M> operator-(const Finite<M> &a, const Finite<M> &b) {
    Finite<M> result = a;
    result -= b;
    return result;
}

template<unsigned M>
Finite<M> operator*(const Finite<M> &a, const Finite<M> &b) {
    Finite<M> result = a;
    result *= b;
    return result;
}


#endif //MATRIX_FINITE_H