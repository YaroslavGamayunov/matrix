//
// Created by Ярослав Гамаюнов on 2020-02-23.
//

#ifndef MATRIX_MATH_UTILS_H
#define MATRIX_MATH_UTILS_H

#include <vector>
#include "matrix_operations/matrix_operations.h"

bool isPrime(unsigned n) {
    if (n < 2) {
        return false;
    }
    int i = 2;
    while (i * i <= n) {
        if (n % i == 0) {
            return false;
        }
        i++;
    }
    return true;
}

// Finds all primes on [1 ; n] segment
std::vector<unsigned> findPrimesOnSegment(unsigned n) {
    std::vector<bool> prime(n + 1, true);
    prime[0] = false;
    prime[1] = false;
    for (unsigned i = 2; i * i <= n; i++) {
        if (prime[i]) {
            for (unsigned j = i * i; j <= n; j += i) {
                prime[j] = false;
            }
        }
    }

    std::vector<unsigned> res;
    for (unsigned i = 0; i <= n; i++) {
        if (prime[i]) {
            res.push_back(i);
        }
    }
    return res;
}

// Finds first prime greater than n
unsigned findNextPrime(unsigned n) {
    n++;
    while (!isPrime(n)) {
        n++;
    }
    return n;
}

// Finds a^n (mod m)
unsigned modPow(unsigned a, unsigned n, unsigned m) {
    if (n == 0) {
        return 1;
    }
    if (n % 2 == 0) {
        long long x = modPow(a, n / 2, m);
        return (x * x) % (long long) m;
    }
    return ((long long) a * modPow(a, n - 1, m)) % (long long) m;
}

// Finds a + b (mod m)
unsigned modAdd(long long a, long long b, long long m) {
    long long res = (a + b) % m;
    return res;
}

// Finds a * b (mod m)
unsigned modMul(long long a, long long b, long long m) {
    long long res = (a * b) % m;
    return res;
}

// Finds a - b (mod m)
unsigned modSub(long long a, long long b, long long m) {
    long long res = (((a - b) % m) + m) % m;
    return res;
}

template<typename T>
std::vector<T> operator*(const std::vector<T> &a, T b) {
    std::vector<T> res = a;
    for (int i = 0; i < res.size(); i++) {
        res[i] = res[i] * b;
    }
    return res;
}

template<typename T>
std::vector<T> operator+(const std::vector<T> &a, const std::vector<T> &b) {
    // todo add exceptions
    std::vector<T> res = a;
    for (int i = 0; i < res.size(); i++) {
        res[i] = a[i] + b[i];
    }
    return res;
}

template<typename T>
std::vector<T> operator-(const std::vector<T> &a, const std::vector<T> &b) {
    // todo add exceptions
    std::vector<T> res = a;
    for (int i = 0; i < res.size(); i++) {
        res[i] = a[i] - b[i];
    }
    return res;
}

template<unsigned M, unsigned N, typename Field>
std::vector<std::vector<Field>> echelonForm(std::vector<std::vector<Field>> matrixRows,
                                            bool keepZeroRows = true,
                                            std::vector<std::shared_ptr<MatrixOperation<M, N, Field>>> *operationLog = nullptr) {
    unsigned nRows = matrixRows.size();
    unsigned nColumns = matrixRows[0].size();
    int lastProcessedRow = -1;

    for (unsigned column = 0; column < nColumns; column++) {
        int nonZeroPos = -1; // A position in current column with non-zero value

        for (unsigned row = lastProcessedRow + 1; row < nRows; row++) {
            if (matrixRows[row][column] != Field::ZERO) {
                nonZeroPos = row;
                break;
            }
        }

        if (nonZeroPos == -1) {
            continue;
        }

        lastProcessedRow++;

        std::swap(matrixRows[lastProcessedRow], matrixRows[nonZeroPos]);
        if (operationLog != nullptr) {
            operationLog->push_back(
                    std::shared_ptr<MatrixOperation<M, N, Field>>(
                            new MatrixSwapRowsOperation<M, N, Field>(lastProcessedRow, nonZeroPos)));
        }

        for (unsigned row = 0; row < nRows; row++) {
            if (row == lastProcessedRow) {
                continue;
            }

            Field ratio = (matrixRows[row][column] / matrixRows[lastProcessedRow][column]);

            matrixRows[row] =
                    matrixRows[row] - matrixRows[lastProcessedRow] * ratio;
            if (operationLog != nullptr) {
                operationLog->push_back(std::shared_ptr<MatrixOperation<M, N, Field>>(
                        new MatrixAddRowOperation<M, N, Field>(row, lastProcessedRow, -ratio)));
            }
        }
    }

    const std::vector<Field> ZERO_ROW(nColumns, Field::ZERO);

    if (N == M && matrixRows.back() != ZERO_ROW) {

        /*
         * If matrix is square and its rank is N, echelon form is diagonal matrix,
         * that's why we multiply rows by scalar value so that each element become ONE
         */

        for (unsigned row = 0; row < nRows; row++) {
            for (unsigned column = 0; column < nColumns; column++) {
                if (matrixRows[row][column] != Field::ZERO) {
                    Field ratio = Field::ONE / matrixRows[row][column];
                    // TODO Do something to division operator

                    matrixRows[row] = matrixRows[row] * ratio;

                    if (operationLog != nullptr) {
                        operationLog->push_back(std::shared_ptr<MatrixOperation<M, N, Field>>(
                                new MatrixMultiplyRowOperation<M, N, Field>(row, ratio)));
                    }
                }
            }
        }
    }

    if (!keepZeroRows) {
        while (!matrixRows.empty() && matrixRows.back() == ZERO_ROW) {
            matrixRows.pop_back();
        }
    }

    return matrixRows;
}

#endif //MATRIX_MATH_UTILS_H