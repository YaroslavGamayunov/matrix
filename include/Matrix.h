//
// Created by Ярослав Гамаюнов on 2020-02-23.
//

#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H

#include "Rational.h"
#include "../src/math_utils.h"
#include "../src/exceptions/matrix_exceptions.h"
#include "../src/matrix_operations/matrix_operations.h"
#include "../src/num_theory_template_tricks.h"

template<unsigned M, unsigned N, typename Field=Rational>
class Matrix {
public:
    explicit Matrix(Field fillValue) {
        rows = std::vector<std::vector<Field>>(M, std::vector<Field>(N, fillValue));
    }

    Matrix() {
        rows = std::vector<std::vector<Field>>(M, std::vector<Field>(N, Field::ZERO));
    }


    Matrix<N, M, Field> transposed() {
        Matrix<N, M, Field> transposedMatrix;
        for (unsigned i = 0; i < M; i++) {
            for (unsigned j = 0; j < N; j++) {
                transposedMatrix[j][i] = this[i][j];
            }
        }
        return transposedMatrix;
    }

    Matrix<N, M, Field> inverted() {
        COMPILE_ASSERT(N == M);
        if (det() == Field::ZERO) {
            throw DegenerateMatrixException();
        }
        std::vector<std::shared_ptr<MatrixOperation<M, N, Field>>> operationLog;
        echelonForm<M, N, Field>(rows, true, &operationLog);

        Matrix<N, N, Field> identityMatrix;
        for (unsigned i = 0; i < N; i++) {
            identityMatrix[i][i] = Field::ONE;
        }

        for (auto op : operationLog) {
            (*op)(identityMatrix);
        }
        return identityMatrix;
    }

    void invert() {
        COMPILE_ASSERT(N == M);
        if (det() == Field::ZERO) {
            throw DegenerateMatrixException();
        }
        *this = inverted();
    }

    unsigned rank() {
        return echelonForm<M, N, Field>(rows, false).size();
    }

    Field det() {
        COMPILE_ASSERT(N == M);
        Field result = Field::ONE;
        auto triangularForm = echelonForm<M, N, Field>(rows);
        for (unsigned i = 0; i < M; i++) {
            result = result * triangularForm[i][i];
        }
        return result;
    }

    Field trace() {
        COMPILE_ASSERT(N == M);
        Field sum = Field::ZERO;
        for (unsigned i = 0; i < N; i++) {
            sum += rows[i][i];
        }
        return sum;
    }

    std::vector<Field> getRow(unsigned index) {
        return rows[index];
    }

    std::vector<Field> getColumn(unsigned index) {
        std::vector<Field> column(N);
        for (unsigned i = 0; i < N; i++) {
            column[i] = rows[i][index];
        }
        return column;
    }

    std::vector<Field> &operator[](unsigned index) {
        return rows[index]; // todo add exceptions
    }

    std::vector<Field> operator[](unsigned index) const {
        return rows[index];
    }

    Matrix<M, N, Field> &operator-=(const Matrix<M, N, Field> other) {
        for (unsigned i = 0; i < M; i++) {
            for (unsigned j = 0; j < N; j++) {
                (*this)[i][j] -= other[i][j];
            }
        }
        return *this;
    }

    Matrix<M, N, Field> &operator+=(const Matrix<M, N, Field> other) {
        for (unsigned i = 0; i < M; i++) {
            for (unsigned j = 0; j < N; j++) {
                (*this)[i][j] = (*this)[i][j] + other[i][j];
            }
        }
        return *this;
    }

    Matrix<M, N, Field> &operator*=(const Field &val) {
        for (unsigned i = 0; i < M; i++) {
            for (unsigned j = 0; j < N; j++) {
                (*this)[i][j] *= val;
            }
        }
    }

    /*
     * Multiplies MxN and NxK matrices, output is MxK matrix
     */
    template<unsigned K>
    friend Matrix<M, K, Field> operator*(const Matrix<M, N, Field> &a, const Matrix<N, K, Field> &b) {
        if (std::max({M, N, K}) < 2) {
            // Performing ordinary O(n^3) algorithm for matrix multiplication since it is better on small matrices
            Matrix<M, K, Field> result;
            for (unsigned rowA = 0; rowA < M; rowA++) {
                for (unsigned colB = 0; colB < K; colB++) {
                    for (unsigned i = 0; i < N; i++) {
                        result[rowA][colB] = result[rowA][colB] + a[rowA][i] * b[i][colB];
                    }
                }
            }
            return result;
        }

        const int extSize = NEXT_POWER_OF_TWO(std::max({M, N, K}));

        Matrix<extSize, extSize, Field> extendedA;
        Matrix<extSize, extSize, Field> extendedB;

        for (unsigned i = 0; i < M; i++) {
            for (unsigned j = 0; j < N; j++) {
                extendedA[i][j] = a[i][j];
            }
        }

        for (unsigned i = 0; i < N; i++) {
            for (unsigned j = 0; j < K; j++) {
                extendedB[i][j] = b[i][j];
            }
        }

        auto squaresA = extendedA.splitIntoSquares();
        auto squaresB = extendedB.splitIntoSquares();

        Matrix<extSize / 2, extSize / 2, Field> A11 = squaresA[0];
        Matrix<extSize / 2, extSize / 2, Field> A12 = squaresA[1];
        Matrix<extSize / 2, extSize / 2, Field> A21 = squaresA[2];
        Matrix<extSize / 2, extSize / 2, Field> A22 = squaresA[3];

        Matrix<extSize / 2, extSize / 2, Field> B11 = squaresB[0];
        Matrix<extSize / 2, extSize / 2, Field> B12 = squaresB[1];
        Matrix<extSize / 2, extSize / 2, Field> B21 = squaresB[2];
        Matrix<extSize / 2, extSize / 2, Field> B22 = squaresB[3];

        auto P1 = (A11 + A22) * (B11 + B22);
        auto P2 = (A21 + A22) * B11;
        auto P3 = A11 * (B12 - B22);
        auto P4 = A22 * (B21 - B11);
        auto P5 = (A11 + A12) * B22;
        auto P6 = (A21 - A11) * (B11 + B12);
        auto P7 = (A12 - A22) * (B21 + B22);

        auto C11 = P1 + P4 - P5 + P7;
        auto C12 = P3 + P5;
        auto C21 = P2 + P4;
        auto C22 = P1 - P2 + P3 + P6;

        Matrix<extSize, extSize, Field> C({C11, C12, C21, C22});

        return Matrix<M, K, Field>(C.getSubMatrix({0, 0}, {M - 1, K - 1}));
    }

