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
    const static Finite ZERO;
    const static Finite ONE;

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

    Finite getMulInverse() const {
        COMPILE_ASSERT(IS_PRIME(M));
        return pow(*this, M - 2);
    }

    Finite(const Finite<M> &other) {
        this->value = other.value;
    }

    explicit Finite(unsigned x) {
        this->value = ((x % M) + M) % M;
    }

    Finite() {
        this->value = 0;
    }

    Finite &operator/=(const Finite<M> &other) {
        COMPILE_ASSERT(IS_PRIME(M));
        *this = *this * other.getMulInverse();
        return *this;
    }

    Finite operator-() {
        return Finite(0) - *this;
    }

    Finite &operator=(const Finite<M> &other) {
        this->value = other.value;
        return *this;
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

    bool operator==(const Finite<M> &other) const {
        return value == other.value;
    }

    bool operator!=(const Finite<M> &other) {
        return value != other.value;
    }

    unsigned getValue() const {
        return value;
    }

private:
    unsigned value = 0;
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

template<unsigned M>
Finite<M> operator/(const Finite<M> &a, const Finite<M> &b) {
    Finite<M> result = a;
    result /= b;
    return result;
}

template<unsigned M>
const Finite<M> Finite<M>::ZERO = Finite<M>(0);

template<unsigned M>
const Finite<M> Finite<M>::ONE = Finite<M>(1);

#endif //MATRIX_FINITE_H