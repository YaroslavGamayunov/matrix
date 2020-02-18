//
// Created by Ярослав Гамаюнов on 2020-02-18.
//

#ifndef MATRIX_BIGINTEGER_H
#define MATRIX_BIGINTEGER_H

#include <iostream>
#include <string>
#include <vector>


class BigInteger {
private:
    std::vector<int> digits;
    int sign;

    int trim() {
        int leadingZeros = 0;
        while (!digits.empty() && digits.back() == 0) {
            digits.pop_back();
            leadingZeros++;
        }
        if (digits.empty()) {
            leadingZeros--;
            sign = 1;
            digits = {0};
        }
        return leadingZeros;
    }

    void insertDigit(int i, int digit) {
        digits.insert(digits.begin() + i, digit);
    }

public:
    static const long long base = 1'000'000'000;
    static const int baseExponent = 9;

    explicit BigInteger(const std::string &s) : sign(1) {
        for (int i = s.size(); i > 0; i -= baseExponent) {

            int left = std::max(0, i - baseExponent);
            int right = i;
            int size = right - left;

            int currentDigit = atoi(s.substr(left, size).c_str());
            digits.push_back(currentDigit);
        }
        if (digits.size() == 0) {
            digits = {0};
        } else {
            if (s[0] == '-') {
                digits.back() *= -1;
                sign = -1;
            }
        }
        trim();
    }

    explicit BigInteger(const std::vector<int> &digits, int sign = 1) : digits(digits), sign(sign) { trim(); }

    BigInteger(int a) : BigInteger(std::to_string(a)) {}

    BigInteger(const BigInteger &a) : digits(a.digits), sign(a.sign) {}

    BigInteger() : sign(1) {}

    std::string toString() const {
        std::string res = sign == -1 ? "-" : "";
        for (int i = (int) digits.size() - 1; i >= 0; i--) {
            std::string currentDigit = std::to_string(digits[i]);
            if (i != (int) digits.size() - 1) {
                int leadingZeros = baseExponent - (int) currentDigit.size();
                currentDigit = std::string(leadingZeros, '0') + currentDigit;
            }
            res += currentDigit;
        }

        return res;
    }

    int getSign() const {
        return sign;
    }

    explicit operator bool() const {
        return digits.size() > 0 && digits.back() != 0;
    }

    int operator[](size_t i) const {
        return this->digits[i];
    }

    BigInteger &operator+=(const BigInteger &other) {
        (*this) = (*this) + other;
        return *this;
    }

    BigInteger &operator-=(const BigInteger &other) {
        (*this) = (*this) - other;
        return *this;
    }

    BigInteger &operator*=(const BigInteger &other) {
        (*this) = (*this) * other;
        return *this;
    }

    BigInteger &operator/=(const BigInteger &other) {
        (*this) = (*this) / other;
        return *this;
    }

    BigInteger &operator%=(const BigInteger &other) {
        (*this) = (*this) % other;
        return *this;
    }

    BigInteger operator++(int) { // postfix operator (a++)
        BigInteger tmp(*this);
        ++*this;
        return tmp;
    }

    BigInteger &operator++() {  // prefix operator (++a)
        *this += BigInteger(1);
        return *this;
    }

    BigInteger operator--(int) {  // postfix operator (a--)
        BigInteger tmp(*this);
        --*this;
        return tmp;
    }

    BigInteger &operator--() { // postfix operator (--a)
        *this -= BigInteger(1);
        return *this;
    }

    size_t length() const {
        return this->digits.size();
    }

    friend BigInteger abs(const BigInteger &a);

    friend BigInteger operator+(const BigInteger &a, const BigInteger &b);

    friend BigInteger operator-(const BigInteger &a, const BigInteger &b);

    friend BigInteger operator*(const BigInteger &a, const BigInteger &b);

    friend std::pair<BigInteger, BigInteger> divmod(const BigInteger &a, const BigInteger &b);

    friend BigInteger operator/(const BigInteger &a, const BigInteger &b);

    friend BigInteger operator%(const BigInteger &a, const BigInteger &b);

    BigInteger operator-() const {
        BigInteger result(*this);

        if (result[result.length() - 1] == 0) {
            result.sign = 1;
        } else {
            result.sign *= -1;
        }

        return result;
    }

    BigInteger &operator=(const BigInteger &other) = default;

    BigInteger &operator=(int b) {
        *this = BigInteger(std::to_string(b));
        return *this;
    }

};