    /*
     * Multiplies NxN matrices
     * Note that these matrices are square!
     */
    Matrix<N, N, Field> &operator*=(const Matrix<N, N, Field> &other) {
        return *this = *this * other;
    }

private:
    // This was made in order to be able to access private methods of M1 x N1 matrix from M x N matrix
    template<unsigned M1, unsigned N1, typename F>
    friend
    class Matrix;

    std::vector<std::vector<Field>> rows;

    Matrix(const std::vector<std::vector<Field>> &rows) : rows(rows) {}

    /*
     * Params: lt and rb are left top and right bottom corners, lt and rb are pairs of {i, j},
     * where i is row pos and j is column pos
     *
     * Returns: vector containing what lays in area between lt and rb
     */

    std::vector<std::vector<Field>> getSubMatrix(std::pair<unsigned, unsigned> lt, std::pair<unsigned, unsigned> rb) {
        // todo bound exceptions
        unsigned int height = rb.first - lt.first + 1;
        unsigned int width = rb.second - lt.second + 1;

        std::vector<std::vector<Field>> subMatrix(height, std::vector<Field>(width, Field::ZERO));
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                subMatrix[i][j] = rows[lt.first + i][lt.second + j];
            }
        }
        return subMatrix;
    }

    void replaceSubMatrix(std::pair<unsigned, unsigned> lt, std::pair<unsigned, unsigned> rb,
                          const std::vector<std::vector<Field>> &replacementMatrix) {
        // todo bound exceptions

        unsigned int height = rb.first - lt.first + 1;
        unsigned int width = rb.second - lt.second + 1;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                rows[lt.first + i][lt.second + j] = replacementMatrix[i][j];
            }
        }
    }

    /*
     * Splits square matrix into four equal squares
     * If matrix has shape of:
     * A11 A12
     * A21 A22
     * It returns {A11, A12, A21, A22}
     */
    std::vector<Matrix<N / 2, N / 2, Field>> splitIntoSquares() {
        COMPILE_ASSERT((N == M));

        Matrix<N / 2, N / 2, Field> A11(getSubMatrix({0, 0}, {N / 2 - 1, N / 2 - 1}));
        Matrix<N / 2, N / 2, Field> A12(getSubMatrix({0, N / 2}, {N / 2 - 1, N - 1}));
        Matrix<N / 2, N / 2, Field> A21(getSubMatrix({N / 2, 0}, {N - 1, N / 2 - 1}));
        Matrix<N / 2, N / 2, Field> A22(getSubMatrix({N / 2, N / 2}, {N - 1, N - 1}));

        std::vector<Matrix<N / 2, N / 2, Field>> res = {A11, A12, A21, A22};

        return res;
    }

    Matrix(std::vector<Matrix<N / 2, N / 2, Field>> squares) {

        rows = std::vector<std::vector<Field>>(M, std::vector<Field>(N, Field::ZERO));

        replaceSubMatrix({0, 0}, {N / 2 - 1, N / 2 - 1}, squares[0].rows);
        replaceSubMatrix({0, N / 2}, {N / 2 - 1, N - 1}, squares[1].rows);
        replaceSubMatrix({N / 2, 0}, {N - 1, N / 2 - 1}, squares[2].rows);
        replaceSubMatrix({N / 2, N / 2}, {N - 1, N - 1}, squares[3].rows);
    }
};


template<unsigned M, unsigned N, typename Field>
Matrix<M, N, Field> operator*(const Matrix<M, N, Field> &a, const Field &val) {
    Matrix<M, N, Field> res = a;
    res *= val;
    return res;
}

template<unsigned M, unsigned N, typename Field>
Matrix<M, N, Field> operator+(const Matrix<M, N, Field> &a, const Matrix<M, N, Field> &b) {
    Matrix<M, N, Field> res = a;
    res += b;
    return res;
}

template<unsigned M, unsigned N, typename Field>
Matrix<M, N, Field> operator-(const Matrix<M, N, Field> &a, const Matrix<M, N, Field> &b) {
    Matrix<M, N, Field> res = a;
    res -= b;
    return res;
}


template<unsigned N, typename Field=Rational>
using SquareMatrix = Matrix<N, N, Field>;

#endif //MATRIX_MATRIX_H
