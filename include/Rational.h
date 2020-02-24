//
// Created by Ярослав Гамаюнов on 2020-02-18.
//

#ifndef MATRIX_RATIONAL_H
#define MATRIX_RATIONAL_H

#include <algorithm>
#include "BigInteger.h"

class Rational : public Field {
private:
    BigInteger numerator;
    BigInteger denominator;

    void reduce() {
        BigInteger a = abs(numerator);
        BigInteger b = abs(denominator);
        while (b > 0) {
            a %= b;
            std::swap(a, b);
        }
        numerator /= a;
        denominator /= a;
        if (denominator < 0) {
            numerator *= -1;
            denominator *= -1;
        }
    }

public:
    Rational() : numerator(0), denominator(1) {}

    Rational(const Rational &q) = default;

    Rational(const BigInteger &b) : numerator(b), denominator(1) { reduce(); }

    Rational(int n) : Rational(BigInteger(n)) {}

    Rational(const BigInteger &a, const BigInteger &b) : numerator(a), denominator(b) { reduce(); }

    std::string toString() const {
        if (abs(denominator) == 1) {
            return (numerator * denominator).toString();
        }
        if (denominator < 0) {
            return (-numerator).toString() + "/" + (-denominator).toString();
        }
        return numerator.toString() + "/" + denominator.toString();
    }

    Rational &operator=(const Rational &other) = default;

    explicit operator double() const {
        return atof(asDecimal(100).c_str());
    }

    Rational &operator+=(const Rational &other) {
        numerator = numerator * other.denominator + denominator * other.numerator;
        denominator = denominator * other.denominator;
        reduce();
        return *this;
    }

    Rational &operator-=(const Rational &other) {
        numerator = numerator * other.denominator - denominator * other.numerator;
        denominator = denominator * other.denominator;
        reduce();
        return *this;
    }

    Rational &operator*=(const Rational &other) {
        numerator *= other.numerator;
        denominator *= other.denominator;
        reduce();
        return *this;
    }

    Rational &operator/=(const Rational &other) {
        numerator *= other.denominator;
        denominator *= other.numerator;
        reduce();
        return *this;
    }

    Rational operator-() {
        Rational res(*this);
        res.numerator *= -1;
        return res;
    }

    std::string asDecimal(size_t precision = 0) const {
        BigInteger a = numerator;
        BigInteger b = denominator;

        int sign = a.getSign() * b.getSign();

        a = abs(numerator);
        b = abs(denominator);

        int cnt = 0;

        if (a != 0) {
            while (a < b) {
                a *= 10;
                cnt++;
            }
            for (size_t i = 0; i < precision; i++) {
                a *= 10;
            }
        }

        a /= b;
        std::string res = a.toString();

        int point = res.size() - (cnt + precision);
        int pointPosition;

        if (point >= 0) {
            res.insert(res.begin() + res.size() - (cnt + precision), '.');
            pointPosition = res.size() - (cnt + precision);
        } else {
            res = "0." + std::string(abs(point), '0') + res;
            pointPosition = 1;
        }

        if (res[0] == '.') {
            pointPosition = 1;
            res = "0" + res;
        }
        res = res.substr(0, pointPosition + precision + 1);
        if (res.back() == '.') {
            res.pop_back();
        }

        res = (sign == -1 && a != 0 ? "-" : "") + res;
        return res;
    }

    friend Rational operator+(const Rational &a, const Rational &b);

    friend Rational operator-(const Rational &a, const Rational &b);

    friend Rational operator*(const Rational &a, const Rational &b);

    friend Rational operator/(const Rational &a, const Rational &b);

    friend bool operator==(const Rational &a, const Rational &b);

    friend bool operator!=(const Rational &a, const Rational &b);

    friend bool operator<(const Rational &a, const Rational &b);

    friend bool operator>(const Rational &a, const Rational &b);

    friend bool operator<=(const Rational &a, const Rational &b);

    friend bool operator>=(const Rational &a, const Rational &b);

    Field &getZero() override;

    Field &getOne() override;

    Field &getAddInverse() override;

    Field &getMulInverse() override;
};

bool operator==(const Rational &a, const Rational &b) {
    return a.numerator * b.denominator == a.denominator * b.numerator;
}

bool operator!=(const Rational &a, const Rational &b) {
    return !(a == b);
}

bool operator<(const Rational &a, const Rational &b) {
    return a.numerator * b.denominator < a.denominator * b.numerator;
    // -1 * -3 < 5 * 1, 3 < 5
}

// a * d < b * c

bool operator>(const Rational &a, const Rational &b) {
    return b < a;
}

bool operator<=(const Rational &a, const Rational &b) {
    return !(a > b);
}

bool operator>=(const Rational &a, const Rational &b) {
    return !(a < b);
}


Rational operator+(const Rational &a, const Rational &b) {
    Rational res = a;
    res += b;
    return res;
}

Rational operator-(const Rational &a, const Rational &b) {
    Rational res = a;
    res -= b;
    return res;
}

Rational operator*(const Rational &a, const Rational &b) {
    Rational res = a;
    res *= b;
    return res;
}

Rational operator/(const Rational &a, const Rational &b) {
    Rational res = a;
    res /= b;
    return res;
}

Field &Rational::getZero() {
    return *(new Rational(0));
}

Field &Rational::getOne() {
    return *(new Rational(1));
}

Field &Rational::getAddInverse() {
    Rational *res = new Rational((*this) * (-1));
    return *res;
}

Field &Rational::getMulInverse() {
    Rational *res = new Rational(Rational(1) / (*this));
    return *res;
}

#endif //MATRIX_RATIONAL_H