bool operator==(const BigInteger &a, const BigInteger &b) {
    if (a.length() != b.length()) {
        return false;
    }
    for (size_t i = 0; i < a.length(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    if (a[a.length() - 1] == 0) {
        return true;
    }
    return a.getSign() == b.getSign();
}

bool operator!=(const BigInteger &a, const BigInteger &b) {
    return !(a == b);
}

bool operator<(const BigInteger &a, const BigInteger &b) {
    if (b.length() > a.length()) {
        return (a.getSign() == -1 && b.getSign() == 1) ||
               (a.getSign() == 1 && b.getSign() == 1);
    } else if (a.length() > b.length()) {
        return !(b < a);
    }
    for (int i = b.length() - 1; i >= 0; i--) {
        int digitA = a[i] * a.getSign();
        int digitB = b[i] * b.getSign();
        if (digitA < digitB) {
            return true;
        } else if (digitA > digitB) {
            return false;
        }
    }
    return false;
}


bool operator>(const BigInteger &a, const BigInteger &b) {
    return b < a;
}

bool operator<=(const BigInteger &a, const BigInteger &b) {
    return !(a > b);
}

bool operator>=(const BigInteger &a, const BigInteger &b) {
    return !(a < b);
}

BigInteger abs(const BigInteger &a) {
    BigInteger res(a);
    res.sign = 1;
    return res;
}

BigInteger operator+(const BigInteger &a, const BigInteger &b) {
    std::vector<int> resultDigits;
    if (a.sign != b.sign) {
        BigInteger absA = abs(a);
        BigInteger absB = abs(b);
        BigInteger res = absA < absB ? absB - absA : absA - absB;
        if ((a.sign == -1 && absA > absB) || (b.sign == -1 && absB > absA)) {
            res.sign = -1;
        }
        return res;
    }

    int digitOverflow = 0;
    for (size_t i = 0; i < std::max(a.length(), b.length()) || digitOverflow; i++) {
        if (i == resultDigits.size()) {
            resultDigits.push_back(0);
        }

        if (i < a.length()) {
            resultDigits[i] += a[i];
        }
        if (i < b.length()) {
            resultDigits[i] += b[i];
        }

        resultDigits[i] += (int) digitOverflow;

        digitOverflow = resultDigits[i] >= BigInteger::base ? 1 : 0;

        if (digitOverflow) {
            resultDigits[i] -= BigInteger::base;
        }
    }
    BigInteger result(resultDigits);
    result.sign = a.sign;
    return result;
}


BigInteger operator-(const BigInteger &a, const BigInteger &b) {
    if (b.sign == -1) {
        return a + abs(b);
    }
    if (a.sign == -1 && b.sign == 1) {
        return -(abs(a) + abs(b));
    }

    if (abs(a) < abs(b)) {
        return -(b - a);
    }

    std::vector<int> resultDigits(a.digits);

    bool digitOverflow = false;
    for (size_t i = 0; i < b.length() || digitOverflow; i++) {
        long long digitA = a[i];
        long long digitB = i < b.length() ? b[i] : 0;
        resultDigits[i] = digitA - digitB;

        if (digitOverflow) {
            resultDigits[i]--;
        }
        digitOverflow = resultDigits[i] < 0;

        if (digitOverflow) {
            resultDigits[i] += BigInteger::base;
        }
    }

    return BigInteger(resultDigits);
}


BigInteger operator*(const BigInteger &a, const BigInteger &b) {

    std::vector<int> resultDigits(a.length() + b.length());

    for (size_t i = 0; i < a.length(); i++) {
        int carry = 0;
        for (size_t j = 0; j < b.length() || carry; j++) {
            long long digitA = a[i];
            long long digitB = j < b.length() ? b[j] : 0;

            long long cur = resultDigits[i + j] + digitA * digitB + carry;

            resultDigits[i + j] = cur % BigInteger::base;
            carry = int(cur / BigInteger::base);
        }
    }

    while (!resultDigits.empty() && resultDigits.back() == 0) {
        resultDigits.pop_back();
    }

    if (a.sign * b.sign == -1) {
        return -BigInteger(resultDigits);
    }
    return BigInteger(resultDigits);
}


std::pair<BigInteger, BigInteger> divmod(const BigInteger &a, const BigInteger &b) {

    if (a.sign != b.sign) {
        std::pair<BigInteger, BigInteger> d = divmod(abs(a), abs(b));
        return {-d.first, -d.second};
    }

    BigInteger buffer;
    BigInteger div;

    int currentIdx = (int) a.length() - 1;
    while (currentIdx >= 0) {

        buffer.insertDigit(0, a[currentIdx--]);

        while (currentIdx >= 0 && b > buffer) {
            buffer.insertDigit(0, a[currentIdx--]);
            div.insertDigit(0, 0);
        }

        buffer.trim();

        int l = 0, r = BigInteger::base;
        while (r - l > 1) {
            int mid = (r + l) / 2;
            if (b * mid > buffer) {
                r = mid;
            } else {
                l = mid;
            }
        }

        buffer -= b * l;

        div.insertDigit(0, l);
    }
    div.trim();
    return {div, buffer};
}

BigInteger operator/(const BigInteger &a, const BigInteger &b) {
    return divmod(a, b).first;
}

BigInteger operator%(const BigInteger &a, const BigInteger &b) {
    return divmod(a, b).second;
}

std::istream &operator>>(std::istream &in, BigInteger &a) {
    std::string num;
    in >> num;
    a = BigInteger(num);
    return in;
}

std::ostream &operator<<(std::ostream &out, const BigInteger &a) {
    out << a.toString();
    return out;
}

#endif //MATRIX_BIGINTEGER_H
